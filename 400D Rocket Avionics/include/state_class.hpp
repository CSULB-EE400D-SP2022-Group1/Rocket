#pragma once
#include "myBME.h"


// CRITICAL VARIABLE DEFINITIONS

    const int DATA_COUNT_LIMIT = 5;                       // Counter limit in detecting Apogee to transition into 'Descent' state
    

                                            
    const int CRITICAL_ALTITUDE = 100;                    // Altiude (m) measurement to transition from 'Descent' state to 'Landing' state


    const int LANDING_IDLE_THRESHOLD = 100;               // Threshold alitude (m) to determine the rocket is in 'Landing Idle' state
    const int LANDING_IDLE_ELAPSED = 60000;               // Elapsed time (ms) to determine the rocket is in in 'Landing Idle' state 
    
    
    const int DEBUG_OUT_INTERVAL = 500;                   // Moterating time (ms) of debug statements for terminal

    const uint8_t REDLEDPIN = 7;
    const uint8_t GRNLEDPIN = 8;


enum state_list 
{
    Init,
    Pad_Hold,
    Pad_Idle,
    Ascent,
    Descent,
    Landing,
    Landing_Idle
};

enum transition_list
{
    Init_to_Pad_Hold,
    Init_to_Pad_Idle,
    Pad_Idle_to_Ascent,
    Ascent_to_Descent,
    Descent_to_Landing,
    Landing_to_Landing_Idle
};

class State 
{
    public:
        void initializeMachine(bool sensors, myBME *someBME);
        char getState();
        char getTransition();
        bool getTransitionEvent();

        bool debugTimer();

        void machine();

    private:
        // APOGEE
        bool detectApogee (uint32_t getDataCount);

        int dataCount = 0;                          // Index of the altidude data from the BME used for detecting apogee
        int indexNow = 0;                           // Indexing the current BME data 
        int indexThen = 0;                          // Indexing current BME data to past BME data
        int avgOne();                               
        int avgThree();

        // STATE
        char stateIndex = stateNow;
        state_list stateNow;
        state_list stateNext;

        transition_list transition;

        bool sensorsGreen = false;        
        bool transitionEvent = false;

        unsigned long millisNow;
        unsigned long millisThen;
        unsigned long landingTransitionEvent;

        myBME *thisBME;
};