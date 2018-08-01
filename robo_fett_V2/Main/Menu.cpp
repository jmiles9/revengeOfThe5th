//menu system just using switch/case.
#include <LiquidCrystal.h>
#include "Menu.h"
#include <phys253.h>


void Menu::display()
{
    switch (state_){
    case S_CYCLE:
        Serial.println("display - S_CYCLE");
        switch (menuOption_){
        case MENU_START:
            Serial.println("display - MENU_START");
            LCD.clear(); LCD.home();
            LCD.setCursor(0, 0); LCD.print("I am ROBO FETT");
            LCD.setCursor(0, 1); LCD.print("Press START :)");
            break;

        case MENU_PDSET: 
            Serial.println("display - MENU_PDSET");
            LCD.clear();LCD.home();
            LCD.setCursor(0, 0); LCD.print("set PD: <STOP>");
            LCD.setCursor(0, 1); LCD.print("Next: <START>");
            break;

        case MENU_QRDSET: 
            Serial.println("display - MENU_QRDSET");
            LCD.clear(); LCD.home();
            LCD.setCursor(0, 0); LCD.print("Set QRD: <STOP>");
            LCD.setCursor(0, 1); LCD.print("Next: <START>");
            break;

        case MENU_IR:
            LCD.clear(); LCD.home();
            LCD.setCursor(0, 0); LCD.print("Check IR: <STOP>");
            LCD.setCursor(0, 1); LCD.print("Next: <START>");
            break;

        case MENU_EXIT:
            LCD.clear(); LCD.home();
            LCD.setCursor(0, 0); LCD.print("Start course: <STOP>");
            LCD.setCursor(0, 1); LCD.print("Next: <START>");
            break;

        default: break;
        }
        break;

    case S_PDSET: 
        Serial.println("display - S_PDSET");
        switch(pdOption_){
            case PD_KP:
                kp = analogRead(KNOB)/200;
                LCD.setCursor(0, 0); LCD.print("KP = "); LCD.print(kp);
                LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
                break;

            case PD_KD:
                kd = analogRead(KNOB)/200;
                LCD.setCursor(0, 0); LCD.print("KD = "); LCD.print(kd);
                LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
                break;

            case PD_GAIN:
                gain = analogRead(KNOB)/200;
                LCD.setCursor(0, 0); LCD.print("GAIN = "); LCD.print(kp);
                LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
                break;
            
            default: break;
        }
        break;

    //TODO: set stuff
    case S_QRDSET:
        //display both, and threshold (can be set using stop)
        switch(qrdPair_){
            case PAIR_OUT:
                LCD.clear(); LCD.home();
                LCD.setCursor(0, 0); 
                LCD.print("L = "); LCD.print(":P");
                LCD.print(" R = "); LCD.print(":P");
                LCD.setCursor(0, 1);
                LCD.print("THRESH = "); LCD.print(":P");
                break;
            
            case PAIR_IN:
                LCD.clear(); LCD.home();
                LCD.setCursor(0, 0); 
                LCD.print("ML = "); LCD.print(":P");
                LCD.print(" MR = "); LCD.print(":P");
                LCD.setCursor(0, 1);
                LCD.print("THRESH = "); LCD.print(":P");
                break;

            case PAIR_EDGE:
                LCD.clear(); LCD.home();
                LCD.setCursor(0, 0); 
                LCD.print("IN = "); LCD.print(":P");
                LCD.print(" OUT = "); LCD.print(":P");
                LCD.setCursor(0, 1);
                LCD.print("THRESH = "); LCD.print(":P");
                break;

            case PAIR_EXIT:
                LCD.clear(); LCD.home();
                LCD.setCursor(0, 0); 
                LCD.print("back: <STOP> ");
                LCD.setCursor(0, 1);
                break;

            default: break;
        }
        break;

    case S_IRCHECK:
        Serial.println("display - S_IR");
        LCD.clear(); LCD.home();
        LCD.setCursor(0, 0); 
        LCD.print("IR = "); LCD.print(":P");
        LCD.setCursor(0, 1); LCD.print("<STOP> to go back");
        break;

    case S_EXIT:
        Serial.println("display - S_EXIT");
        LCD.clear(); LCD.home();
        LCD.setCursor(0, 0); LCD.print("<START> to GO!!!");
        LCD.setCursor(0, 1); LCD.print("<STOP> to go back");
        break;

    default: break;
    }
}

Menu::Menu(){
    state_ = S_CYCLE;
    menuOption_ = 0;
    pdOption_ = 0;
    counter = 0;
    quitMenu = false;

    LCD.clear(); LCD.home();
    LCD.setCursor(0, 0); LCD.print("Starting up");
    LCD.setCursor(0, 1); LCD.print("Press START");
}

void Menu::handleInput(int input){
    switch (state_){
    case S_CYCLE:
        if (input == BTN_START)
            (menuOption_ < NUM_OPTIONS - 1) ? menuOption_++ : menuOption_ = 0;
        else if (input == BTN_STOP){
            switch (menuOption_){
            case MENU_START: //if it is on the start menu, do nothing
                break;

            case MENU_PDSET:
                //switch to PD set menu
                state_ = S_PDSET;
                break;

            case MENU_QRDSET:
                //switch to QRD value view
                state_ = S_QRDSET;
                break;

            case MENU_IR:
                //switch to QRD value view
                state_ = S_IRCHECK;
                break;

            case MENU_EXIT:
                //switch to QRD value view
                state_ = S_EXIT;
                break;

            default: break;
            }
        }
        break;

    case S_PDSET:
        //start to cycle through kp, kd, gain
        //stop to go back
        if (input == BTN_START) 
            (pdOption_ < PD_OPTIONS - 1) ? pdOption_++ : pdOption_ = 0;
        else if (input == BTN_STOP) state_ = S_CYCLE;
        break;

    case S_QRDSET:
        //three sets of qrds
        if (input == BTN_START)
            (qrdPair_ < NUM_PAIRS - 1) ? qrdPair_ ++ : qrdPair_ = 0;
        else if (input == BTN_STOP){
            switch(qrdPair_){
                case PAIR_OUT:
                    //TODO: set qrd threshold
                    break;

                case PAIR_IN:
                    //TODO: set
                    break;
                
                case PAIR_EDGE:
                    //TODO: set
                    break;

                case PAIR_EXIT:
                    state_ = S_CYCLE;
                    break;
                
                default: break;
            }
        }
        break;

    case S_IRCHECK:
        if (input == BTN_STOP) state_ = S_CYCLE;
        break;

    case S_EXIT:
        if (input == BTN_START) quitMenu = true;
        else if (input == BTN_STOP) state_ = S_CYCLE;
        break;

    default: break;
    }

    display();
}
