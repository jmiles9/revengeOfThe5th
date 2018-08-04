#include <phys253.h>          
#include <LiquidCrystal.h>     

    const int EWOK_IR_OUT_LEFT = 15;
void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  pinMode(EWOK_IR_OUT_LEFT,OUTPUT);
  attachInterrupt(2, encoderRightRising, CHANGE);
  attachInterrupt(3, encoderLeftRising, CHANGE);
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
const float umPerWheelIndex = wheelRadius * 3.14 * 2 / (TICKSPERROTATION) / (ENCODER_RATIO) / 3 * 1.26 * 2 * 1000;
const int wheelSeparation = 175;
const float degreesPermm = 360 / (3.14 * wheelSeparation) * 5 / 6 * 36 / 35;
const int TAPE_QRD_FAR_LEFT = 2;
const int TAPE_QRD_MID_LEFT = 3;
const int TAPE_QRD_MID_RIGHT = 4;
const int TAPE_QRD_FAR_RIGHT = 5; 
const int TAPE_QRD_THRESHOLD = 512;
int leftPower = 100;
int rightPower = 100;
    const int EWOK_SENSOR_LEFT = 6;

// leave the func(s) you want to test uncommented
void loop() {
    digitalWrite(EWOK_IR_OUT_LEFT,HIGH);
    delay(40);
    double with = analogRead(EWOK_SENSOR_LEFT);
    digitalWrite(EWOK_IR_OUT_LEFT,LOW);
    delay(40);
    double without = analogRead(EWOK_SENSOR_LEFT);
    Serial.println(with-without);
delay(1000);

}

/// MARK: Movement functions

//PARAM: deg - degrees to turn clockwise
void turn(int deg) {
    moveWheels(deg / degreesPermm, -deg / degreesPermm, -200, 200);
}

//PARAM: distance - distance in mm to more forwards
void move(int distance, int speed) {
    int originalLeft = leftWheelIndex;
    int originalRight = rightWheelIndex;
    int32_t leftPower = speed / float(800) * 255;
    int32_t rightPower = speed / float(875) * 255;
    setMotorPower(leftPower, rightPower);
    Serial.println(leftPower);
    while(distanceTravelled(leftWheelIndex, originalLeft) < abs(distance) && distanceTravelled(rightWheelIndex, originalRight) < abs(distance)) {
        Serial.print("Left: "); Serial.print(distanceTravelled(leftWheelIndex,originalLeft));
        Serial.print("     Right: "); Serial.println(distanceTravelled(rightWheelIndex,originalRight));
    }
    setMotorPower(0,0);
}

void moveWheels(int leftDistance, int rightDistance, int32_t lSpeed, int32_t rSpeed) {
    int32_t leftPower = lSpeed * 255 / 700;
    int32_t rightPower = rSpeed * 255 / 500;
    rSpeed *= 1.005;
    int originalRightIndex = rightWheelIndex;
    int originalLeftIndex = leftWheelIndex;
    setMotorPower(leftPower, rightPower);
    delay(100);
    while(distanceTravelled(leftWheelIndex, originalLeftIndex) < abs(leftDistance) && distanceTravelled(rightWheelIndex, originalRightIndex) < abs(rightDistance)) {
        rightPower = maintainSpeed(RIGHT, rSpeed, rightPower);
        leftPower = maintainSpeed(LEFT, lSpeed, leftPower);
        Serial.print("Left: "); Serial.print(distanceTravelled(leftWheelIndex,originalLeftIndex));
        Serial.print("     Right: "); Serial.println(distanceTravelled(rightWheelIndex,originalRightIndex));
    }
    setMotorPower(-leftPower, -rightPower);
    delay(10);
    setMotorPower(0,0);
}

//// Maintains a speed for one side.
//// Param:
//// power       - current power of the motor
//// targetSpeed - desired speed
//// side        - side
//// Returns: Current power of motor.
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
            power--;
          } else {
            power++;
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

float distanceTravelled(uint32_t newIndex, uint32_t oldIndex) {
  Serial.print("NEW: "); Serial.print(newIndex);
  Serial.print("      OLD:  "); Serial.println(oldIndex);
    return float((newIndex - oldIndex)) / float(1000.0) * umPerWheelIndex * 2.7 / 2 / 1.4 / 1.08 / 1.08 / 1.05;
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
    rightSpeed = umPerWheelIndex / (time - rightWheelLastTime);
    rightWheelLastTime = time;
}

void encoderLeftRising() {
	int time = millis();
    if(time - leftWheelLastTime < 5) {
        return;
    }
    leftWheelIndex++;
    leftSpeed = umPerWheelIndex / (time - leftWheelLastTime);
    leftWheelLastTime = time;
}

