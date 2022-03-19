#include <Arduino.h>

//Set to true to enable serial monitor debugging info
bool DEBUG[true];  

//Switch Variables:
bool irq_BME[true];
bool irq_IMU[true];
bool irq_GPS[true];

bool go_launch[true];

// fake interrupt function to pass 'IRQ = true' to machine
bool IRQ (bool BME, bool IMU, bool GPS) {
  if (BME == true &&
      IMU == true && 
      GPS == true) {
    return true;

  } else {
    return false;
  };
};

// Build STATES list
enum state_list {
  INIT,
  PAD_HOLD,
  PAD_IDLE,
  ASCENT,
  DESCENT,
  LANDING,
  LANDING_IDLE
};

// Build flag list as bool
struct flag_list {
  bool flag;
} init,
  init_error,
  standby,
  launch,
  apogee,
  drogues,
  crit_alt,
  pyro_arm,
  main_chute;

// State transition variables take STATE list values (enum)
state_list state_now;
state_list state_prev;

// the machine
void machine() {
  // state history
  state_prev = state_now;

  // state machine switch structure
  switch (state_now) {

    case INIT:
      // Make files upon initialization

      // conditional statement for 'pad_****' state - for now, default true
      if (IRQ(irq_BME, irq_IMU, irq_GPS) == true) {
        state_now = PAD_IDLE;

        // throw flags for later state conditional statements
        init.flag = true;

        // debug monitoring
        Serial.println("init.flag is true");
        Serial.println("initialize to pad_idle");

      } else {
        // reinitialize? user input here
        state_now = PAD_HOLD;
        Serial.println("initialize to pad_hold");
      }
      break;

    case PAD_IDLE:
      if (init.flag == true) {
        state_now = PAD_IDLE;

        // launch ready? looking for user input from loop - for now, default true
        //if (go_launch == true) {
        standby.flag = true;
        //};

        Serial.println("standby.flag is true");
        Serial.println("pad_idle to ascent");
      }
      break;

    case PAD_HOLD:
      state_now = PAD_HOLD;
      init_error.flag = true;

      Serial.println("init_error.flag is true");
      Serial.println("pad_hold to init");
    break;
 
    case ASCENT:
      if (standby.flag == true && launch.flag == true) {
      state_now = ASCENT;
      }

      launch.flag = true;

      //if apogee event statement here

      Serial.println("apogee.flag is true");
      Serial.println("ascent to descent");
    break;

    case DESCENT:
      if (apogee.flag == true) {
      state_now = DESCENT;
      }
      // code for drogues
      apogee.flag = true;
      drogues.flag = true;

      Serial.println("drogues.flag is true");
      Serial.println("descent to landing");
    break;
 };

/*
    case DESCENT:
      if (apogee.flag == true)
      state_now = ASCENT;

      std::cout << "apogee.flag is " << apogee.flag << "\n";
      std::cout << "ascent to descent" << "\n\n" ;
    break;

    case LANDING:

    break;

    case LANDING_IDLE:

    break;
*/    
};

void setup() {
  // initialize digital pins

  //if DEBUG is turned on, intiialize serial connection
  if(DEBUG) {
    Serial.begin(115200);
    Serial.println("Debugging is ON");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  machine();

  //If debug is enabled, do some printing of messages

  //Let the world know when the state changes

}
