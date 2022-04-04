#include "state_class.hpp"

bool sensorsGreen = true;
bool detectLaunch = true;
bool detectApogee = true;
bool criticalAltitude = true;
bool touchdown = true;


void State::initializeMachine(bool sensors)
{
    if (sensors == true)
    {
        state_list stateNow = static_cast<state_list>(0);
        transition_list transition = static_cast<transition_list>(0);
        bool sensorsGreen = true;
    }
}

char State::getState()
{
    return stateNow;
}

char State::getTransition()
{
    return transition;
}

bool State::getTransitionEvent()
{
    return transitionEvent;
}

// the machine
void State::machine() 
{
  // state machine switch structure
  switch (stateIndex)
  {
    case Init:
      Serial.println("System startup. Init state");     

      if (sensorsGreen == true)
      {
        stateNext = Pad_Idle;
        transition = Init_to_Pad_Idle;
        transitionEvent = true;

        // debug out
        Serial.println("Sensors detected. Proceeding to Pad_Idle state");
      }
      else
      {
        stateNext = Pad_Hold;
        transition = Init_to_Pad_Hold;
        transitionEvent = true;

        // debug out
        Serial.println("Sensors not detected. Proceeding to Pad_Hold state");
      };
    
    stateIndex = stateNext;
    break;

    case Pad_Idle:
      // reset flag
      transitionEvent = false;

      if (detectLaunch == true)
      {
        stateNext = Ascent;
        transition = Pad_Idle_to_Ascent;
        transitionEvent = true;

        // debug out
        Serial.println("Launch detected. Proceeding to Ascent state");
      }
      else
      {
        stateNext = Pad_Idle;

        // debug out
        Serial.println("Awaiting Launch. Standby in Pad_Idle state");        
      }

    stateIndex = stateNext;
    break;

    case Pad_Hold:
      // reset flag
      transitionEvent = false;
      stateNext = Pad_Hold;

      // debug out
      Serial.println("System Error. Restart required");

    stateIndex = stateNext;
    break;

    case Ascent:
      // reset flag
      transitionEvent = false;

      while (detectApogee == false)
      {
        stateNext = Ascent;

        // debug out
        Serial.println("Rocket is Ascending");        
      }

      if (detectApogee == true)
      {
        stateNext = Descent;
        transition = Ascent_to_Descent;
        transitionEvent = true;

        // debug out
        Serial.println("Apogee detected. Proceeding to Descent state");        
      }
    stateIndex = stateNext;
    break;

    case Descent:
      // reset flag
      transitionEvent = false;

      while (criticalAltitude == false) 
      {
        stateNext = Descent;

        // debug out
        Serial.println("Rocket is Descending");          
      }

      if (criticalAltitude == true)
      {
        stateNext = Landing;
        transition = Descent_to_Landing;
        transitionEvent = true;

        // debug out
        Serial.println("Critical Altitude achieved. Proceeding to Landing state");        
      }

    stateIndex = stateNext;
    break;
 
    case Landing:
      // reset flag
      transitionEvent = false;

      while (touchdown == false) 
      {
        stateNext = Landing;

        // debug out
        Serial.println("Rocket is drifting softly to the Earth");          
      }

      if (touchdown == true)
      {
        stateNext = Landing_Idle;
        transition = Landing_to_Landing_Idle;
        transitionEvent = true;

        // debug out
        Serial.println("Touchdown detected. Proceeding to Landing_Idle state");        
      }
    stateIndex = stateNext;
    break;

    case Landing_Idle:
      // reset flag
      transitionEvent = false;

        stateNext = Landing_Idle;

        // debug out
        Serial.println("Eagle has Landed");          
  
    stateIndex = stateNext;
    break;

  } // switch
} // machine
