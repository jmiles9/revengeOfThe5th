#include "Main.h"
#include "TapeFollower.cpp"


Main::Main(){
    state_ = STARTUP;
    TapeFollower platform1(3,3,3);
}

void Main::run(){
    while(true){
        switch(state_){
            case STARTUP:
                break;
        
            case CRUISE:
                platform1.tapeFollow(stuff);
                break;

            case EWOK_SEARCH:
                //looking for an ewok. travel more slowly
                //tapefollow()
                //pollIR
        }

    }
}