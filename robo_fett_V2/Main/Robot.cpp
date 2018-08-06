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
}

//starts at start
//goes into cruise_plat1
void Robot::STARTUP() {
    Serial.println("in startup");
    Funcs::sweepServo(ARM_LEFT, ARM_UP_LEFT, ARM_DOWN_CHEWIE_LEFT);
    Funcs::sweepServo(ARM_RIGHT, ARM_UP_RIGHT, ARM_DOWN_CHEWIE_RIGHT);
    delay(1000);
    contractZipline();
    delay(500);
    extendZipline(3000);

    Funcs::sweepServo(CLAW_LEFT, CLAW_OPEN_LEFT, CLAW_CLOSED_LEFT);
    Funcs::sweepServo(CLAW_RIGHT, CLAW_OPEN_RIGHT, CLAW_CLOSED_RIGHT);
    delay(1000);
    Funcs::sweepServo(ARM_LEFT, ARM_DOWN_CHEWIE_LEFT, ARM_REST_LEFT);
    Funcs::sweepServo(ARM_RIGHT, ARM_DOWN_CHEWIE_RIGHT, ARM_REST_RIGHT);
    delay(1000);
    nextEwok = 3;

    runState = RunState::IR_WAIT;
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
    tapeFollowForDistance(1400);
    runState = RunState::EWOK_SEARCH_RIGHT;
}

void Robot::EWOK_SEARCH_RIGHT() {
    Funcs::setMotorPower(180,180);
    Funcs::sweepServo(ARM_RIGHT,ARM_UP_RIGHT,ARM_REST_RIGHT);
    Funcs::sweepServo(CLAW_RIGHT,CLAW_CLOSED_RIGHT,CLAW_OPEN_RIGHT);
    int startLeftIndex = leftWheelIndex;
    int startRightIndex = rightWheelIndex;
    while(true) {
        tapeFollow(TF_KP1, TF_KD1, TF_KI1, TF_GAIN1, 120);
        if(ewokDetectRight()) {
            Serial.println("detected");
            Funcs::hardStop();
            LCD.clear();LCD.home();
            LCD.setCursor(0,0); LCD.print("EWOK DETECTED");
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

    //move forward here?

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
    }
    int sensorVal = checkEwokSensor(side);
    int time = millis();
    if(sensorVal < 300) {
        if(side == LEFT) {
            LCD.clear(); LCD.setCursor(0,0);
            LCD.print(sensorVal);
            setMotorPower(-70,90);
            while(sensorVal < 300 && millis() - time < 150) {}
            setMotorPower(0,0);
        } else {
            LCD.clear(); LCD.setCursor(0,0);
            LCD.print(sensorVal);
            setMotorPower(90,-70);
            while(sensorVal < 300 && millis() - time < 150) {}
            setMotorPower(0,0);
        }
    } else if(sensorVal > 300) {
            if(side == RIGHT) {
            LCD.clear(); LCD.setCursor(0,0);
            LCD.print(sensorVal);
            setMotorPower(-100,-50);
            while(sensorVal > 300 && millis() - time < 150) {}
            setMotorPower(0,0);
        } else {
            LCD.clear(); LCD.setCursor(0,0);
            LCD.print(sensorVal);
            setMotorPower(-50,-100);
            while(sensorVal > 300 && millis() - time < 300) {}
            setMotorPower(0,0);
        }
    }
    Funcs::pickUp(side, stuffy);
    nextEwok++;
    delay(1000);
}

//starts after picking up first ewok
//ends when near second ewok
//goes into ewok_search
void Robot::DRAWBRIDGE() {
    //Adjust
    Funcs::move(180,150);
    LCD.setCursor(0,0);
    LCD.print("move");
    //Turn towards gap, should be perpendicular
    Funcs::turn(-70);
    Funcs::setMotorPower(120,120);
    int currTime = millis();
    //Timeout after 3 seconds
    while(millis() - currTime < 3000) {
        if(Funcs::edgeDetect()) { 
            break;
        }
    }
    hardStop();
    centerOffEdge();
    delay(1000);
    Funcs::lowerBridge();
    delay(500);
    move(100,-150);
    delay(200);
    move(450,170);
    delay(1500);
    Funcs::findTape();
    runState = RunState::EWOK_SEARCH_RIGHT;
}

// starts right after first ewok is picked up
//ends after right IR is detected
//enters cruise_plat_2
void Robot::IR_WAIT() {
    delay(5000);
    sweepServo(ARM_LEFT,ARM_DOWN_EWOK_LEFT,ARM_ARCH_LEFT);
    sweepServo(ARM_RIGHT,ARM_DOWN_EWOK_RIGHT,ARM_ARCH_RIGHT);
    delay(1000);
    move(120,120);
    rotateUntilTapeCCW();
    // while(!irReady) {
    //     if(record10KIRBeacon() > record1KIRBeacon()) {
    //         irReady = true;
    //     }
    // }
    // while (record10KIRBeacon() < record1KIRBeacon()) {
    // }
    runState = RunState::CRUISE_PLAT2;
}

//starts right after 10khz has been detected
//ends when ready to detect third ewok
//enters ewok_search
void Robot::CRUISE_PLAT2() {
    
    tapeFollowForDistance(1500);
    runState = RunState::EWOK_SEARCH_LEFT;
}

void Robot::EWOK_SEARCH_LEFT() {
    Funcs::setMotorPower(180,180);
    Funcs::sweepServo(ARM_RIGHT,ARM_UP_RIGHT,ARM_REST_RIGHT);
    Funcs::sweepServo(ARM_LEFT,ARM_UP_LEFT,ARM_REST_LEFT);
    Funcs::sweepServo(CLAW_LEFT,CLAW_CLOSED_LEFT,CLAW_OPEN_LEFT);
    while(true) {
        if(edgeDetect()) {
            hardStop();
            setMotorPower(-80,-90);
            delay(2000);
            setMotorPower(0,0);
            break;
        }
        tapeFollow(TF_KP1, TF_KD1, TF_KI1, TF_GAIN1, 120);
        if(ewokDetectLeft()) {
            Funcs::hardStop();
            delay(400);
            LCD.clear();LCD.home();
            LCD.setCursor(0,0); LCD.print("EWOK DETECTED");
            break;
        }
    }
    runState = RunState::EWOK_GRAB;
}

//starts after 3rd ewok is grabbed
//ends when aligned and has front right at wall 
//goes into dump_ewoks
void Robot::DUMP_PREP() {
    delay(1000);
    turn(-30);
    delay(1000);
    move(100,-130);
    delay(1000);
    turn(-90);
    delay(1000);
    move(100,100);
    //can either TapeFollowForDistance or put contact sensor on front? 
    runState = RunState::DUMP_EWOKS;

}

//starts after robot is aligned and has front right at wall
//ends after robots are dumped
//goes into find_zip_Plat2
void Robot::DUMP_EWOKS() {
    Serial.println("dump");
    dumpBasket();
    delay(5000);
    Serial.println("here we go");
    runState = RunState::FIND_ZIP_PLAT2;
}


//starts after dump is finished
//ends when located under zipline on the 2nd platform
//enters zip_hook
void Robot::FIND_ZIP_PLAT2(){
    move(200,-140);
    delay(1000);
    turn(90);
    delay(1000000);
}

// starts when ewoks are dumped
//ends when attached to zipline and ready to go 
void Robot::ZIP_HOOK() {
    //need to find zipline, hook up
    //assume is started from already being on tape directly under line
    //likely need to do something other than turning
    extendZipline();
    
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
