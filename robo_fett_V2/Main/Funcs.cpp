#include "configs.h"
#include "Funcs.h"

//rightSpeed; //can't get this from Robot?? how?
//same with other vals
using namespace configs;

int tf_power;

 // Used in tapeFollow
void Funcs::setMotorPower(int left, int right) {
    motor.speed(RIGHT_MOTOR, -right);
    motor.speed(LEFT_MOTOR, left);
}



/**
 * tape following - reads sensor values, sets motor speeds using pd
 * param: kp = proportional constant
 *        kd = derivative constant
 *        gain = gain for pd
 */
void Funcs::tapeFollow(int kp, int kd, int gain, int power) {
    tf_power = power;
    bool farLeftOnTape = (analogRead(TAPE_QRD_FAR_LEFT) > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool midLeftOnTape = (analogRead(TAPE_QRD_MID_LEFT) > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool midRightOnTape = (analogRead(TAPE_QRD_MID_RIGHT) > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool farRightOnTape = (analogRead(TAPE_QRD_FAR_RIGHT) > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;

    //setting error values
    int lasterr = error; //saving the old error value

    if(farLeftOnTape && !midLeftOnTape && !midRightOnTape && !farRightOnTape) {
        error = LARGE_LEFT_ERROR;
    } else if(farLeftOnTape && midLeftOnTape && !midRightOnTape && !farRightOnTape) {
        error = MED_LEFT_ERROR;
    } else if(!farLeftOnTape && midLeftOnTape && !midRightOnTape && !farRightOnTape) {
        error = SMALL_LEFT_ERROR;
    } else if(!farLeftOnTape && midLeftOnTape && midRightOnTape && !farRightOnTape) {
        error = CENTERED_ERROR;
    } else if(!farLeftOnTape && !midLeftOnTape && midRightOnTape && !farRightOnTape) {
        error = SMALL_RIGHT_ERROR;
    } else if(!farLeftOnTape && !midLeftOnTape && midRightOnTape && farRightOnTape) {
        error = MED_RIGHT_ERROR;
    } else if(!farLeftOnTape && !midLeftOnTape && !midRightOnTape && farRightOnTape) {
        error = LARGE_RIGHT_ERROR;
    } else {
        // if QRDs read other values, let robot go straight until they do.
        error = 0;
    }

    steer((kp*error + kd*(error - lasterr))*gain);
}

// Used in tapefollow
void Funcs::steer(int error) {
    if(error > 0) {
        if(error > tf_power) error = tf_power;
        setMotorPower(tf_power, tf_power - error);
    } 
    else if(error < 0) {
        if(-error > tf_power) error = -tf_power;
        setMotorPower(tf_power + error, tf_power);
    } 
    else setMotorPower(tf_power, tf_power);
}

// Stops hard
void Funcs::hardStop() {
    Serial.println("STOP");
    setMotorPower(FULL_R, FULL_R);
    delay(10);
    setMotorPower(0,0);
}

// Param - distance in cm
void Funcs::tapeFollowForDistance(int distance, int speed) {
    leftWheelIndex = 0;
    rightWheelIndex = 0;
    int originalLeftIndex = leftWheelIndex;
    int originalRightIndex = rightWheelIndex;
    setMotorPower(255,255);
    while((distanceTravelled(leftWheelIndex, originalLeftIndex) + distanceTravelled(rightWheelIndex, originalRightIndex)) / 2 < distance) {
        tapeFollow(TF_KP1, TF_KD1, TF_GAIN1, speed);
    }
    hardStop();
}

/// Parameters:
/// side and stuffy are defined in config now.
/// returns true if pickup was successful.
void Funcs::pickUp(int side, int stuffy) {
    TINAH::Servo arm;
    TINAH::Servo claw;

    if(side == LEFT) {
        arm = ARM_LEFT;
        claw = CLAW_LEFT;
    } else {
        arm = ARM_RIGHT;
        claw = CLAW_RIGHT;
    }
    int armDown;
    if(stuffy == EWOK) {
        armDown = ARMS_DOWN_EWOK;
    } else {
        armDown = ARMS_DOWN_CHEWIE;
    }
    sweepServo(claw, CLAWS_CLOSED, CLAWS_OPEN);
    delay(1000);
    sweepServo(arm, ARMS_UP, armDown);
    delay(1000);
    sweepServo(claw, CLAWS_OPEN, CLAWS_CLOSED);
    delay(1000);
    sweepServo(arm, armDown, ARMS_UP);
}

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

//PARAM: deg - degrees to turn clockwise
void Funcs::turn(int deg) {
    moveWheels(deg / degreesPerCm, -deg / degreesPerCm, 100, -100);
}

//PARAM: distance - distance in cm
void Funcs::move(int distance) {
    moveWheels(distance, distance, 255,250);
}

void Funcs::moveWheels(float leftDistance, float rightDistance, int leftPower, int rightPower) {
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
    while(distanceTravelled(leftWheelIndex, originalLeftIndex) < abs(leftDistance) && distanceTravelled(rightWheelIndex, originalRightIndex) < abs(rightDistance)) {
    }
    hardStop();
}

// Maintains a speed for one side.
// Param:
//       power - current power of the motor
// targetSpeed - desired speed
//        side - side
// Returns: Current power of motor.
int Funcs::maintainSpeed(int side, int targetSpeed, int power) {
    int currSpeed;
    if(side == LEFT) {
        currSpeed = leftSpeed;
    } else {
        currSpeed = rightSpeed;
    }

    if(currSpeed != targetSpeed) {
        power = power + (targetSpeed - currSpeed) * 20;
    }
    power = max(FULL_R, power);
    power = min(FULL_F, power);
    setMotorPower(side, power);
    return power;
}

void Funcs::rotateUntilTape() {
    setMotorPower(100,-100);
    while(true) {
        if(analogRead(TAPE_QRD_MID_LEFT > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE == ON_TAPE 
        || analogRead(TAPE_QRD_MID_RIGHT > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE == ON_TAPE) {
            setMotorPower(0,0);
            break;
        }
    }
}

void Funcs::rotateUntilTapeCCW() {
    setMotorPower(-100,100);
    while(true) {
        if(analogRead(TAPE_QRD_MID_LEFT > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE == ON_TAPE 
        || analogRead(TAPE_QRD_MID_RIGHT > TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE == ON_TAPE) {
            setMotorPower(0,0);
            break;
        }
    }
}

int Funcs::speedToPower(int speed) {
    int power = speed * 20;
    power = min(FULL_F, power);
    power = max(FULL_R, power);
    return power;
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
        // Give the arm 8 seconds to extend
        if(millis() > startTime + 8000) {
            break;
        }
    }
    motor.stop(ZIP_ARM_MOTOR);
}

void Funcs::contractZipline() {
    motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_CONTRACTING);
    double startTime = millis();
    while(!digitalRead(ZIP_SWITCH_CLOSED)) {
        // Give the arm 8 seconds to extend
        if(millis() > startTime + 8000) {
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

void Funcs::findEdge() {
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
bool Funcs::edgeDetect() {
    //probs dont need this since it finds edge with tapefollow?
}

void Funcs::tapeFollowToEdge(int speed){

    while(true) {
        tapeFollow(TF_KP1, TF_KD1, TF_GAIN1, speed);
        if(edgeDetect()) {
            hardStop();
        }
    }
}
float Funcs::distanceTravelled(int newIndex, int oldIndex) {
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

void Funcs::sweepServo(TINAH::Servo servo, int startAngle, int endAngle) {
    int dA;
    int currAngle = startAngle;
    if(endAngle > startAngle) {
        while(currAngle < endAngle) {
            servo.write(currAngle);
            currAngle += 3;
        }
    } else if(endAngle < startAngle) {
        while(currAngle > endAngle) {
            servo.write(currAngle);
            currAngle -= 3;
        }
    }
}