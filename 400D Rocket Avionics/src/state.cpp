#include "state_class.hpp"

#define DEBUG_STATE_MACHINE true

/*!
    @brief Initializes the state machine to 'Init' and provides pointer to myBME object
    @return void
*/
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

/*!
    @brief gets current state of state machine
    @return char stateNow
*/
char State::getState()
{
    return stateNow;
}

/*!
    @brief gets next state transition of state machine
    @return char stateNext
*/
char State::getTransition()
{
    return transition;
}

/*!
    @brief flags transition event when detected
    @return 1 (true) if state machine transition, 0 (false) otherwise
*/
bool State::getTransitionEvent()
{
  return transitionEvent;
}

/*!
    @brief calls myBME::getAvgRecent() for altitude
    @return int avgOne
*/
int State::avgOne ()
{
  return thisBME->getAvgRecent();
}

/*!
    @brief calls myBME::getAvgRecent() for altitude
    @return int avgThree
*/
int State::avgThree ()
{
  return thisBME->getAvg();
}

/*!
    @brief calls myBME::getDataCount() for data pull events, compares avgOne and avgThree for apogee across multiple pulls
    @return 1 (true) if apogee detected, 0 (false) otherwise
*/
bool State::detectApogee (uint32_t getDataCount)
{
  indexNow = getDataCount;

  if (dataCount == DATA_COUNT_LIMIT)
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

/*!
    @brief determines debug output interval
    @return 1 (true) if interval satisfied, 0 (false) otherwise
*/
bool State::debugTimer ()
{
  millisNow = millis();
  
  if (millisNow - millisThen >= DEBUG_OUT_INTERVAL)
  {
    millisThen = millisNow;
    return true;
  }
  return false;
}

/*!
    @brief calls State:machine()
    @return void
*/
void State::machine() 
{
  // state machine switch structure
  switch (stateIndex)
  {
    case Init:
      #if DEBUG_STATE_MACHINE
      if (debugTimer())
      {
        Serial.print("System startup. Init state");
        Serial.println(millis()/1000);
      }  
      #endif

      // 'Init' state transition to 'Pad_Idle' or 'Pad_Hold'
      if (sensorsGreen == true)
      {
        stateNext = Pad_Idle;
        transition = Init_to_Pad_Idle;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Sensors detected. Proceeding to Pad_Idle state");
          Serial.println(millis()/1000);
        }
        #endif
      }
      else
      {
        stateNext = Pad_Hold;
        transition = Init_to_Pad_Hold;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Sensors not detected. Proceeding to Pad_Hold state");
          Serial.println(millis()/1000);
        }
        #endif
      };
    
    stateIndex = stateNext;
    break;

    case Pad_Idle:
      // reset flag
      transitionEvent = false;

      // 'Pad_Idle' state transtion to 'Ascent'
      if (thisBME->detectLaunch() == true)
      {
        stateNext = Ascent;
        transition = Pad_Idle_to_Ascent;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
        Serial.print("Launch detected. Proceeding to Ascent state");
        Serial.println(millis()/1000);
        }
        #endif
      }
      else
      {
        stateNext = Pad_Idle;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
        Serial.print("Awaiting Launch. Standby in Pad_Idle state");
        Serial.println(millis()/1000);
        }
        #endif        
      }
      
    stateIndex = stateNext;
    break;

    case Pad_Hold:
      // reset flag
      transitionEvent = false;
      stateNext = Pad_Hold;

      #if DEBUG_STATE_MACHINE
      if (debugTimer())
      {
        Serial.print("System Error. Restart required");
        Serial.println(millis()/1000);
      }
      #endif  

    stateIndex = stateNext;
    break;

    case Ascent:
      // reset flag
      transitionEvent = false;

      // 'Ascent' state transtion to 'Descent'
      if (detectApogee(thisBME->getDataCount()) == true)
      {
        stateNext = Descent;
        transition = Ascent_to_Descent;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Apogee detected. Proceeding to Descent state");
          Serial.println(millis()/1000);
        }
        #endif           
      }
      else 
      {
        stateNext = Ascent;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Rocket is Ascending");
          Serial.println(millis()/1000);
        }
        #endif           
      }
      
    //detectApogee = true;   
    stateIndex = stateNext;
    break;

    case Descent:
      // reset flag
      transitionEvent = false;

      // 'Descent' state transtion to 'Landing'
      if (thisBME->getAvgRecent() < CRITICAL_ALTITUDE)
      {
        stateNext = Landing;
        transition = Descent_to_Landing;
        landingTransitionEvent = millis();
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Critical Altitude achieved. Proceeding to Landing state");
          Serial.println(millis()/1000);
        }
        #endif           
      }
      else
      {
        stateNext = Descent;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Rocket is Descending");
          Serial.println(millis()/1000);
        }
        #endif             
      }
      
    //criticalAltitude = true;
    stateIndex = stateNext;
    break;
 
    case Landing:
      // reset flag
      transitionEvent = false;

      // 'Landing' state transtion to 'Landing_Idle'
      if ((thisBME->getAvg() < LANDING_IDLE_THRESHOLD) && (millis() - landingTransitionEvent > LANDING_IDLE_ELAPSED))
      {
        stateNext = Landing_Idle;
        transition = Landing_to_Landing_Idle;
        transitionEvent = true;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Touchdown detected. Proceeding to Landing_Idle state");
          Serial.println(millis()/1000);
        }
        #endif            
      }

      else
      {
        stateNext = Landing;

        #if DEBUG_STATE_MACHINE
        if (debugTimer())
        {
          Serial.print("Rocket is drifting gently to Earth");
          Serial.println(millis()/1000);
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
        if (debugTimer())
        {
          Serial.print("Eagle has Landed");
          Serial.println(millis()/1000); 
        }
        #endif         
  
    stateIndex = stateNext;
    break;

  } // switch
} // machine
