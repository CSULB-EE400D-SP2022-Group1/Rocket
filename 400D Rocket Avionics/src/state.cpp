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
        millisThen = millis();
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

bool State::debugTimer (unsigned long millisThen)
{
  unsigned long millisNow = millis();
  
  if (millisNow - millisThen >= 250)
  {
    millisThen = millisNow;
    return true;
  }
}


// the machine
void State::machine() 
{
  elapsedMillis();

  // state machine switch structure
  switch (stateIndex)
  {
    case Init:
      #if DEBUG_STATE_MACHINE
      if (debugTimer(millisThen))
      {
        Serial.println("System startup. Init state");   
      }  
      #endif

      if (sensorsGreen == true)
      {
        stateNext = Pad_Idle;
        transition = Init_to_Pad_Idle;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Sensors detected. Proceeding to Pad_Idle state");
        }
        #endif
      }
      else
      {
        stateNext = Pad_Hold;
        transition = Init_to_Pad_Hold;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Sensors not detected. Proceeding to Pad_Hold state");
        }
        #endif
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

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
        Serial.println("Launch detected. Proceeding to Ascent state");
        }
        #endif
      }
      else
      {
        stateNext = Pad_Idle;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
        Serial.println("Awaiting Launch. Standby in Pad_Idle state");
        }
        #endif        
      }
      
    //detectLaunch = true;
    stateIndex = stateNext;
    break;

    case Pad_Hold:
      // reset flag
      transitionEvent = false;
      stateNext = Pad_Hold;

      #if DEBUG_STATE_MACHINE
      if (debugTimer(millisThen))
      {
        Serial.println("System Error. Restart required");
      }
      #endif  

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

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Apogee detected. Proceeding to Descent state");
        }
        #endif           
      }
      else 
      {
        stateNext = Ascent;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Rocket is Ascending");
        }
        #endif           
      }
      
    //detectApogee = true;   
    stateIndex = stateNext;
    break;

    case Descent:
      // reset flag
      transitionEvent = false;

      if (thisBME->getAvgRecent() < 5)
      {
        stateNext = Landing;
        transition = Descent_to_Landing;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Critical Altitude achieved. Proceeding to Landing state");
        }
        #endif           
      }
      else
      {
        stateNext = Descent;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Rocket is Descending");
        }
        #endif             
      }
      
    //criticalAltitude = true;
    stateIndex = stateNext;
    break;
 
    case Landing:
      // reset flag
      transitionEvent = false;

      if (thisBME->getAvgRecent() > 2)
      {
        stateNext = Landing;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Rocket is drifting softly to the Earth");
        }
        #endif            
      }

      else
      {
        stateNext = Landing_Idle;
        transition = Landing_to_Landing_Idle;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Touchdown detected. Proceeding to Landing_Idle state");
        }
        #endif           
      }
    stateIndex = stateNext;
    break;

    case Landing_Idle:
      // reset flag
      transitionEvent = false;

        stateNext = Landing_Idle;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Eagle has Landed"); 
        }
        #endif         
  
    stateIndex = stateNext;
    break;

  } // switch
} // machine
