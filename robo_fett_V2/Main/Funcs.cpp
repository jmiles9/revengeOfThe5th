#include "configs.h"
#include "Funcs.h"

//rightSpeed; //can't get this from Robot?? how?
//same with other vals
using namespace configs;

TINAH::Servo RCServo7(RCSERVO7);
TINAH::Servo RCServo6(RCSERVO6);

 // Used in tapeFollow
void Funcs::setMotorPower(int left, int right) {
    motor.speed(RIGHT_MOTOR, right);
    motor.speed(LEFT_MOTOR, left);
}
    
// Used in tapefollow
void Funcs::steer(int deg, int speed) {
    if(deg > 0) {
        if(deg > speed*2) {
            deg = speed*2;
        }
        setMotorPower(speed - deg, speed);
    } else if(deg < 0) {
        if(-deg > speed*2) {
            deg = -speed*2;
        }
        setMotorPower(speed, speed + deg);
    } else setMotorPower(speed, speed);
}

void Funcs::hardStop() {
    setMotorPower(FULL_R, FULL_R);
    delay(10); //DO WE WANT A DELAY HERE?? check this
    setMotorPower(0,0);
}

// TODO: do something about error
/**
 * tape following - reads sensor values, sets motor speeds using pd
 * param: kp = proportional constant
 *        kd = derivative constant
 *        gain = gain for pd
 */
bool Funcs::tapeFollow(int kp, int kd, int gain, Speed speed_) {
    //defining what speeds to use
    int highSpeed;
    int lowSpeed;
    switch(speed_){
        default:
            highSpeed = 255;
            lowSpeed = 200;
            break;
    }

    bool rightOnTape = digitalRead(TAPE_QRD_RIGHT);
    bool leftOnTape = digitalRead(TAPE_QRD_LEFT);
    bool edgeDetect = digitalRead(EDGE_QRD);
    //boolean edgeDetect = false; //this turns off edge detecting for testing purposes

    //setting error values
    int lasterr = error; //saving the old error value

    if(rightOnTape && leftOnTape){
        if(edgeDetect){
            hardStop();
            return true;
        }
    }
    if(!rightOnTape && leftOnTape) error = -1;
    if(rightOnTape && !leftOnTape) error = 1;
    if(!rightOnTape && !leftOnTape){
        if(error == -1 || error == -5) error = -5;
        else error = 5;
    }
    //steering for error
    steer((kp*error + kd*(error - lasterr))*gain, FULL_F) ;
    return false;
}

// TODO: complete
// Param - distance in cm
// NOTE: currently assuming only one set of pid constants
void Funcs::tapeFollowForDistance(int distance) {
    /*originalLeftIndex = leftWheelIndex;
    originalRightIndex = rightWheelIndex;
    while((distanceTravelled(leftWheelIndex, originalLeftIndex) 
    + distanceTravelled(rightWheelIndex, originalRightIndex)) / 2 < distance) {
        tapeFollow(TF_KP1,TF_KD1,TF_GAIN1,TF_SPEED1);
    }
    stop();*/
}

/// Parameters:
/// side and stuffy are defined in config now.
/// returns true if pickup was successful.
//TODO: finish this
bool Funcs::pickUp(int side, int stuffy) {
    bool stuffyPicked = false;

    if(side == LEFT){
        if(stuffy == EWOK) ARM_LEFT.write(ARMS_DOWN_EWOK);
        else ARM_LEFT.write(ARMS_DOWN_CHEWIE);

        delay(1000);

        TONGS_LEFT.write(CLAWS_CLOSED);

        delay(1000);

        if(digitalRead(LEFT_CLAW_STUFFY_SWITCH)) stuffyPicked = true;
    }

    if(side == RIGHT){
        if(stuffy == EWOK) ARM_RIGHT.write(ARMS_DOWN_EWOK);
        else ARM_RIGHT.write(ARMS_DOWN_CHEWIE);

        delay(1000);

        TONGS_RIGHT.write(CLAWS_CLOSED);

        delay(1000);

        if(digitalRead(RIGHT_CLAW_STUFFY_SWITCH)) stuffyPicked = true;
    }
    

    // If pickup is unsuccessful, can raise arm and open claw at the same time.
    /* if(!stuffyPicked) {
        ARMS.write(ARMS_UP);
        CLAWS.write(CLAWS_OPEN);
        return stuffyPicked;
    }

    else {
        ARMS.write(ARMS_UP);
        delay(1000);
        CLAWS.write(CLAWS_OPEN);
        return stuffyPicked;
    }*/
}

// CURRENTLY ASSUMINE ONE IR SENSOR
double Funcs::record1KIRBeacon() {
    return analogRead(IR_1KHZ);
}

double Funcs::record10KIRBeacon() {
    return analogRead(IR_10KHZ);
}

void Funcs::lowerBridge() {
    BASKET.write(BASKET_DROPBRIDGE);
    delay(1000);
    // may not need to close
    BASKET.write(BASKET_CLOSED);
}

bool Funcs::checkBeacon() {
    return record10KIRBeacon() > record1KIRBeacon();
}


