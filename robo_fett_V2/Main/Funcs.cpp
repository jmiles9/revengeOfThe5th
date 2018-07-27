#include "configs.h"
#include "Funcs.h"

//rightSpeed; //can't get this from Robot?? how?
//same with other vals
using namespace configs;

TINAH::Servo RCServo7(RCSERVO7);
TINAH::Servo RCServo6(RCSERVO6);
int highSpeed;
int lowSpeed;

 // Used in tapeFollow
void Funcs::setMotorPower(int left, int right) {
    motor.speed(RIGHT_MOTOR, -right * 0.8);
    motor.speed(LEFT_MOTOR, left);
}
    
// Used in tapefollow
void Funcs::steer(int deg) {
    if(deg > 0) {
        if(deg > highSpeed) deg = highSpeed;
        setMotorPower(highSpeed, highSpeed - deg);
    } 
    else if(deg < 0) {
        if(-deg > highSpeed) deg = -highSpeed;
        setMotorPower(highSpeed + deg, highSpeed);
    } 
    else setMotorPower(highSpeed, highSpeed);
}

void Funcs::hardStop() {
    Serial.println("STOP");
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
    //Serial.println("in tapefollow");
    //defining what speeds to use
    switch(speed_){
        case LOWSPEED:
            highSpeed = 220; 
            lowSpeed = 190;
            break;
        default:
            highSpeed = 255;
            lowSpeed = 220;
            break;
    }

    bool rightOnTape = digitalRead(TAPE_QRD_RIGHT);
    bool leftOnTape = digitalRead(TAPE_QRD_LEFT);
    bool edgeDetect = digitalRead(EDGE_QRD);
//     LCD.clear();  LCD.home() ;
//     LCD.setCursor(0,0); LCD.print("left= "); LCD.print(leftOnTape); LCD.print("righ= "); LCD.print(rightOnTape);
//     LCD.setCursor(0,1); LCD.print("edge= "); LCD.print(edgeDetect);
// //    //boolean edgeDetect = false; //this turns off edge detecting for testing purposes

    //setting error values
    int lasterr = error; //saving the old error value

    if(rightOnTape && leftOnTape){
        if(edgeDetect){
            //hardStop();
//            LCD.clear();  LCD.home() ;
//            LCD.setCursor(0,0); LCD.print("edge "); 
    
            return ON_EDGE;
        }
        error = 0;
    }
    if(!rightOnTape && leftOnTape) {
      error = -1;
      Serial.println("");
    }
    if(rightOnTape && !leftOnTape) {
      error = 1;
    }
    if(!rightOnTape && !leftOnTape) {
        if(lasterr == -1 || lasterr == -5) {
          error = -5;
        } else if(lasterr == 1 || lasterr == 5) {
          error = 5;
        } else if(lasterr == 0) {
          Serial.println("DFJSODIJFSAOGHD");
          error = 0;
        }
    }

    if(error == 5 && distanceTravelled(leftWheelIndex, 0) > 120) {
      error = -0.5;
    }
    Serial.println(error);
    //steering for error
    steer((kp*error + kd*(error - lasterr))*gain) ;
    return NOT_ON_EDGE;
}

// TODO: complete
// Param - distance in cm
// NOTE: currently assuming only one set of pid constants
void Funcs::tapeFollowForDistance(int distance) {
    leftWheelIndex = 0;
    rightWheelIndex = 0;
    Serial.println("in tape distance");
    int originalLeftIndex = leftWheelIndex;
    int originalRightIndex = rightWheelIndex;
    setMotorPower(255,255);
    while((distanceTravelled(leftWheelIndex, originalLeftIndex) + distanceTravelled(rightWheelIndex, originalRightIndex)) / 2 < distance) {
        tapeFollow(TF_KP1,TF_KD1,TF_GAIN1,SPEED);
    }
    hardStop();
}

/// Parameters:
/// side and stuffy are defined in config now.
/// returns true if pickup was successful.
//TODO: finish this
bool Funcs::pickUp(int side, int stuffy) {
    bool stuffyPicked = false;

    TINAH::Servo ARM;
    TINAH::Servo CLAW;
    int stuffySwitch;

    if(side == LEFT) {
        ARM = ARM_LEFT;
        CLAW = CLAW_LEFT;
        stuffySwitch = LEFT_CLAW_STUFFY_SWITCH;
    } else {
        ARM = ARM_RIGHT;
        CLAW = CLAW_RIGHT;
        stuffySwitch = RIGHT_CLAW_STUFFY_SWITCH;
    }

    // if(stuffy == EWOK) {
    //     ARM.write(ARMS_DOWN_EWOK);
    // } else {
    //     ARM.write(ARMS_DOWN_CHEWIE);
    // }
    RCServo1.write(CLAWS_OPEN);
    delay(1000);
    RCServo0.write(ARMS_DOWN_EWOK);
    delay(1000);
    Serial.println("closing claws");
    RCServo1.write(CLAWS_CLOSED);
    delay(3000);
    Serial.println("raising arms");

    // if(digitalRead(stuffySwitch)) {
    //     stuffyPicked = true;
    // }

    // If pickup is unsuccessful, can raise arm and open claw at the same time.
    RCServo0.write(ARMS_UP);
    // if(!stuffyPicked) {
    //     delay(1000);
    // }
    //stuffyPicked = true;
    delay(3000);
    Serial.println("opening");
    RCServo1.write(CLAWS_OPEN);
    delay(3000);

    Serial.println("lowering");
    RCServo0.write(ARMS_DOWN_EWOK);
    delay(3000);
    Serial.println("closing claws");
    RCServo1.write(CLAWS_CLOSED);
    delay(3000);
    Serial.println("raising arms");
    RCServo0.write(ARMS_UP);

    Serial.println("done ewok grab");

    return stuffyPicked;
}

