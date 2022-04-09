#include "Data_Storage.h"
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
#include "state_class.hpp"

#define DEBUG false

IntervalTimer timer_32Hz; // BME
IntervalTimer timer_100Hz; // IMU
 
uint64_t bme_last_update = 0;
uint64_t imu_last_update = 0;
uint64_t gps_last_update = 0;

myBME bme(9);
myIMU imu;
myGPS gps;

bool allGood{0};

State fsm;

Data_Storage storage;

bool initSensors();
void initState();
bool initStorage();

void runSensors();
void runState();
void runStorage();

void init_timers();
void timer_32Hz_func();
void timer_100Hz_func();

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms
  delay(1000); // testing delay, not necessary for flight

  pinMode(GRNLEDPIN, OUTPUT);
  pinMode(REDLEDPIN, OUTPUT);

  digitalWrite(REDLEDPIN, 0);
  digitalWrite(GRNLEDPIN, 0);
  
  allGood = initSensors() && initStorage();

  initState();
  
  init_timers();
}

uint32_t counter = 0;

void loop()
{ 
  runSensors();
  runState();
  runStorage();
}


bool initSensors()
{
  bool sensorsWork{1};

  if(!bme.start())
  {
    sensorsWork = 0;
    Serial.println("BME failed.");
  }
  else if (!imu.start(10))
  {
    sensorsWork = 0;
    Serial.println("IMU failed.");
  }
  
  // Initialize GPS
  gps.start();

  return sensorsWork;
}


void initState()
{
  // Initialize State Machine
  fsm.initializeMachine(allGood,&bme); // pass a true value if sensors are good
}


bool initStorage()
{
  // Initialize File Storage  
  storage.init();
  bool initSuccessful = 1;


  //char dummy[13] = "fuck you.csv";


  if (storage.exists(bme_filename))
  {
    storage.dumpFile(bme_filename);
    initSuccessful = 0;
  }

  if (storage.exists(imu_filename))
  {
    storage.dumpFile(imu_filename);
    initSuccessful = 0;
  }

  if (storage.exists(gps_filename))
  {
    storage.dumpFile(gps_filename);
    initSuccessful = 0;
  }

  if (storage.exists(fsm_filename))
  {
    storage.dumpFile(fsm_filename);
    initSuccessful = 0;
  }
  
  storage.formatFlash();  // may not want to format on bootup (possible inflight data loss)
  if (initSuccessful)
  {
    storage.initFiles(&bme,&imu,&gps,&fsm);
  }
  else
    Serial.println("Data storage failed. Good luck");

  while(millis() <= 10*1000); // wait till 10 seconds after bootup, only meant for testing

  return initSuccessful;
}


void runSensors()
{
  // RUN SENSORS
  //bme.getData();
  //imu.getData();
  gps.getData();
}


void runState()
{
  // RUN FINITE STATE MACHINE
  fsm.machine();  
}


void runStorage()
{
  // RUN DATA LOGGING
  // If sensor logging is in high rate mode, log everytime the sensors sample
  // If sensor logging is in low rate mode, log only a small subset of times the sensors sample
  storage.runLogs();
  // Dump data to SD Card post landing confirmation
  if(millis() >= (10+30)*1000) // testing after 70 seconds past bootup, real code executes when FSM landing flag goes high
  //if (fsm.getState() == Landing_Idle)
  {
    timer_32Hz.end();
    timer_100Hz.end();

    storage.dumpData();

    while(1);
  }
}

void init_timers()
{
  SPI.usingInterrupt(timer_32Hz);
  SPI.usingInterrupt(timer_100Hz);
  timer_32Hz.begin(timer_32Hz_func,1000000/bme_update_frequency); // 30 Hz interrupt
  timer_32Hz.priority(129);
  timer_100Hz.begin(timer_100Hz_func,1000000/imu_update_frequency); // 100 Hz interrupt
  timer_100Hz.priority(128);
}


void timer_32Hz_func()
{
  bme.getData();
}


void timer_100Hz_func()
{
  imu.getData();
}