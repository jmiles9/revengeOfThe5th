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

//number of menu options -- SHOULD BE NUMBER OF NAMES
const int NUM_OPTIONS = 5;
const int PD_OPTIONS = 3; //kp, kd, gain

class Menu{
private:
    enum State{
        S_CYCLE,
        S_PDSET,
        S_QRDCHECK,
        S_IRCHECK,
        S_EXIT
    };

    //names for menu items
    enum menuOption{
        MENU_START = 0,
        MENU_PDSET = 1,
        MENU_QRDS  = 2,
        MENU_IR    = 3,
        MENU_EXIT  = 4
    };

    //names for the PD set menu options
    enum pdOption{
        PD_KP = 0,
        PD_KD = 1,
        PD_GAIN = 2
    };

    State state_;
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