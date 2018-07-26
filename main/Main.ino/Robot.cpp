#include "Robot.h"
#include "Menu.h"

using namespace configs;

// Constructor
Robot::Robot() {
    runState = RunState::STARTUP;
    nextEwok = 1;
    leftWheelIndex = 0;
    rightWheelIndex = 0;
    leftWheelLastTime = 0;
    rightWheelLastTime = 0;
    leftSpeed = 0;
    rightSpeed = 0;
    error = 0;
}

//starts at start
//goes into cruise_plat1
void Robot::STARTUP() {
    Serial.println("in startup");
    // while(!menu.quitMenu){
    //     bool start = false;
    //     bool stopp = false;

    //     while(startbutton()){start = true;}
    //     if(start) menu.handleInput(BTN_START);

    //     while(stopbutton()){stopp = true;}
    //     if(stopp) menu.handleInput(BTN_STOP);
    // }
    CLAW_RIGHT.write(CLAW_TONG_CLOSED_POSITION);
    ARM_RIGHT.write(CLAW_ARM_RAISED_POSITION);
    runState = RunState::CRUISE_PLAT1;
}

// Starts at start
//end when near first ewok 
//goes into ewok search
void Robot::CRUISE_PLAT1() {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("CRUISE_PLAT1");
    tapeFollowForDistance(PLAT1_CRUISE);
    runState = RunState::EWOK_SEARCH;
}

void Robot::EWOK_SEARCH() {

    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("EWOK_SEARCH");

    int i = 0;
  
    while(true) {
        tapeFollow(TF_KP1, TF_KD1, TF_GAIN1, SPEED);
        if(ewokDetect()) {
            setMotorPower(0,0);
            runState = RunState::EWOK_GRAB;
            break;
        }
    }
}

/// Grabs ewok.
/// If fails, attemps twice more, once a little in front, once behind.
void Robot::EWOK_GRAB() {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("EWOK_GRAB");
    
    int side;
    int stuffy;
    // Gets side and stuffy parameters
    if(nextEwok == 1 ) {
        side = LEFT;
        stuffy = configs::EWOK;
        runState = RunState::DRAWBRIDGE;
    } else if(nextEwok == 2) {
        side = RIGHT;
        stuffy = configs::EWOK;
        runState = RunState::IR_WAIT;
    } else if(nextEwok == 3) {
        side = LEFT;
        stuffy = configs::EWOK;
        runState = RunState::DUMP_PREP;        
    } else {
        side = RIGHT;
        stuffy = configs::EWOK;
    }

    

    // If don't get ewok, try again on either side
    int count = 1;
    while(count <= 3) {
        if(pickUp(side, stuffy)) {
            break;
        }
        if(count == 1) {
            tapeFollowForDistance(STUFFY_GRAB_MANEUVER);
        } else {
            tapeFollowForDistance(-2*STUFFY_GRAB_MANEUVER);
        }
        count++;
    }
    // Update ewok count
    nextEwok++;
}

//starts after picking up first ewok
//ends when near second ewok
//goes into ewok_search
void Robot::DRAWBRIDGE() {

    move(PRE_BRIDGE_MOVE);
    turn(PRE_BRIDGE_TURN);
   
    tapeFollowToEdge();
    
    lowerBridge();
    tapeFollowForDistance(BRIDGE_REVERSE);
    tapeFollowForDistance(BRIDGE_CRUISE);
    runState = RunState::EWOK_SEARCH;

}

// starts right after first ewok is picked up
//ends after right IR is detected
//enters cruise_plat_2
void Robot::IR_WAIT() {

    while (record10KIRBeacon() < record1KIRBeacon()) {
    }
    runState = RunState::CRUISE_PLAT2;
}

//starts right after 10khz has been detected
//ends when ready to detect third ewok
//enters ewok_search
void Robot::CRUISE_PLAT2() {

    tapeFollowForDistance(PLAT2_CRUISE);
    runState = RunState::EWOK_SEARCH;

}

//starts after 3rd ewok is grabbed
//ends when aligned and has front right at wall
//goes into dump_ewoks
void Robot::DUMP_PREP() {
    turn(-TURN_90);
    //can either TapeFollowForDistance or put contact sensor on front? 
    tapeFollowForDistance(DUMP_PREP_DIST);
    runState = RunState::DUMP_EWOKS;

}

//starts after robot is aligned and has front right at wall
//ends after robots are dumped
//goes into find_zip_Plat2
void Robot::DUMP_EWOKS() {
    dumpBasket();
    runState = RunState::ZIP_HOOK;
}

//ALL STATES ABOVE THIS READY TO RUN

//starts after dump is finished
//ends when located under zipline on the 2nd platform
//enters zip_hook
void Robot::FIND_ZIP_PLAT2(){
    //need to move around
}

// starts when ewoks are dumped
//ends when attached to zipline and ready to go 
void Robot::ZIP_HOOK() {
    //need to find zipline, hook up
    //assume is started from already being on tape directly under line
    //likely need to do something other than turning
    turn(ZIPLINE_ATTACH_ROTATION);
    extendZipline();
    turn(-ZIPLINE_ATTACH_ROTATION);
    contractZipline();

    if (nextEwok ==  4){
        //going up to 3rd platform
    }else{
        //going down to safe zone :')
    }


}
//TODO: WRite
void Robot::ZIP_UP() {
    zipUp();
    //zipUp function checks for contact switch and has a time limit
}
// TODO: Write
void Robot::ZIP_UNHOOK() {
    extendZipline();
    turn(ZIPLINE_ATTACH_ROTATION);
    contractZipline();
    turn(-ZIPLINE_ATTACH_ROTATION);
}
// TODO: Write
void Robot::EWOK_4() {
    //may need to reverse a bit first
    EWOK_SEARCH();
    //may need to have something in here about edge detecting also, or should add to ewok search

}
// TODO: Write
void Robot::BRIDGE_FOLLOW() {

    turn(90);

    // this assumes when they are on tape they read HI = true
    //follows side of bridge until sees tape indicating zipline
    while(!(digitalRead(TAPE_QRD_LEFT) && digitalRead(TAPE_QRD_RIGHT))){
        bridgeFollow(TF_KP1,TF_KD1,TF_GAIN1);
    }

}
// TODO: WRite
void Robot::CHEWIE() {
    //will end on tape so can follow tape for a little bit longer
    EWOK_SEARCH();
}
// TODO: Write
void Robot::ZIP_DOWN() {
    turn(190);
    //turn around 180 degrees so goes down with sensor going outwards
    extendZipline();
    turn(-10);
    contractZipline();
    zipUp();
}
