//#include <cstdint>
#include "Funcs.h"
#include "Menu.h"

enum RunState {
    STARTUP,
    CRUISE_PLAT1,
    EWOK_SEARCH_RIGHT,
    EWOK_GRAB,
    DRAWBRIDGE,
    IR_WAIT,
    CRUISE_PLAT2,
    EWOK_SEARCH_LEFT,
    PICKUP_THIRD,
    DUMP_PREP,
    DUMP_EWOKS,
    FIND_ZIP_PLAT2,
    ZIP_HOOK,
    ZIP_UP,
    ZIP_UNHOOK,
    EWOK_4,
    BRIDGE_FOLLOW,
    SAVE_CHEWIE,
    ZIP_DOWN
};

class Robot : public Funcs {

    public:
        RunState runState;
        // Menu menu;

        Robot();
        void STARTUP();
        void CRUISE_PLAT1();
        void DRAWBRIDGE();
        void EWOK_SEARCH_RIGHT();
        void EWOK_GRAB();
        void CRUISE_IR();
        void IR_WAIT();
        void CRUISE_PLAT2();
        void EWOK_SEARCH_LEFT();
        void PICKUP_THIRD();
        void DUMP_PREP();
        void DUMP_EWOKS();
        void FIND_ZIP_PLAT2();
        void ZIP_HOOK();
        void ZIP_UP();
        void ZIP_UNHOOK();
        void EWOK_4();
        void BRIDGE_FOLLOW();
        void SAVE_CHEWIE();
        void ZIP_DOWN();
        void IRHANDLE();

};
