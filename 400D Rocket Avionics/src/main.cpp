#include "Data_Storage.h"
<<<<<<< Updated upstream
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
#include "state_class.hpp"
=======
//#include "state.hpp"
>>>>>>> Stashed changes

#define DEBUG true

myBME bme(9);
myIMU imu;
myGPS gps;

Data_Storage storage;

void test_datalogging();

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms
  delay(3000); // testing delay, not necessary for flight

  // Initialize BME
  bme.start(30);

  // Initialize IMU
  imu.start(10,100,100);

  // Initialize GPS
  gps.start();

  // Initialize State Machine
  int (myBME::*ptrAvg)() = &myBME::getAvg;
  int (myBME::*ptrAvgRecent)() = &myBME::getAvgRecent;
  bool (myBME::*ptrDetectLaunch)() = &myBME::detectLaunch;

  // Initialize File Storage  
  storage.init();
  storage.formatFlash();  // may not want to format on bootup (possible inflight data loss)
  storage.initFiles(&bme,&imu,&gps);
}

void loop()
{
#if DEBUG
  test_datalogging(); // not for flight, just for testing
#endif
  // Run sensors here


  // Run data logging here
  // If sensor logging is in high rate mode, log everytime the sensors sample
  // If sensor logging is in low rate mode, log only a small subset of times the sensors sample


  // Run state machine here


  // Run other stuff here
  // example: dumping data from NOR flash to SD cards after FSM landing flag is raised


}


void test_datalogging()
{
  for(int i = 0; i < 10; i++)
  {
    bme.getData();
    imu.getData();
    gps.getData();

    storage.logBME();
    storage.logIMU();
    storage.logGPS();

    delay(1000);
  }
  storage.dumpData();

  while(1);
}