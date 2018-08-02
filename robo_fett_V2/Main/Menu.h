#ifndef MENU_H
#define MENU_H

//set PD constants
//check tape sensor vals
//check IR?
//start/
//maybe reset stuff?
//check servo pos??

const int BTN_START = 1; 
const int BTN_STOP = 0; 

const int KNOB = 0; //analog port 0

//menuOption
const int MENU_START  = 0;
const int MENU_PDSET  = 1;
const int MENU_QRDSET = 2;
const int MENU_IR     = 3;
const int MENU_EXIT   = 4;
const int NUM_OPTIONS = 5; //keep this last!! is num of options

//pdOption
const int PD_KP   = 0;
const int PD_KD   = 1;
const int PD_GAIN = 2;
const int PD_OPTIONS = 3; //keep this last!! is num of options

//qrdPair (for qrdSet)
const int PAIR_OUT  = 0;
const int PAIR_IN   = 1;
const int PAIR_EDGE = 2;
const int PAIR_EXIT = 3;
const int NUM_PAIRS = 4;

class Menu{
private:
    enum State{
        S_CYCLE,
        S_PDSET,
        S_QRDSET,
        S_IRCHECK,
        S_EXIT
    };
    
    State state_;
    int menuOption_;
    int pdOption_;
    int qrdPair_;
    int counter;

    void display();

public:
    bool quitMenu;
    int kp;
    int kd;
    int gain;

    Menu();
    void handleInput(int input);
};

#endif