/**
* ewok detecting - reads IR sensor, decides if it's looking at an ewok
* return: true if ewok, false if not
*/
//TODO: CHECK THIS
bool Funcs::ewokDetect() {
    digitalWrite(IR_OUT,HIGH);
    double without = analogRead(EWOK_SENSOR);
    digitalWrite(IR_OUT,LOW);
    double with = analogRead(EWOK_SENSOR);
    if(with-without > EWOK_THRESH) return true; 
    else return false;
}

//PARAM: deg - degrees to turn clockwise
void Funcs::turn(int deg) {
    /*std::thread right(moveRightWheel, -1 * deg / degreesPerCm, MAX_SPEED / 2);
    std::thread left(moveLeftWheel, deg / degreesPerCm, MAX_SPEED / 2);

    right.join();
    left.join();*/
}

//PARAM: deg - degrees to turn clockwise
void Funcs::move(int distance) {
    /*std::thread right (moveRightWheel,distance,MAX_SPEED);
    std::thread left (moveLeftWheel,distance,MAX_SPEED);

    right.join();
    left.join();*/
}

//PARAM: distance - distance in cm (positive or negative)
//       speed    - speed in cm/s (always positive)
void Funcs::moveRightWheel(int distance, int speed) {
    // makes distance absoute value, speed directional
    /*if(distance < 0) {
        distance = distance * -1;
        speed = speed * -1;
    }
    int currIndex = rightWheelIndex;
    int currDistance = 0;
    int currSpeed = speed;
    motor.speed(RIGHT_MOTOR, speedToPower(currSpeed));
    while(currIndex - rightWheelIndex < 2) {
        if currDistance >= distance {
            break;
        }
    }
    while(currDistance < distance) {
        if(rightSpeed < speed) {
            currSpeed--;
        }
        motor.speed(RIGHT_MOTOR, speedToPower(currSpeed));
    }
    motor.stop(RIGHT_MOTOR);*/
}

//PARAM: distance - distance in cm (positive or negative)
//       speed    - speed in cm/s (always positive)
void Funcs::moveLeftWheel(int distance, int speed) {
    // makes distance absoute value, speed directional
    /*if(distance < 0) {
        distance = distance * -1;
        speed = speed * -1;
    }
    int currIndex = leftWheelIndex;
    int currDistance = 0;
    int currSpeed = speed;
    motor.speed(LEFT_MOTOR, speedToPower(currSpeed));
    while(currIndex - leftWheelIndex < 2) {
        if(currDistance >= distance) {
            break;
        }
    }
    while(currDistance < distance) {
        if(rightSpeed < speed) {
            currSpeed--;
        }
        motor.speed(LEFT_MOTOR, speedToPower(currSpeed));
    }
    motor.stop(LEFT_MOTOR);*/
}

//TODO: check float calculation (slow af??)
int Funcs::speedToPower(int speed) {
    int power = int(float(speed) / MAX_SPEED * 255);
    if(power > MAX_SPEED) {
        return MAX_SPEED;
    } else if(power < MAX_SPEED * -1) {
        return MAX_SPEED * -1;
    } else {
        return power;
    }
}

void Funcs::dumpBasket() {
    BASKET.write(BASKET_OPENED);
    delay(1000);
    BASKET.write(BASKET_CLOSED);
}

void Funcs::extendZipline() {
    motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_EXTENDING);
    double startTime = millis();
    while(!digitalRead(ZIP_SWITCH_EXTENDED)) {
        // Give the arm 5 seconds to extend
        if(millis() > startTime + 5000) {
            break;
        }
    }
    motor.stop(ZIP_ARM_MOTOR);
}

void Funcs::contractZipline() {
    motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_CONTRACTING);
    double startTime = millis();
    while(!digitalRead(ZIP_SWITCH_CLOSED)) {
        // Give the arm 5 seconds to extend
        if(millis() > startTime + 5000) {
            break;
        }
    }
    motor.stop(ZIP_ARM_MOTOR);
}

void Funcs::zipUp() {
    motor.speed(ZIP_WHEEL_MOTOR, ZIPPING_UP);
    double startTime = millis();
    while(!digitalRead(ZIPPED_UP_SWITCH)) {
        // Give 10 seconds to make it up zipline
        if(millis() > startTime + 10000) {
            break;
        }
    }
    motor.stop(ZIP_WHEEL_MOTOR);
}

//TODO:
void Funcs::findEdge() {
    //dont think we need this since it finds the edge in the 
    //tape follow function?
}

//TODO: write this. Should pretty much be tapefollow, different sensors.
void FUNCS::bridgeFollow(int kp, int kd, int gain, Speed speed_) {

    //should be true if they are on the bridge
    bool rightOnBridge = digitalRead(BRIDGE_QRD_RIGHT);
    bool leftOnBridge = digitalRead(BRIDGE_QRD_LEFT);

    int lasterr = err;

    if (leftOnBridge && !(rightOnBridge)){
        err = 0;
        //is good
    }
    if (leftOnBridge && rightOnBridge){
        //need to turn right
        err = 1;
    }
    if (!(leftOnBridge) && (!(rightOnBridge))){
        err = -1;
    }
    
    steer((kp*err + kd*(err - lasterr))*gain) ;
}

//TODO: Write this
bool Funcs::isOnEdge() {
    //probs dont need this since it finds edge with tapefollow?
}

double Funcs::distanceTravelled(int newIndex, int oldIndex) {
    return newIndex - oldIndex * cmPerWheelIndex;
}
