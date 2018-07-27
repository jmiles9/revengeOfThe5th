#include <phys253.h>

const int COUNTS_PER_ROTATION = 6;
const int WHEEL_RADIUS = 30; //millimeters
const int WHEEL_CIRCUMFERENCE = 2*PI*WHEEL_RADIUS; //millimetres/rotation ==  
const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 1;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  Serial.print("circ = "); Serial.println(WHEEL_CIRCUMFERENCE);
  attachInterrupt(2, leftEncoder, RISING);
  attachInterrupt(3, rightEncoder, RISING);
  setMotorPower(100,100);
}

bool reset;
int saveTime = 0;
int leftCount = 0;
int rightCount = 0;
int lastLeftTime = 0;
int lastRightTime = 0;

void loop() {
  /*count++;
  if(count>100){
    Serial.println(encoderCount);
    count = 0;
  }*/
  int lastDistance = 0;
  int dt = millis() - saveTime;
  if(dt > 1000){
    int leftDis = leftCount/COUNTS_PER_ROTATION*WHEEL_CIRCUMFERENCE; //distance travelled (cm*1000) the 1000 will disappear when /dt
    Serial.print("count = "); Serial.println(leftCount);
    Serial.print("distance = "); Serial.println(leftDis);
    Serial.print("avr speed = "); Serial.print((leftDis - lastDistance) / (dt * 1000)); Serial.println(" cm/s"); 
    Serial.println();
    saveTime = millis();
    lastDistance = leftDis;
  }
  while(startbutton()){ reset = true;}
  if(reset) leftCount = 0;
  lastDistance = 0;
  reset = false;
}

void leftEncoder(){
  int t = millis();
  if(t - lastLeftTime < 40) {
    return;
  }
  lastLeftTime = t;
  leftCount++;
}

void rightEncoder() {
  rightCount++;
}

void setMotorPower(int left, int right) {
    motor.speed(RIGHT_MOTOR, -right);
    motor.speed(LEFT_MOTOR, left);
}

