#include <stdio.h>
#include <iostream>
using namespace std;

int state_machine{  //state_machine to be called from main?
/*
Here we will structure the state machine that will dictate the order of operations for the rocket
Yet to do:

[]  Signals required to trigger states
[]  Internal conditions
[]  Outbound signalling
[]  Error handling

*/
    enum state{
        Iniitialize, Pad_idle, Pad_hold, Ascent, Descent, Landing, Landing_idle //preliminary list of states, subject to change
    }


}