#include "Data_Storage.h"
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
#include "state_class.hpp"

#define DEBUG false

uint64_t bme_last_update = 0;
uint64_t imu_last_update = 0;
uint64_t gps_last_update = 0;

myBME bme(9);
myIMU imu;
myGPS gps;

State fsm;

Data_Storage storage;

void initSensors();
void initState();
void initStorage();

void runSensors();
void runState();
void runStorage();

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms
  delay(1000); // testing delay, not necessary for flight

  initSensors();
  initState();
  initStorage();
}

uint32_t counter = 0;

void loop()
{ 
  runSensors();
  runState();
  runStorage();
}


void initSensors()
{
  // Initialize BME
  bme.start();

  // Initialize IMU
  imu.start(10);

  // Initialize GPS
  gps.start();
}


void initState()
{
  // Initialize State Machine
  fsm.initializeMachine(true,&bme); // pass a true value if sensors are good
}


void initStorage()
{
  // Initialize File Storage  
  storage.init();
  storage.formatFlash();  // may not want to format on bootup (possible inflight data loss)
  storage.initFiles(&bme,&imu,&gps);

  while(millis() <= 10*1000); // wait till 10 seconds after bootup, only meant for testing
}


void runSensors()
{
  // RUN SENSORS
  bme.getData();
  imu.getData();
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
  {
    storage.dumpData();
    while(1);
  }
}