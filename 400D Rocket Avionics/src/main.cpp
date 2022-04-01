#include <Arduino.h>
#include "Data_Storage.h"

Data_Storage storage;
File dataFile;

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms

  // Initialize BME

  // Initialize IMU

  // Initialize GPS

  // Initialize State Machine

  // Initialize File Storage  
  storage.init();
  storage.formatFlash();  
  storage.initFiles();
}

void loop()
{
  
}