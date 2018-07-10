#ifndef MAIN_H
#define MAIN_H

#include <phys253.h>

class Main{
private:
    enum State{
        STARTUP,
        CRUISE,
        EWOK_SEARCH,
        EWOK_GRAB,
        DRAWBRIDGE,
        IR_WAIT,
        DUMP_PREP,
        DUMP_EWOKS,
        ZIP_HOOK,
        ZIP_UP,
        ZIP_UNHOOK,
        EWOK_4,
        BRIDGE_FOLLOW,
        ZIP_DOWN
    };

    State state_;

public:

    Main();
    void run();

};

#endif