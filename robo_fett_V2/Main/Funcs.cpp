#include "configs.h"
#include "Funcs.h"

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

// Param - distance in mm
void Funcs::tapeFollowForDistance(int distance, int speed) {
    leftWheelIndex = 0;
    rightWheelIndex = 0;
    int originalLeftIndex = leftWheelIndex;
    int originalRightIndex = rightWheelIndex;
    setMotorPower(speed,speed);
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

    int clawClose;
    int clawOpen;
    int armUp;
    int armDown;

    if(side == LEFT) {
        arm = ARM_LEFT;
        claw = CLAW_LEFT;
        clawClose = CLAW_CLOSED_LEFT;
        clawOpen = CLAW_OPEN_LEFT;
        armUp = ARM_UP_LEFT;
        if(stuffy == EWOK) {
            armDown = ARM_DOWN_EWOK_LEFT;
        } else {
            armDown = ARM_DOWN_CHEWIE_LEFT;
        }
    } else {
        arm = ARM_RIGHT;
        claw = CLAW_RIGHT;
        clawClose = CLAW_CLOSED_RIGHT;
        clawOpen = CLAW_OPEN_RIGHT;
        armUp = ARM_UP_RIGHT;
        if(stuffy == EWOK) {
            armDown = ARM_DOWN_EWOK_RIGHT;
        } else {
            armDown = ARM_DOWN_CHEWIE_RIGHT;
        }

    }
   
    sweepServo(claw, clawClose, clawOpen);
    delay(1000);
    sweepServo(arm, armUp, armDown);
    delay(1000);
    sweepServo(claw, clawOpen, clawClose);
    delay(1000);
    sweepServo(arm, armDown, armUp);
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
    BASKET.write(BASKET_REST);
}

bool Funcs::checkBeacon() {
    return record10KIRBeacon() > record1KIRBeacon();
}

//PARAM: deg - degrees to turn clockwise
void Funcs::turn(int deg) {
    moveWheels(deg / degreesPermm, -deg / degreesPermm, 50, 50);
}

//PARAM: distance - distance in mm to move.
//         speed  - speed to move. Must be positive.
void Funcs::move(int distance, int speed) {
    moveWheels(distance, distance, speed, speed);
}

// helper function
// moves wheels lol
void Funcs::moveWheels(int leftDistance, int rightDistance, int leftSpeed, int rightSpeed) {
    int leftPower = leftSpeed * 255 / 290;
    int rightPower = rightSpeed * 255 / 290;
    leftPower = max(leftPower, FULL_F);
    leftPower = min(leftPower, FULL_R);
    rightPower = max(rightPower, FULL_F);
    rightPower = min(rightPower, FULL_R);
    int originalRightIndex = rightWheelIndex;
    int rightCurrDistance = 0;
    int originalLeftIndex = leftWheelIndex;
    int leftCurrDistance = 0;
    setMotorPower(leftPower, rightPower);
    delay(50);
    while(distanceTravelled(leftWheelIndex, originalLeftIndex) < abs(leftDistance) && distanceTravelled(rightWheelIndex, originalRightIndex) < abs(rightDistance)) {
        rightPower = maintainSpeed(RIGHT_MOTOR, rightSpeed, rightPower);
        leftPower = maintainSpeed(LEFT_MOTOR, leftSpeed, leftPower);
    }
    setMotorPower(-leftPower, -rightPower);
    delay(10);
    setMotorPower(0,0);
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
    if(side == LEFT) {
        motor.speed(LEFT_MOTOR, power);
    } else if(side == RIGHT) {
        motor.speed(RIGHT_MOTOR, power);
    }
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
    BASKET.write(BASKET_DUMP);
    delay(500);
    BASKET.write(BASKET_REST);
}

void Funcs::extendZipline() {
    motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_EXTENDING);
    long startTime = millis();
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
    long startTime = millis();
    while(!digitalRead(ZIP_SWITCH_CLOSED)) {
        // Give the arm 8 seconds to contract
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
    return abs(analogRead(TAPE_QRD_FAR_LEFT) - EDGE_QRD_THRESHOLD) < 100
        && abs(analogRead(TAPE_QRD_MID_LEFT) - EDGE_QRD_THRESHOLD) < 100
        && abs(analogRead(TAPE_QRD_MID_RIGHT) - EDGE_QRD_THRESHOLD) < 100
        && abs(analogRead(TAPE_QRD_FAR_RIGHT) - EDGE_QRD_THRESHOLD) < 100;
}

void Funcs::tapeFollowToEdge(int speed){
    while(true) {
        tapeFollow(TF_KP1, TF_KD1, TF_GAIN1, speed);
        if(edgeDetect()) {
            hardStop();
        }
    }
}

