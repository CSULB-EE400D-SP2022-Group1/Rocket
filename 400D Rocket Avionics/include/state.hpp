#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include <fstream>

// Set to true to enable serial monitor debugging info
bool DEBUG[true];  

//*****************************************************************************
// Interrupt function to pass 'IRQ = true' to machine - Testing purposes only
bool irq_BME[true];
bool irq_IMU[true];
bool irq_GPS[true];

bool IRQ (bool BME, bool IMU, bool GPS) {
  if (BME == true &&
      IMU == true && 
      GPS == true) {
    return true;

  } else {
    return false;
  };
};
//*****************************************************************************

// Logging functionality
std::fstream log_file;

// Build STATES list as char
enum state_list {
  POWER_ON,
  INIT,
  PAD_HOLD,
  PAD_IDLE,
  ASCENT,
  DESCENT,
  LANDING,
  LANDING_IDLE
};

// Build flag_list as bool
struct flag_list {
  // default flag initializeialization
  bool flag = false;
} power_on,
  initialize,
  initialize_error,
  standby,
  launch,
  apogee,
  drogues,
  crit_alt,
  main_chute,
  touchdown;

// Build log_list as bool
struct log_list {
  // default flag initializeialization
  bool log = true;
} power_on_log,
  initialize_log,
  standby_log,
  launch_log,
  apogee_log,
  drogues_log,
  crit_alt_log,
  main_chute_log,
  touchdown_log;


// State transition variables take STATE list values (enum)
state_list state_now;
state_list state_prev;

#endif