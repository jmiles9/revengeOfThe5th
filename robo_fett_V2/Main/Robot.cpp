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
    Serial.println("contract");
    contractZipline();
    delay(500);
    Serial.println("extend");
    extendZipline(3750);

    Funcs::sweepServo(CLAW_LEFT, CLAW_OPEN_LEFT, CLAW_CLOSED_LEFT);
    Funcs::sweepServo(CLAW_RIGHT, CLAW_OPEN_RIGHT, CLAW_CLOSED_RIGHT);
    delay(1000);
    Funcs::sweepServo(ARM_LEFT, ARM_DOWN_CHEWIE_LEFT, ARM_REST_LEFT);
    Funcs::sweepServo(ARM_RIGHT, ARM_DOWN_CHEWIE_RIGHT, ARM_REST_RIGHT);
    delay(1000);

    nextEwok = 1;

    LCD.clear();
    LCD.print("FUCK");
  
    while(!(startbutton())){delay(100);}

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
    sweepServo(ARM_LEFT,ARM_DOWN_EWOK_LEFT,ARM_ARCH_LEFT);
    sweepServo(ARM_RIGHT,ARM_DOWN_EWOK_RIGHT,ARM_ARCH_RIGHT);
    delay(1000);
    contractZipline(1500);
    delay(5000);
    // while(!irReady) {
    //     if(record1KIRBeacon() > record10KIRBeacon()) {
    //         irReady = true;
    //     }
    //     int k = analogRead(IR_1KHZ);
    //     int kk = analogRead(IR_10KHZ);
    //     LCD.clear(); LCD.setCursor(0,0);
    //     LCD.print("1kz: "); LCD.print(k);
    //     LCD.setCursor(0,1); LCD.print("10kz: "); LCD.print(kk);
    //     delay(250);
    // }
    // while (record10KIRBeacon() < record1KIRBeacon()) {
    //     int k = analogRead(IR_1KHZ);
    //     int kk = analogRead(IR_10KHZ);
    //     LCD.clear(); LCD.setCursor(0,0);
    //     LCD.print("1k: "); LCD.print(k);
    //     LCD.setCursor(0,1); LCD.print("10k: "); LCD.print(kk);
    //     delay(250);
    // }
    Serial.println("done IR");
    move(120,120);
    rotateUntilTapeCCW();
    Serial.println("cruise set");
    runState = RunState::CRUISE_PLAT2;
}

//starts right after 10khz has been detected
//ends when ready to detect third ewok
//enters ewok_search
void Robot::CRUISE_PLAT2() {
    
    tapeFollowForDistance(1450);
    runState = RunState::EWOK_SEARCH_LEFT;
}

void Robot::EWOK_SEARCH_LEFT() {
    Funcs::extendZipline(1500);
    delay(5000);
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
    turn(-25);
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
    delay(1000);
    runState = RunState::ZIP_HOOK;
}

//MIGHT NOT NEED THIS STATE CUZ CAN USE centreOnZipline()
// starts when ewoks are dumped
//ends when attached to zipline and ready to go 
void Robot::ZIP_HOOK() {
    //need to find zipline, hook up
    //assume is started from already being on tape directly under line
    //likely need to do something other than turning
    extendZipline();
    delay(10000000000);
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
    runState = RunState::ZIP_UNHOOK;
}

// TODO: Write
void Robot::ZIP_UNHOOK() {
    extendZipline();
    move(50, -100);
    contractZipline();
    move(50, 100); //these values may not be enough 
}
// TODO: Write
void Robot::EWOK_4() {
    //may need to add more movements before searching
    EWOK_SEARCH_LEFT();
    EWOK_GRAB(); //maybe should just use as states, idgaf
    turn(5);
    move(100, 100); //need to get past the circular platforn
    centreOnBridgeEdge();

    runState = RunState::BRIDGE_FOLLOW;
    //may need to have something in here about edge detecting also, or should add to ewok search

}
// Starts when bot is on edge
//ends when chewie is detected 
void Robot::BRIDGE_FOLLOW() {

    LCD.clear(); LCD.print("Reset");
    int start = millis();

    sweepServo(ARM_LEFT, ARM_UP_LEFT, ARM_DOWN_EWOK_LEFT);
    sweepServo(CLAW_LEFT, CLAW_CLOSED_LEFT, CLAW_OPEN_LEFT);
    while((millis()-start) > 3000){ //follow for 4 s before looking for chewie
        bridgeFollow(BF_KP, BF_KD, BF_GAIN);
    }
    delay(1000); //just so we know when it changes 
    while(!ewokDetectRight()){
        bridgeFollow(BF_KP, BF_KD, BF_GAIN);
    }

    runState = RunState::CHEWIE;

    motor.stop(RIGHT_MOTOR);
    motor.stop(LEFT_MOTOR); //may not need these

}
// Starts right after chewie has been detected
// stops when chewie has been picked up
void Robot::CHEWIE() {
    pickUp(LEFT, CHEWIE);
    runState = RunState::ZIP_DOWN;

}
// Starts right after Chewie picked up
//ends when we beat dom and tom
void Robot::ZIP_DOWN() {

    //move forward until one switch hit zipline
    while(digitalRead(ZIPLINE_HIT_SWITCH_LEFT) && digitalRead(ZIPLINE_HIT_SWITCH_RIGHT)){
        move(10, 100);
    }

    centerOnZipline();
    contractZipline();
    zipUp();
}
