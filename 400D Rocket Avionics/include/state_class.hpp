enum class state_list 
{
    POWER_ON,
    INIT,
    PAD_HOLD,
    PAD_IDLE,
    ASCENT,
    DESCENT,
    LANDING,
    LANDING_IDLE
};

enum class transition_list
{
    POWER_ON_to_INIT,
    INIT_to_PAD_HOLD,
    INIT_to_PAD_IDLE,
    PAD_IDLE_to_ASCENT,
    ASCENT_to_DESCENT,
    DESCENT_to_LANDING,
    LANDING_to_LANDING_IDLE
};

class State 
{
    public:
        void initializeMachine(int start);
        char getState();
        char getTransition();

        bool getStateChange();      
        State();
    private:
        using enum state_list;
            char stateNow;
            char stateNext;

        using enum transition_list;
            char transitionFlag;
     
};