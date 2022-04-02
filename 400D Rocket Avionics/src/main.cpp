#include <Arduino.h>
#include "Data_Storage.h"

#define DEBUG false

Data_Storage storage;

void test_datalogging();

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms
  delay(5000); // testing delay, not necessary for flight

  // Initialize BME


  // Initialize IMU


  // Initialize GPS


  // Initialize State Machine


  // Initialize File Storage  
  storage.init();
  storage.formatFlash();  // may not want to format on bootup (possible inflight data loss)
  storage.initFiles();

#if DEBUG
  test_datalogging(); // not for flight, just for testing
#endif
}

void loop()
{
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
  // Generate fake data to test storage functionality
  for(int i = 0; i < 10; i++)
  {
    String dataLine;    
    dataLine += String(i*50000);
    dataLine += ",";
    dataLine += String(1000.0*sin(i*2*3.14159*0.005),6); // need to make sure full float data is saved (that's what the 6 is for, allows for 6 digits after decimal place)
    dataLine += ",";
    dataLine += String(10.0*sin(i*2*3.14159*0.01) + 30.0,6);
    dataLine += ",";
    dataLine += String(10.0*sin(i*2*3.14159*0.1) + 50.0,6);    
    storage.writeData("BME.csv",dataLine);
  }
  storage.dumpData();
}