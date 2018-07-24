// //menu system just using switch/case.
// #include <LiquidCrystal.h>
// #include "Menu.h"
// #include <phys253.h>


// void Menu::display()
// {
//     switch (state_)
//     {
//     case S_CYCLE:
//         Serial.println("display - S_CYCLE");
//         switch (menuOption)
//         {
//         case MENU_START:
//             Serial.println("display - MENU_START");
//             LCD.clear(); LCD.home();
//             LCD.setCursor(0, 0); LCD.print("I am ROBO FETT");
//             LCD.setCursor(0, 1); LCD.print("Press START :)");
//             break;

//         case MENU_PDSET: 
//             Serial.println("display - MENU_PDSET");
//             LCD.clear();LCD.home();
//             LCD.setCursor(0, 0); LCD.print("set PD: <STOP>");
//             LCD.setCursor(0, 1); LCD.print("Next: <START>");
//             break;

//         case MENU_NAMES: //TODO: CHANGE THIS
//             Serial.println("display - MENU_NAMES");
//             LCD.clear(); LCD.home();
//             LCD.setCursor(0, 0); LCD.print("Team: STOP");
//             LCD.setCursor(0, 1); LCD.print("Next: START");
//             break;

//         default:
//             break;
//         }
//         break;

//     case S_PDSET: 
//         Serial.println("display - S_PDSET");
//         switch(pdOption){
//             case PD_KP:
//                 kp = analogRead(KNOB)/200;
//                 LCD.setCursor(0, 0); LCD.print("KP = "); LCD.print(kp)
//                 LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
//                 break;

//             case PD_KD:
//                 kd = analogRead(KNOB)/200;
//                 LCD.setCursor(0, 0); LCD.print("KD = "); LCD.print(kd)
//                 LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
//                 break;

//             case PD_GAIN:
//                 gain = analogRead(KNOB)/200;
//                 LCD.setCursor(0, 0); LCD.print("GAIN = "); LCD.print(kp)
//                 LCD.setCursor(0, 1); LCD.print("Use KNOB to set");
//                 break;
            
//             default: break;
//         }
//         break;

//     case S_QRDCHECK:
//         Serial.println("display - S_QRD");
//         LCD.clear(); LCD.home();
//         LCD.setCursor(0, 0); 
//         LCD.print("left = "); LCD.print(":P");
//         LCD.print("right = "); LCD.print(":P");
//         LCD.setCursor(0, 1); 
//         LCD.print("edge = "); LCD.print(":P");
//         break;

//     case S_IRCHECK:
//         Serial.println("display - S_IR");
//         LCD.clear(); LCD.home();
//         LCD.setCursor(0, 0); 
//         LCD.print("IR"); LCD.print(":P");
//         LCD.setCursor(0, 1); LCD.print("<STOP> to go back");
//         break;

//     case S_EXIT:
//         Serial.println("display - S_EXIT");
//         LCD.clear(); LCD.home();
//         LCD.setCursor(0, 0); LCD.print("<START> to GO!!!");
//         LCD.setCursor(0, 1); LCD.print("<STOP> to go back");
//         break;

//     default: break;
//     }
// }

// Menu::Menu(){
//     state_ = S_CYCLE;
//     menuOption = 0;
//     pdOption = 0;
//     counter = 0;
//     quitMenu = false;

//     LCD.clear(); LCD.home();
//     LCD.setCursor(0, 0); LCD.print("Starting up");
//     LCD.setCursor(0, 1); LCD.print("Press START");
// }

// void Menu::handleInput(int input){
//     switch (state_){
//     case S_CYCLE:
//         if (input == BTN_START)
//             (menuOption < NUM_OPTIONS - 1) ? menuOption++ : menuOption = 0;
//         else if (input == BTN_STOP){
//             switch (menuOption){
//             case MENU_START: //if it is on the start menu, do nothing
//                 break;

//             case MENU_PDSET:
//                 //switch to PD set menu
//                 state_ = S_PDSET;
//                 break;

//             case MENU_QRDS:
//                 //switch to QRD value view
//                 state_ = S_QRDCHECK;
//                 break;

//             case MENU_IR:
//                 //switch to QRD value view
//                 state_ = S_IRCHECK;
//                 break;

//             case MENU_EXIT:
//                 //switch to QRD value view
//                 state_ = S_EXIT;
//                 break;

//             default: break;
//             }
//         }
//         break;

//     case S_PDSET:
//         //start to cycle through kp, kd, gain
//         //stop to go back
//         if (input == BTN_START) 
//             (pdOption < PD_OPTIONS - 1) ? pdOption++ : pdOption = 0;
//         else if (input == BTN_STOP) state_ = S_CYCLE;
//         break;

//     case S_QRDCHECK:
//         if (input == BTN_STOP) state_ = S_CYCLE;
//         break;

//     case S_IRCHECK:
//         if (input == BTN_STOP) state_ = S_CYCLE;
//         break;

//     case S_EXIT:
//         if (input == BTN_START) quitMenu = true;
//         else if (input == BTN_STOP) state_ = S_CYCLE;
//         break;

//     default: break;
//     }

//     display();
// }
