#include "Menu.h"
#include <phys253.h>

#define BTN_STOP 0
#define BTN_START 1

Menu menu;

void setup(){
    Serial.begin(9600);
    Serial.println("hello there. team 5 is the best");
    LCD.begin(16,2);
}

void loop(){
   bool start = false;
   bool stopp = false;
    while(startbutton()){start = true;}
    if(start){
      menu.handleInput(BTN_START);
      Serial.println("START pressed");
    }
    while(stopbutton()){stopp = true;}
    if(stopp){
      menu.handleInput(BTN_STOP);
      Serial.println("STOP pressed");
    }
}
