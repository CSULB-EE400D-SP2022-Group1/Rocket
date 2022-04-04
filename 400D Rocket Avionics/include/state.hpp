#ifndef STATE_H
#define STATE_H

#pragma once
#include <Arduino.h>
#include <fstream>

//*****************************************************************************
// Interrupt function to pass 'IRQ = true' to machine - Testing purposes only
static bool irq_BME[true];
static bool irq_IMU[true];
static bool irq_GPS[true];

static bool IRQ (bool BME, bool IMU, bool GPS) {
  if (BME == true &&
      IMU == true && 
      GPS == true) {
    return true;

  } else {
    return false;
  };
};
//*****************************************************************************

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
static struct flag_list {
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
static struct log_list {
  // default flag initializeialization
  bool entry = true;
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
static state_list state_now;

#endif