void Funcs::pickUpAndHoldHalfway(int side, int stuffy) {
    TINAH::Servo ARM;
    TINAH::Servo CLAW;

    if(side == LEFT) {
        ARM = ARM_LEFT;
        CLAW = CLAW_LEFT;
    } else {
        ARM = ARM_RIGHT;
        CLAW = CLAW_RIGHT;
    }

    if(stuffy == EWOK) {
        ARM.write(ARMS_DOWN_EWOK);
    } else {
        ARM.write(ARMS_DOWN_CHEWIE);
    }

    delay(1000);

    CLAW.write(CLAWS_CLOSED);

    delay(1000);

    ARM.write(ARM_HALF);
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
//TODO: MAKE THIS WORK ON EITHER SIDE SENSOR!1!!


//PARAM: deg - degrees to turn clockwise
void Funcs::turn(int deg) {
    moveWheels(deg / degreesPerCm, -1 * deg / degreesPerCm, 150);
}

//PARAM: distance - distance in cm
void Funcs::move(int distance) {
    moveWheels(distance, distance, 200);
}

void Funcs::moveWheels(int leftDistance, int rightDistance, int power) {
    int leftPower = power;
    int rightPower = power;
    int leftFactor = 1;
    int rightFactor = 1;
    if(leftDistance < 0) {
        leftFactor = -1;
    }
    if(rightDistance < 0) {
        rightFactor = -1;
    }
    leftPower *= leftFactor;
    rightPower *= rightFactor;
    int originalRightIndex = rightWheelIndex;
    int rightCurrDistance = 0;
    int originalLeftIndex = leftWheelIndex;
    int leftCurrDistance = 0;
    setMotorPower(leftPower, rightPower);
    bool leftDone = false;
    bool rightDone = false;
    while(distanceTravelled(leftWheelIndex, originalLeftIndex) < abs(leftDistance) || distanceTravelled(rightWheelIndex, originalRightIndex) < abs(rightDistance)) {
        if(!leftDone) {
            if(distanceTravelled(leftWheelIndex, originalLeftIndex) > abs(leftDistance)) {
                motor.speed(LEFT_MOTOR, 0);
                leftDone = true;
            }
        }
        if(!rightDone) {
            if(distanceTravelled(rightWheelIndex, originalRightIndex) > abs(rightDistance)) {
                motor.speed(RIGHT_MOTOR, 0);
                rightDone = true;
            }
        }
    }
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
    delay(200);
    move(-DUMP_RAM_DISTANCE);
    move(DUMP_RAM_DISTANCE + 1);
    delay(500);
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
void Funcs::bridgeFollow(int kp, int kd, int gain) {

    //should be true if they are OFF the bridge
    bool rightOffBridge = digitalRead(BRIDGE_QRD_RIGHT);
    bool leftOffBridge = digitalRead(BRIDGE_QRD_LEFT);

    int lastError = error;
    //use same static error that tapefollow uses, shouldn't be issue as don't tapefollow again

    if (!(leftOffBridge) && rightOffBridge){
        error = 0;
        //is good
    }
    if (leftOffBridge && rightOffBridge){
        //need to turn left
        error = -1;
    }
    if (!(leftOffBridge) && (!(rightOffBridge))){
        //need to turn right
        error = 1;
    }
    
    steer((kp*error + kd*(error - lastError))*gain) ;
}

//TODO: Write this
bool Funcs::isOnEdge() {
    //probs dont need this since it finds edge with tapefollow?
}

void Funcs::tapeFollowToEdge(){

    while(true) {
    bool temp = Funcs::tapeFollow(TF_KP1,TF_KD1,TF_GAIN1,Speed::SPEED);
    if(temp == ON_EDGE) {
        hardStop();
        //Serial.println("on edge"); delay(1000);
        break;
    }
}
}
double Funcs::distanceTravelled(int newIndex, int oldIndex) {
    return (newIndex - oldIndex) * cmPerWheelIndex * DIST_CONV;
}

bool Funcs::ewokDetect() {
    digitalWrite(EWOK_IR_OUT,HIGH);
    delay(50);
    double with = analogRead(EWOK_SENSOR);
    digitalWrite(EWOK_IR_OUT,LOW);
    delay(50);
    double without = analogRead(EWOK_SENSOR);
    int count = 0;
      Serial.println(with-without-EWOK_THRESH);
      while(abs(with-without > EWOK_THRESH)) {
        Serial.println(with-without-EWOK_THRESH);
        Serial.println(count);
        count++;
        if(count >= 2) {
            Serial.println("RETURNING TRUE");
         return true;
        }
        digitalWrite(EWOK_IR_OUT,HIGH);
        delay(50);
        with = analogRead(EWOK_SENSOR);
        digitalWrite(EWOK_IR_OUT,LOW);
        delay(50);
        without = analogRead(EWOK_SENSOR);
        Serial.println(with-without);
    }
    return false;
}