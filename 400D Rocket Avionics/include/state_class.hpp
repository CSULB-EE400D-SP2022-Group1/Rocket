#pragma once
#include "myBME.h"

// CRITICAL VARIABLE DEFINITIONS
    // detectApogee() criteria, transition to 'Descent' state
    const int DATA_COUNT_LIMIT = 5;
    
    // Transition to 'Landing' state
    const int CRITICAL_ALTITUDE = 100;

    // Transition to 'Landing_Idle' state
    const int LANDING_IDLE_THRESHOLD = 100;
    const int LANDING_IDLE_ELAPSED = 60000;
    
    // DEBUG
    const int DEBUG_OUT_INTERVAL = 500;


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

        bool debugTimer(unsigned long millisThen);

        void machine();

    private:
        // APOGEE
        bool detectApogee (uint32_t getDataCount);

        int dataCount = 0;
        int indexNow = 0;
        int indexThen = 0;
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