//#include <cstdint>
#include "Funcs.h"

enum State {
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
    CHEWIE,
    ZIP_DOWN
};

namespace robot {

    class Robot : public Funcs {

        public:
            State state;

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
            void ZIP_HOOK();
            void ZIP_UP();
            void ZIP_UNHOOK();
            void EWOK_4();
            void BRIDGE_FOLLOW();
            void CHEWIE();
            void ZIP_DOWN();
 
    };
}