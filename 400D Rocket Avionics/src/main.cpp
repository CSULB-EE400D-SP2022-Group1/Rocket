#include <Arduino.h>
#include "Data_Storage.h"

Data_Storage storage;

void setup()
{
  Serial.begin(1000000); // initialize fast serial comms

  // Initialize BME

  // Initialize IMU

  // Initialize GPS

  // Initialize State Machine

  // Initialize File Storage  
  storage.init();
  storage.formatFlash();  // may not want to format on bootup (possible inflight data loss)
  storage.initFiles();
}

void loop()
{
  
}