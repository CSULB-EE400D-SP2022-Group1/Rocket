#include <state.hpp>

// the machine
void machine() {
  // state history
  state_prev = state_now;

  // state machine switch structure
  switch (state_now) {

    case POWER_ON:
      // wait for 250 milliseconds
      delay(250);

      // throw flag - internal signal
      power_on.flag = true;

      // State_log entry
      while (power_on_log.log == true) {
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "System powered on at " /* << clock access */ << "\n" ;
      
        // debug out
        Serial.println("System is in POWER_ON state. Next state is INIT");      

        // one-time logging
        power_on_log.log = false;
      }

      // conditional for 'INIT' state - for now, default true
      if (IRQ(irq_BME, irq_IMU, irq_GPS) == true) {
        state_now = INIT;

        // State_log entry
        log_file << "Proceeding to INIT state\n\n";

        // debug out
        Serial.println("Proceeding to INIT");  
      } 
    
    log_file.close();
    break;

    case INIT:
      // throw flag - internal signal
      initialize.flag = true;
      
      while (initialize_log.log == true) {
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "System initializeialized at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("System is in INIT state. Next state is PAD_IDLE or PAD_HOLD");

        // one-time flag
        initialize_log.log = false;
      }

      // conditional for PAD_IDLE
      if (IRQ(irq_BME, irq_IMU, irq_GPS) == true && initialize.flag == true) {
        state_now = PAD_IDLE;

        // State_log entry
        log_file << "Sensors detected. Proceeding to PAD_IDLE state\n\n";

        // debug out
        Serial.println("Sensors detected. Proceeding to PAD_IDLE state");

      // conditional for PAD_HOLD
      } else {
        state_now = PAD_HOLD;

        // State_log entry
        log_file << "Sensor error. Proceeding to PAD_HOLD state\n\n";

        // debug out
        Serial.println("Sensor error. Proceeding to PAD_HOLD state");
      }

    log_file.close();
    break;

    case PAD_IDLE:
      // throw flag - internal signal
      standby.flag = true;

      while (standby_log.log == true) {
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "System entered standby at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("System is in PAD_IDLE state. Next state is ASCENT");

        // one-time logging
        standby_log.log = false;
      }

      // conditional for ASCENT - external signal
      if (standby.flag == true && launch.flag == true) {
        state_now = ASCENT;

        // State_log entry
        log_file << "Launch event detected. Proceeding to ASCENT state\n\n";  

        // debug out
        Serial.println("Launch event detected. Proceeding to ASCENT state");    
      }

    log_file.close();
    break;

    case PAD_HOLD:
      // throw flag - internal signal
      initialize_error.flag = true;

      // debug out
      Serial.println("Sensor initializeialization error. Restart required");
    break;
 
    case ASCENT:
      while (launch_log.log == true) {   
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "System launch at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("System is in ASCENT state. Next state is DESCENT");          

        // one-time logging
        launch_log.log = false;
      }

      // conditional for DESCENT - external signal
      if (launch.flag == true && apogee.flag == true) {
        state_now = DESCENT;

        // State_log entry
        log_file << "Apogee event detected. Proceeding to DESCENT state\n\n"; 

        // debug out
        Serial.println("Apogee event detected. Proceeding to DESCENT state");
      }

    log_file.close();
    break;

    case DESCENT:
      // deploy drogue chutes - external signal
      drogues.flag = true;

      while (apogee_log.log == true && drogues_log.log == true) {
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "System apogee at " /* << clock access */ << "\n" ;
        log_file << "Drogues deployed at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("Drogue deploy event. Waiting on critical altitude event.");
        Serial.println("Rocket is in DESCENT state. Next state is LANDING");

        //one-time logging
        apogee_log.log = false;
        drogues_log.log = false;
      }

      // conditional for LANDING - external signal
      if (drogues.flag == true && crit_alt.flag == true) {
        state_now = LANDING;
        crit_alt.flag = true;

        // State_log entry
        log_file << "Critical altitude reached at " /* << clock access */ << "Proceeding to LANDING state\n" ;

        // debug out
        Serial.println("Critical altitude event detected. Proceeding to LANDING state");
      }

    log_file.close();
    break;
 
    case LANDING:
      // Deploy main chute - external signal
      main_chute.flag = true;

      while (main_chute_log.log == true) {
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "Main chute deployed at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("Main chute deploy event detected. Waiting on touchdown event.");

        //one-time logging
        main_chute_log.log = false;
      }

      // conditional for LANDING_IDLE - external signal
      if (main_chute.flag == true && touchdown.flag == true) {
        state_now = LANDING_IDLE;

        // State_log entry
        log_file << "Touchdown detected at " /* << clock access */ << "Proceeding to LANDING_IDLE state\n" ;

        // debug out
        Serial.println("Touchdown event detected. Proceeding to LANDING_IDLE state");
      }

    log_file.close();  
    break;

     case LANDING_IDLE:
      // throw flag - internal signal
      touchdown.flag = true;

      while (main_chute_log.log == true) {
        // State_log entry
        log_file.open ("test\\State_logs.txt", std::ios::app);
        log_file << "End of flight at " /* << clock access */ << "\n" ;

        // debug out
        Serial.println("END OF FLIGHT");
      }

    log_file.close();
    break;
  } // switch
} // machine