// mm
int Funcs::distanceTravelled(int newIndex, int oldIndex) {
    return (newIndex - oldIndex) * umPerWheelIndex / 1000;
}

bool Funcs::ewokDetectRight() {
    digitalWrite(EWOK_IR_OUT_RIGHT,HIGH);
    delay(50);
    double with = analogRead(EWOK_SENSOR_RIGHT);
    digitalWrite(EWOK_IR_OUT_RIGHT,LOW);
    delay(50);
    double without = analogRead(EWOK_SENSOR_RIGHT);
    int count = 0;
    while(abs(with-without > EWOK_THRESH)) {
        Serial.println(with-without-EWOK_THRESH);
        Serial.println(count);
        count++;
        if(count >= 2) {
            Serial.println("RETURNING TRUE");
            return true;
        }
        digitalWrite(EWOK_IR_OUT_RIGHT,HIGH);
        delay(50);
        with = analogRead(EWOK_SENSOR_RIGHT);
        digitalWrite(EWOK_IR_OUT_RIGHT,LOW);
        delay(50);
        without = analogRead(EWOK_SENSOR_RIGHT);
        Serial.println(with-without);
        }
    return false;
}

bool Funcs::ewokDetectLeft() {
    digitalWrite(EWOK_IR_OUT_LEFT,HIGH);
    delay(50);
    double with = analogRead(EWOK_SENSOR_LEFT);
    digitalWrite(EWOK_IR_OUT_LEFT,LOW);
    delay(50);
    double without = analogRead(EWOK_SENSOR_LEFT);
    int count = 0;
    while(abs(with-without > EWOK_THRESH)) {
        Serial.println(with-without-EWOK_THRESH);
        Serial.println(count);
        count++;
        if(count >= 2) {
            Serial.println("RETURNING TRUE");
            return true;
        }
        digitalWrite(EWOK_IR_OUT_LEFT,HIGH);
        delay(50);
        with = analogRead(EWOK_SENSOR_LEFT);
        digitalWrite(EWOK_IR_OUT_LEFT,LOW);
        delay(50);
        without = analogRead(EWOK_SENSOR_LEFT);
        Serial.println(with-without);
        }
    return false;
}

//function to move servo, works more consistently than servo.write
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

//rotate until middle qrds are on tape
void Funcs::rotateUntilTape(int direction) {
    if(direction == CLOCKWISE) {
        setMotorPower(100,-100);
    } else {
        setMotorPower(-100,100);
    }
    while(true) {
        if(digitalRead(TAPE_QRD_MID_RIGHT) || digitalRead(TAPE_QRD_MID_LEFT)) {
            //stop
            if(direction == CLOCKWISE) {
                setMotorPower(-100,100);
            } else {
                setMotorPower(100,-100);
            }
            setMotorPower(0,0);
            break;
        }
    }
}

bool Funcs::digitalReadMultiplex(int port){
  int bin[3] = {0,0,0};
  int i = 0;
  int num = port;
  while (num > 0)
   {
       bin[i] = num % 2;
       num /= 2;
        i++;
   }

  digitalWrite(MULTI_CHOOSEA, bin[0]);
  digitalWrite(MULTI_CHOOSEB, bin[1]);
  digitalWrite(MULTI_CHOOSEC, bin[2]);

  return digitalRead(MULTIPLEX_IN);
  
}

// finds tape, turns left, turns right, big left, big right
void Funcs::findTape() {
    //first check left slightly
    int time = millis();
    setMotorPower(-100,100);
    while(millis() - time < 1000) {
        if( digitalRead(TAPE_QRD_MID_RIGHT) || digitalRead(TAPE_QRD_MID_LEFT) ) {
            setMotorPower(100,-100);
            setMotorPower(0,0);
            return;
        }
    }
    //then check right slightly
    time = millis();
    setMotorPower(100,-100); {
        while(millis() - time < 2000) {
            if( digitalRead(TAPE_QRD_MID_RIGHT) || digitalRead(TAPE_QRD_MID_LEFT) ) {
            setMotorPower(-100,100);
            setMotorPower(0,0);
            return;
        }
        }
    }
    //then check left more
    time = millis();
    setMotorPower(-100,100);
    while(millis() - time < 3000) {
        if( digitalRead(TAPE_QRD_MID_RIGHT) || digitalRead(TAPE_QRD_MID_LEFT) ) {
            setMotorPower(100,-100);
            setMotorPower(0,0);
            return;
        }
    }
    //then check right more
    time = millis();
    setMotorPower(100,-100); {
        while(millis() - time < 4000) {
            if( digitalRead(TAPE_QRD_MID_RIGHT) || digitalRead(TAPE_QRD_MID_LEFT) ) {
            setMotorPower(-100,100);
            setMotorPower(0,0);
            return;
        }
        }
    }
}
