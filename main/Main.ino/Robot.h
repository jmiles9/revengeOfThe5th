//#include <cstdint>
#include "Funcs.h"

enum RunState {
    STARTUP,
    CRUISE_PLAT1,
    EWOK_SEARCH,
    EWOK_GRAB,
    DRAWBRIDGE,
    IR_WAIT,
    CRUISE_PLAT2,
    DUMP_PREP,
    DUMP_EWOKS,
    FIND_ZIP_PLAT2,
    ZIP_HOOK,
    ZIP_UP,
    ZIP_UNHOOK,
    EWOK_4,
    BRIDGE_FOLLOW,
    CHEWIE,
    ZIP_DOWN
};

class Robot : public Funcs {

    public:
        RunState runState;

        Robot();
        void STARTUP();
        void CRUISE_PLAT1();
        void DRAWBRIDGE();
        void EWOK_SEARCH();
        void EWOK_GRAB();
        void CRUISE_IR();
        void IR_WAIT();
        void CRUISE_PLAT2();
        void DUMP_PREP();
        void DUMP_EWOKS();
        void FIND_ZIP_PLAT2();
        void ZIP_HOOK();
        void ZIP_UP();
        void ZIP_UNHOOK();
        void EWOK_4();
        void BRIDGE_FOLLOW();
        void CHEWIE();
        void ZIP_DOWN();
        void IRHANDLE();

};
