#pragma once
#include "myBME.cpp"

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
/*
  int (myBME::*ptrAvg)() = &myBME::getAvg;
  int (myBME::*ptrAvgRecent)() = &myBME::getAvgRecent;
    
  bool detectLaunch = &myBME::detectLaunch;
*/
class State 
{
    public:
        void initializeMachine(bool sensors);
        char getState();
        char getTransition();
        bool getTransitionEvent();

        char stateIndex = stateNow;

        bool apogee();
        void machine();
    private:
        state_list stateNow;
        state_list stateNext;

        transition_list transition;

        bool sensorsGreen = false;
        
        bool transitionEvent = false;
};