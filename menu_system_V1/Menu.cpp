//menu system just using switch/case.
#include <LiquidCrystal.h>

constexpr int BTN_START = -1; //CHANGE THIS
constexpr int BTN_STOP  = -1; //ALSO THIS

//number of menu options -- SHOULD BE NUMBER OF NAMES
constexpr int NUM_OPTIONS = 3;

//names for menu items 
constexpr int MENU_START = 0;
constexpr int MENU_COUNT = 1;
constexpr int MENU_NAMES = 2;


enum class State : int{
    S_CYCLE,
    S_COUNTING,
    S_NAMES
}

class Menu{
private:
    int menuOption;
    int state_;
    int counter;

    void display(){
        switch(state_){
            case S_CYCLE:
                switch(menuOption){
                    case MENU_START:
                        LCD.clear();  LCD.home() ;
                        LCD.setCursor(0,0); LCD.print("Hello!"); 
                        LCD.setCursor(0,1); LCD.print("Press START");
                    break;

                    case MENU_COUNT:
                        LCD.clear();  LCD.home() ;
                        LCD.setCursor(0,0); LCD.print("Count: STOP"); 
                        LCD.setCursor(0,1); LCD.print("Next: START");
                    break;

                    case MENU_NAMES:
                        LCD.clear();  LCD.home() ;
                        LCD.setCursor(0,0); LCD.print("Team: STOP"); 
                        LCD.setCursor(0,1); LCD.print("Next: START");
                    break;
                    
                    default: break;
                } 
            break;

            case S_COUNTING:
                LCD.clear();  LCD.home() ;
                LCD.setCursor(0,0); LCD.print(counter); 
                LCD.setCursor(0,1); LCD.print("count: START");
            break;

            case S_COUNTING:
                LCD.clear();  LCD.home() ;
                LCD.setCursor(0,0); LCD.print("5A: Joel, Miles"); 
                LCD.setCursor(0,1); LCD.print("5B: Brendan, Matan");
            break;

            default: break;
        }
    }

public:

    Menu{
        state_ = S_CYCLE;
        menuOption = 0;
        counter = 0;

        LCD.clear();  LCD.home() ;
        LCD.setCursor(0,0); LCD.print("Starting up"); 
        LCD.setCursor(0,1); LCD.print("Press START");
    }
    void Menu::handleInput(int input){
        switch(state_){
            case S_CYCLE:
                if(input == BTN_START){
                    //cycle through menu options
                    if(menuOption < NUM_OPTIONS - 1) menuOption ++;
                    else menuOption = 0; 
                }
                else if(input == BTN_STOP){
                    switch(menuOption){
                        case MENU_START: //if it is on the start menu, do nothing
                        break;
                        
                        case MENU_COUNT:
                            //switch to counting option (temporary)
                            state_ = S_COUNTING;
                            counter = 0;
                        break;

                        case MENU_NAMES:
                            //switch to names option (temporary)
                            state_ = S_NAMES;
                        break;
                    }
                }
            break;

            case S_COUNTING:
                //if the stop button is pressed, leave. start increments a counter (temp)
                if(input == BTN_START) counter ++;
                else if(input == BTN_STOP) state_ = S_CYCLE;
            break;

            case S_NAMES:
                //display team names. go back if stop pressed
                if(input == BTN_STOP) state_ = S_CYCLE;
            break;

            default: break;
        }

        display();
    }
};