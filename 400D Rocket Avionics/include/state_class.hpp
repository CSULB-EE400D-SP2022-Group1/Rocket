#pragma once
#include "myBME.h"
//comment check it out
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

        bool detectApogee ();
        int avgOne ();
        int avgThree ();

        void machine ();

    private:
        char stateIndex = stateNow;
        
        state_list stateNow;
        state_list stateNext;

        transition_list transition;

        bool sensorsGreen = false;        
        bool transitionEvent = false;

        myBME *thisBME;
};