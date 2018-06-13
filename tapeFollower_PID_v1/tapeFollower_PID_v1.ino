/**
 * Version one of tape follower code - with PD
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
#define LEFT_SENSOR 5  //analog port 3 (they are reversed on board!!)
#define RIGHT_SENSOR 0 //analog port 7
#define LEFT_MOTOR 3   //motor port 3
#define RIGHT_MOTOR 2  //motor port 2
#define KP_SET 2       //analog port 5
#define KD_SET 3       //analog port 4
#define GAIN_SET 4     //analog port 3

//motor speeds
#define FULL_F 255
#define HALF_F 200
#define FULL_R -255
#define HALF_R -200

#define THRESHOLD 200 //threshold voltage for the QRDs (decides white from black)
#define CONVERSION -1 //converting between potentiometer and k
 
int error = 0;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hello there");

}

void loop() {
  //constants for PD
  int kp = analogRead(KP_SET)*CONVERSION;
  int kd = analogRead(KD_SET)*CONVERSION;
  int gain = analogRead(GAIN_SET)*CONVERSION;

  double rightVal = analogRead(RIGHT_SENSOR);
  double leftVal = analogRead(LEFT_SENSOR);

  boolean rightOnTape = rightVal > THRESHOLD; //the sensor reads high values on the tape, low off tape
  boolean leftOnTape = leftVal > THRESHOLD;

  //setting error values
  int lasterr = error; //saving the old error value
  
  if(rightOnTape && leftOnTape) error = 0; 
  if(!rightOnTape && leftOnTape)error = -1;
  if(rightOnTape && !leftOnTape)error = 1;
  if(!rightOnTape && !leftOnTape){
    if(error == -1 || error == -5) error = -5;
    else error = 5;
  }

  //steering for error
  steer((kp*error + kd*(error - lasterr))*gain) ;

  //printing k values on LCD -- for debugging purposes
  LCD.clear();  LCD.home() ;
  // the next comment line is used to help identify when lines written to the LCD will be cut off.
  // 16 positions on LCD screen:   1234567890123456
  LCD.setCursor(0,0); LCD.print("kp=%d  err=%d", kp, err);
  LCD.setCursor(0,1); LCD.print("kd=%d  gai=%d", kd, gain);
}

/**
 * steering based on the argument "deg". not really the degrees of turning. just loosely 
 *  approximated. 
 *  
 * should go straight with deg = 0, left with deg < 0, right with deg > 0
 */
void steer(int deg){
  if(deg < 0) setMotorPower(255 - deg, 255);
  else if(deg > 0) setMotorPower(255, 255 - deg);
  else setMotorPower(255, 255);
}

void setMotorPower(int l, int r){
  motor.speed(RIGHT_MOTOR, -r);
  motor.speed(LEFT_MOTOR, -l);
}

