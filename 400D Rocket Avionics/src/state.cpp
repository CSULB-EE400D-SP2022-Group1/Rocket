#include "state_class.hpp"
/*
bool sensorsGreen = true;
bool detectLaunch = true;
bool detectApogee = true;
bool criticalAltitude = true;
bool touchdown = true;
*/
#define DEBUG_STATE_MACHINE true

void State::initializeMachine(bool sensors, myBME *someBME)
{
    if (sensors == true)
    {
        stateNow = static_cast<state_list>(0);
        transition = static_cast<transition_list>(0);
        bool sensorsGreen = true;
        thisBME = someBME;
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

int State::avgOne ()
{
  return thisBME->getAvgRecent();
}

int State::avgThree ()
{
  return thisBME->getAvg();
}

bool State::detectApogee ()
{
  int index = 0;
  int count = 0;

  while (index < 5)
  {
    if (count == 5)
    {
      break;
    }
    else if (avgOne() < avgThree())
    {
      count++;
      index++;
      continue;
    }
    else
    {
      index = 0;
      continue;
    }
  }
  return true;
}


// the machine
void State::machine() 
{

  thisBME->getData();

  // state machine switch structure
  switch (stateIndex)
  {
    case Init:
      #if DEBUG_STATE_MACHINE
      Serial.println("System startup. Init state");     
      #endif

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

      if (thisBME->detectLaunch() == true)
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
      
    //detectLaunch = true;
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

      if (detectApogee() == true)
      {
        stateNext = Descent;
        transition = Ascent_to_Descent;
        transitionEvent = true;

        // debug out
        Serial.println("Apogee detected. Proceeding to Descent state");        
      }
      else 
      {
        stateNext = Ascent;

        // debug out
        Serial.println("Rocket is Ascending");        
      }
      
    //detectApogee = true;   
    stateIndex = stateNext;
    break;

    case Descent:
      // reset flag
      transitionEvent = false;

      if (thisBME->getAltitude() < 5)
      {
        stateNext = Landing;
        transition = Descent_to_Landing;
        transitionEvent = true;

        // debug out
        Serial.println("Critical Altitude achieved. Proceeding to Landing state");        
      }
      else
      {
        stateNext = Descent;

        // debug out
        Serial.println("Rocket is Descending");          
      }
      
    //criticalAltitude = true;
    stateIndex = stateNext;
    break;
 
    case Landing:
      // reset flag
      transitionEvent = false;

      if (thisBME->getAltitude() > 2)
      {
        stateNext = Landing;

        // debug out
        Serial.println("Rocket is drifting softly to the Earth");          
      }

      else
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
