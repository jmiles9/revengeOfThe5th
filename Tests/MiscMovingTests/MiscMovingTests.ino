#include <phys253.h>          
#include <LiquidCrystal.h>     

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  attachInterrupt(2, encoderRightRising, RISING);
  attachInterrupt(3, encoderLeftRising, RISING);
}

int count = 0;
uint32_t leftWheelLastTime = 0;
uint32_t leftWheelIndex = 0;
int leftSpeed = 0;
uint32_t rightWheelLastTime = 0;
uint32_t rightWheelIndex = 0;
int rightSpeed = 0;

const int LEFT = 0;
const int RIGHT = 1;  
const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 1;

const int FULL_F = 255;
const int FULL_R = -255;
const float ENCODER_RATIO = 3 / 2;
const int TICKSPERROTATION = 48;
const float wheelRadius = 31.7;
const int umPerWheelIndex = wheelRadius * 3.14 * 2 / (TICKSPERROTATION) / (ENCODER_RATIO) / 3 * 2 * 1000;
const int wheelSeparation = 175;
const float degreesPermm = 360 / (3.14 * wheelSeparation);
    const int TAPE_QRD_FAR_LEFT = 3;
    const int TAPE_QRD_MID_LEFT = 4;
    const int TAPE_QRD_MID_RIGHT = 5;
    const int TAPE_QRD_FAR_RIGHT = 6; 
         const int TAPE_QRD_THRESHOLD = 512;
int leftPower = 100;
int rightPower = 100;

// leave the func(s) you want to test uncommented
void loop() {
//  setMotorPower(255,255);
//  delay(200);
//  Serial.print("LEFT: ");
//  Serial.println(leftSpeed);
//  Serial.print("Right: ");
//
//  Serial.println(rightSpeed);
//  delay(200);
//  Serial.print("LEFT: ");
//  Serial.println(leftSpeed);
//  Serial.print("Right: ");
//
//  Serial.println(rightSpeed);
    move(1000,200);
//    turn(90);
//    rotateUntilTape();
//    rotateUntilTapeCCW();
//    delay(50000);

//    setMotorPower(leftPower, rightPower);
//    delay(50);
//    while(true) {
//        leftPower = maintainSpeed(LEFT, 50, leftPower);
//        rightPower = maintainSpeed(RIGHT, 50, rightPower);
//    }
//    setMotorPower(50,50);
delay(50000);
}

/// MARK: Movement functions

//PARAM: deg - degrees to turn clockwise
void turn(int deg) {
    moveWheels(deg / degreesPermm, -deg / degreesPermm, 50, -50);
}

//PARAM: distance - distance in mm to more forwards
void move(int distance, int speed) {
    moveWheels(distance, distance, speed, speed);
}

void moveWheels(int leftDistance, int rightDistance, int32_t lSpeed, int32_t rSpeed) {
    int32_t leftPower = lSpeed * 255 / 800;
    int32_t rightPower = rSpeed * 255 / 800;
    int originalRightIndex = rightWheelIndex;
    int originalLeftIndex = leftWheelIndex;
    setMotorPower(leftPower, rightPower);
    delay(100);
    while(distanceTravelled(leftWheelIndex, originalLeftIndex) < abs(leftDistance) && distanceTravelled(rightWheelIndex, originalRightIndex) < abs(rightDistance)) {
        rightPower = maintainSpeed(RIGHT, rSpeed, rightPower);
        leftPower = maintainSpeed(LEFT, lSpeed, leftPower);
        Serial.print("left: "); Serial.println(leftSpeed);
        Serial.print("right: "); Serial.println(rightSpeed);
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
int maintainSpeed(int side, int targetSpeed, int power) {
    int currSpeed;
    if(side == LEFT) {
        if(millis() - leftWheelLastTime > 500) {
          LCD.clear(); LCD.setCursor(0,0); LCD.print("ERROR");
          leftSpeed = 0;
        } 
        currSpeed = leftSpeed;
    } else {
        if(millis() - rightWheelLastTime > 500) {
          LCD.clear(); LCD.setCursor(0,0); LCD.print("ERROR");
          rightSpeed = 0;
        } 
        currSpeed = rightSpeed;
    }

    
    if(currSpeed != targetSpeed) {
        if(power < 0) {
          if(targetSpeed - currSpeed < 0) {
            power++;
          } else {
            power--;
          }
        } else {
          if(targetSpeed - currSpeed < 0) {
            power--;
          } else {
            power++;
          }
        }
    }
    power = max(FULL_R, power);
    power = min(FULL_F, power);
    if(side == LEFT) {
        motor.speed(LEFT_MOTOR, power);
    } else if(side == RIGHT) {
        motor.speed(RIGHT_MOTOR, -power);
    }
    return power;
}

int distanceTravelled(uint32_t newIndex, uint32_t oldIndex) {
    Serial.println((newIndex - oldIndex) * umPerWheelIndex / 1000);
    return (newIndex - oldIndex) * umPerWheelIndex / 1000;
}


void setMotorPower(int left, int right) {
    motor.speed(RIGHT_MOTOR, -right);
    motor.speed(LEFT_MOTOR, left);
}

/// MARK: Encoder interrupt functions
void encoderRightRising() {
    int time = millis();
    if(time - rightWheelLastTime < 5) {
        return;
    }
    rightWheelIndex++;
  if(rightWheelIndex % 10 == 0) {
      rightSpeed = umPerWheelIndex / (time - rightWheelLastTime) * 3;
      rightWheelLastTime = time;
  }
    attachInterrupt(2, encoderRightFalling, FALLING);
}

void encoderRightFalling() {
    int time = millis();
    if(time - rightWheelLastTime < 5) {
        return;
    }
    rightWheelIndex++;
  if(rightWheelIndex % 10 == 0) {
      rightSpeed = umPerWheelIndex / (time - rightWheelLastTime) * 10;
      rightWheelLastTime = time;
  }
    attachInterrupt(2, encoderRightRising, RISING);
}

void encoderLeftFalling() {
	int time = millis();
    if(time - leftWheelLastTime < 5) {
        return;
    }
    leftWheelIndex++;
  if(leftWheelIndex % 10 == 0) {
      leftSpeed = umPerWheelIndex / (time - leftWheelLastTime) * 10;
      leftWheelLastTime = time;
  }
    attachInterrupt(3, encoderLeftRising, RISING);
}

void encoderLeftRising() {
	int time = millis();
    if(time - leftWheelLastTime < 5) {
        return;
    }
    leftWheelIndex++;
  if(leftWheelIndex % 10 == 0) {
      leftSpeed = umPerWheelIndex / (time - leftWheelLastTime) * 10;
      leftWheelLastTime = time;
  }
    attachInterrupt(3, encoderLeftFalling, FALLING);
}

