/**
 * Version 2 of tape follower code - with PD 
 * this version has set kp/kd vals
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
#define LEFT_SENSOR 5  //analog port 3 (they are reversed on board!!)
#define RIGHT_SENSOR 0 //analog port 7
#define EDGE_SENSOR 4  
#define LEFT_MOTOR 3   //motor port 3
#define RIGHT_MOTOR 2  //motor port 2

//motor speeds
#define FULL_F 255
#define HALF_F 200
#define FULL_R -255
#define HALF_R -200

//PD constants
#define KP 16    
#define KD 10    
#define GAIN 23    

#define THRESHOLD 200 //threshold voltage for the QRDs (decides white from black)
#define CONVERSION 0.1 //converting between potentiometer and k
 
int error = 0;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hello there");

}

int count = 0;

void loop() {
  double rightVal = analogRead(RIGHT_SENSOR);
  double leftVal = analogRead(LEFT_SENSOR);
  double edgeVal = analogRead(EDGE_SENSOR);

  boolean rightOnTape = rightVal > THRESHOLD; //the sensor reads high values on the tape, low off tape
  boolean leftOnTape = leftVal > THRESHOLD;
  boolean edgeDetect = edgeVal > THRESHOLD;

  //setting error values
  int lasterr = error; //saving the old error value
  
  if(rightOnTape && leftOnTape){
    if(edgeDetect) hardStop();
    error = 0; 
  }
  if(!rightOnTape && leftOnTape)error = -1;
  if(rightOnTape && !leftOnTape)error = 1;
  if(!rightOnTape && !leftOnTape){
    if(error == -1 || error == -5) error = -5;
    else error = 5;
  }

  //steering for error
  steer((KP*error + KD*(error - lasterr))*GAIN) ;

  if(count > 20){
    //printing k values on LCD -- for debugging purposes
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("left= "); LCD.print(leftOnTape); LCD.print("righ= "); LCD.print(rightOnTape);
    LCD.setCursor(0,1); LCD.print("edge= "); LCD.print(edgeDetect);
  }
  count ++;
}

/**
 * steering based on the argument "deg". not really the degrees of turning. just loosely 
 *  approximated. 
 *  
 * should go straight with deg = 0, left with deg > 0, right with deg < 0
 */
void steer(int deg){
  if(deg > 0){
    setMotorPower(255 - deg, 255);
    Serial.println("deg > 0");
    Serial.println(deg);
  }
  else if(deg < 0){
    Serial.println("deg < 0");
    Serial.println(deg);
    setMotorPower(255, 255 + deg);
  }
  else{
    Serial.println("deg = 0");
    Serial.println(deg);
    setMotorPower(255, 255);
  }
}

void hardStop(){
  //slam on brakes
  setMotorPower(FULL_R, FULL_R);
  delay(10);
  setMotorPower(0,0);
  delay(5000);
}

void setMotorPower(int l, int r){
  motor.speed(RIGHT_MOTOR, -r);
  motor.speed(LEFT_MOTOR, -l);
}

