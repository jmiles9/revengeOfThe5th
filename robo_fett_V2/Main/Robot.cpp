#include "Robot.h"

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
    irReady = false;
    ARM_RIGHT = RCServo2;
    CLAW_RIGHT = TINAH::Servo(RCSERVO7);
    ARM_LEFT = RCServo0;
    CLAW_LEFT = TINAH::Servo(RCSERVO6);
    BASKET = RCServo1;
}

//starts at start
//goes into cruise_plat1
void Robot::STARTUP() {
    Serial.println("in startup");
    Funcs::sweepServo(CLAW_LEFT, CLAW_CLOSED_LEFT, CLAW_OPEN_LEFT);
    Funcs::sweepServo(CLAW_RIGHT, CLAW_OPEN_RIGHT, CLAW_CLOSED_RIGHT);
    delay(1000);
    Funcs::sweepServo(ARM_LEFT, ARM_DOWN_CHEWIE_LEFT, ARM_UP_LEFT);
    Funcs::sweepServo(ARM_RIGHT, ARM_DOWN_CHEWIE_RIGHT, ARM_UP_RIGHT);



    runState = RunState::CRUISE_PLAT1;
}

// Starts at start
// end when near first ewok 
// goes into ewok search
void Robot::CRUISE_PLAT1() {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("CRUISE_PLAT1");
    int startTime = millis();
    int count = 0;
    tapeFollowForDistance(1200);
    runState = RunState::EWOK_SEARCH_RIGHT;
}

void Robot::EWOK_SEARCH_RIGHT() {
    Funcs::setMotorPower(100,100);
    int startLeftIndex = leftWheelIndex;
    int startRightIndex = rightWheelIndex;
    while(true) {
        tapeFollow(TF_KP1, TF_KD1, TF_KI1, TF_GAIN1, 80);
        if(ewokDetectRight()) {
            Funcs::hardStop();
            LCD.clear();LCD.home();
            LCD.setCursor(0,0); LCD.print("EWOK DETECTED");
            delay(2000);
            break;
        }
    }
    runState = RunState::EWOK_GRAB;
}

/// Grabs ewok.
void Robot::EWOK_GRAB() {
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print("EWOK_GRAB");

    TINAH::Servo arm;
    TINAH::Servo claw;
    int side;
    int stuffy;
    // Gets side and stuffy parameters
    switch(nextEwok) {
        case 1:
            side = RIGHT;
            stuffy = EWOK;
            runState = RunState::DRAWBRIDGE;
            break;
        case 2:
            side = RIGHT;
            stuffy = EWOK;
            runState = RunState::IR_WAIT;
            break;
        case 3: 
            side = LEFT;
            stuffy = EWOK;
            runState = RunState::DUMP_PREP;        
            break;
        case 4: 
            side = RIGHT;
            stuffy = EWOK;
            // TODO: Add state here
            break;
        case 5:
            side = LEFT;
            stuffy = configs::CHEWIE;
            //TODO: Add state here
            break;
    Funcs::pickUp(side, stuffy);
    nextEwok++;
    delay(100000);
    }
}

//starts after picking up first ewok
//ends when near second ewok
//goes into ewok_search
void Robot::DRAWBRIDGE() {
    //Adjust
    Funcs::move(150,100);
    delay(1000);
    //Turn towards gap, should be perpendicular
    Funcs::turn(-55);
    delay(1000);
    Funcs::setMotorPower(100,100);
    int currTime = millis();
    //Timeout after 3 seconds
    while(millis() - currTime < 3000) {
        if(Funcs::edgeDetect()) {
            break;
        }
    }
    hardStop();
    move(configs::PRE_BRIDGE_MOVE,50);
    delay(1000);
    Funcs::lowerBridge();
    delay(1000);
    move(configs::BRIDGE_REVERSE,100);
    delay(1000);
    move(configs::BRIDGE_CRUISE,100);
    delay(1000);
    Funcs::findTape();
    runState = RunState::EWOK_SEARCH_RIGHT;
}

// starts right after first ewok is picked up
//ends after right IR is detected
//enters cruise_plat_2
void Robot::IR_WAIT() {
    while(!irReady) {
        if(record10KIRBeacon() > record1KIRBeacon()) {
            irReady = true;
        }
    }
    while (record10KIRBeacon() < record1KIRBeacon()) {
    }
    runState = RunState::CRUISE_PLAT2;
}

//starts right after 10khz has been detected
//ends when ready to detect third ewok
//enters ewok_search
void Robot::CRUISE_PLAT2() {

    tapeFollowForDistance(PLAT2_CRUISE);
    runState = RunState::EWOK_SEARCH_LEFT;

}

void Robot::EWOK_SEARCH_LEFT() {
    ewokDetectLeft();
    runState = RunState::DUMP_PREP;
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
    EWOK_SEARCH_RIGHT();
    //may need to have something in here about edge detecting also, or should add to ewok search

}
// TODO: Write
void Robot::BRIDGE_FOLLOW() {

    turn(90);

    // this assumes when they are on tape they read HI = true
    //follows side of bridge until sees tape indicating zipline
    while(!(digitalRead(BRIDGE_QRD_LEFT) && digitalRead(BRIDGE_QRD_RIGHT))){
        bridgeFollow(TF_KP1,TF_KD1,TF_GAIN1);
    }

}
// TODO: WRite
void Robot::CHEWIE() {
    //will end on tape so can follow tape for a little bit longer
    EWOK_SEARCH_LEFT();
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
