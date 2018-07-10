#ifndef MENU_H
#define MENU_H

class Menu{
private:
    enum State{
        S_CYCLE,
        S_COUNTING,
        S_NAMES
    };
    int menuOption;
    State state_;
    int counter;

    void display();

public:

    Menu();
    void handleInput(int input);
};

#endif