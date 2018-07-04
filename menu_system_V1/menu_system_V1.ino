#include <Menu.cpp>
#include <phys253.h>

constexpr int BTN_STOP  = 0
constexpr int BTN_START = 1

void setup(){
    Menu menu;
}

void loop(){
    if(startbutton()) menu.handleInput(BTN_START);
    if(stopbutton()) menu.handleInput(BTN_STOP);
}