#include "state_class.hpp"
/*
bool sensorsGreen = true;
bool detectLaunch = true;
bool detectApogee = true;
bool criticalAltitude = true;
bool touchdown = true;
*/
#define DEBUG_STATE_MACHINE false

void State::initializeMachine(bool sensors, myBME *someBME)
{
    if (sensors == true)
    {
        stateNow = static_cast<state_list>(0);
        transition = static_cast<transition_list>(0);
        sensorsGreen = true;
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

bool State::detectApogee (uint32_t getDataCount)
{
  indexNow = getDataCount;

  if (dataCount == 3)
  {
    return true;
  }  
  else if ((indexNow > indexThen) && (avgOne() < avgThree()))
  {
    dataCount++;
  }
  else
  {
    dataCount = 0;
  }

  indexThen = indexNow;     
  return false;
}


bool State::debugTimer (unsigned long millisThen)
{
  unsigned long millisNow = millis();
  
  if (millisNow - millisThen >= 250)
  {
    millisThen = millisNow;
    return true;
  }
  return false;
}


// the machine
void State::machine() 
{
  elapsedMillis();

  thisBME->getData();

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

      if (detectApogee(thisBME->getDataCount()) == true)
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

      if (thisBME->getAvgRecent() < 3)
      {
        stateNext = Landing;
        transition = Descent_to_Landing;
        landingTransitionEvent = millis();
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

      if ((thisBME->getAvg() < 2) && (millis() - landingTransitionEvent > 60000))
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

      else
      {
        stateNext = Landing;

        #if DEBUG_STATE_MACHINE
        if (debugTimer(millisThen))
        {
          Serial.println("Rocket is drifting gently to Earth");
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
