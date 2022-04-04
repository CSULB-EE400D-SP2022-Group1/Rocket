#pragma once
#include <state.hpp>

// the machine
static void machine() {
  // state history

  // state machine switch structure
  switch (state_now) {
    case POWER_ON:
      // wait for 250 milliseconds
      delay(250);

      // throw flag - internal signal
      power_on.flag = true;

      // State_log entry
      while (power_on_log.entry == true) {
        // debug out
        Serial.println("System is in POWER_ON state. Next state is INIT");      

        // one-time logging
        power_on_log.entry = false;
      }

      // conditional for 'INIT' state - external signals (Simulate true, for now)
      if (IRQ(irq_BME, irq_IMU, irq_GPS) == true) {
        state_now = INIT;

        // debug out
        Serial.println("Proceeding to INIT");  
      } 
    

    break;

    case INIT:
      // throw flag - internal signal
      initialize.flag = true;
      
      while (initialize_log.entry == true) {
        // debug out
        Serial.println("System is in INIT state. Next state is PAD_IDLE or PAD_HOLD");

        // one-time flag
        initialize_log.entry = false;
      }

      // conditional for PAD_IDLE
      if (IRQ(irq_BME, irq_IMU, irq_GPS) == true && initialize.flag == true) {
        state_now = PAD_IDLE;

        // debug out
        Serial.println("Sensors detected. Proceeding to PAD_IDLE state");

      // conditional for PAD_HOLD
      } else {
        state_now = PAD_HOLD;

        // debug out
        Serial.println("Sensor error. Proceeding to PAD_HOLD state");
      }

    break;

    case PAD_IDLE:
      // throw flag - internal signal
      standby.flag = true;

      while (standby_log.entry == true) {
        // debug out
        Serial.println("System is in PAD_IDLE state. Next state is ASCENT");

        // one-time logging
        standby_log.entry = false;
      }

      // conditional for ASCENT - external signal
      if (standby.flag == true && launch.flag == true) {
        state_now = ASCENT;

        // debug out
        Serial.println("Launch event detected. Proceeding to ASCENT state");    
      }

    break;

    case PAD_HOLD:
      // throw flag - internal signal
      initialize_error.flag = true;

      // debug out
      Serial.println("Sensor initializeialization error. Restart required");
    break;
 
    case ASCENT:
      while (launch_log.entry == true) {   
        // debug out
        Serial.println("System is in ASCENT state. Next state is DESCENT");          

        // one-time logging
        launch_log.entry = false;
      }

      // conditional for DESCENT - external signal
      if (launch.flag == true && apogee.flag == true) {
        state_now = DESCENT;

        // debug out
        Serial.println("Apogee event detected. Proceeding to DESCENT state");
      }

    break;

    case DESCENT:
      // deploy drogue chutes - external signal
      drogues.flag = true;

      while (apogee_log.entry == true && drogues_log.entry == true) {
        // debug out
        Serial.println("Drogue deploy event. Waiting on critical altitude event.");
        Serial.println("Rocket is in DESCENT state. Next state is LANDING");

        //one-time logging
        apogee_log.entry = false;
        drogues_log.entry = false;
      }

      // conditional for LANDING - external signal
      if (drogues.flag == true && crit_alt.flag == true) {
        state_now = LANDING;
        crit_alt.flag = true;

        // debug out
        Serial.println("Critical altitude event detected. Proceeding to LANDING state");
      }

    break;
 
    case LANDING:
      // Deploy main chute - external signal
      main_chute.flag = true;

      while (main_chute_log.entry == true) {
        // debug out
        Serial.println("Main chute deploy event detected. Waiting on touchdown event.");

        //one-time logging
        main_chute_log.entry = false;
      }

      // conditional for LANDING_IDLE - external signal
      if (main_chute.flag == true && touchdown.flag == true) {
        state_now = LANDING_IDLE;

        // debug out
        Serial.println("Touchdown event detected. Proceeding to LANDING_IDLE state");
      }

    break;

     case LANDING_IDLE:
      // throw flag - internal signal
      touchdown.flag = true;

      while (main_chute_log.entry == true) {
        // debug out
        Serial.println("END OF FLIGHT");
      }

    break;
  } // switch
} // machine
