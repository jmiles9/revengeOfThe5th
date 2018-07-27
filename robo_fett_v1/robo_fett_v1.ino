/**
 * Version 2 of tape follower code - with PD 
 * this version has set kp/kd vals and uses analog QRDs
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
    const int ENCODER_LEFT = 2;
    const int ENCODER_RIGHT = 3;
    const int EDGE_QRD = 6;
    const int TAPE_QRD_LEFT = 7;
    const int TAPE_QRD_RIGHT = 5;  
    const int RIGHT_CLAW_STUFFY_SWITCH = 5;
    const int LEFT_MOTOR = 0;
    const int RIGHT_MOTOR = 1;

//motor speeds
#define FULL_F 150
#define HALF_F 120
#define FULL_R -150
#define HALF_R -120

//PD constants
#define KP 16    
#define KD 10    
#define GAIN 23

#define EWOK_THRESH 30

#define ROTATE_SERVO 0
#define CLOSE_SERVO 1
#define CLAW_ARM_RAISED_POSITION 0 
#define CLAW_ARM_LOWERED_POSITION 115
#define CLAW_TONG_CLOSED_POSITION 0
#define CLAW_TONG_OPEN_POSITION 110

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hello there");
  RCServo0.write(CLAW_ARM_RAISED_POSITION);
  RCServo1.write(CLAW_TONG_CLOSED_POSITION);
  attachInterrupt(2, encoderLeft, RISING);
  attachInterrupt(3, encoderRight, RISING);
  
}

int count;
int error = 0;

void loop() {
  tapeFollow(9,12,10);
}


//MAIN FUNCTIONS/////////////////////////////////////////////////////////////////////
/**
 * grab ewok - grabs the ewok
 */
void grabEwok(){
  //assume starts vertical, closed
  //RCServo0 is rotation arm
  //RCServo1 is claw tong

  RCServo0.write(CLAW_ARM_LOWERED_POSITION);
  delay(1000);
  RCServo1.write(CLAW_TONG_CLOSED_POSITION);
  delay(1500);
  RCServo0.write(CLAW_ARM_RAISED_POSITION);
  delay(2000);
  RCServo1.write(CLAW_TONG_OPEN_POSITION);

  
}
 
/**
 * tape following - reads sensor values, sets motor speeds using pd
 * param: kp = proportional constant
 *        kd = derivative constant
 *        gain = gain for pd
 */
void tapeFollow(int kp, int kd, int gain){
  boolean rightOnTape = digitalRead(TAPE_QRD_RIGHT);
  boolean leftOnTape = digitalRead(TAPE_QRD_LEFT);
  boolean edgeDetect = digitalRead(EDGE_QRD);
  //boolean edgeDetect = false; //this turns off edge detecting for testing purposes

  //setting error values
  int lasterr = error; //saving the old error value
  
  if(rightOnTape && leftOnTape){
    if(edgeDetect){
      hardStop(10);
      LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("AAHH");
      delay(1000);
    }
    error = 0; 
  }
  if(!rightOnTape && leftOnTape)error = -1;
  if(rightOnTape && !leftOnTape)error = 1;
  if(!rightOnTape && !leftOnTape){
    if(error == -1 || error == -5) error = -5;
    else error = 5;
  }

  if(error == 5) {
    error = -0.5;
  }

  //steering for error
  steer((kp*error + kd*(error - lasterr))*gain) ;

  if(count > 10){
    //printing k values on LCD -- for debugging purposes
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("left= "); LCD.print(leftOnTape); LCD.print("righ= "); LCD.print(rightOnTape);
    LCD.setCursor(0,1); LCD.print("edge= "); LCD.print(edgeDetect); LCD.print("err = "); LCD.print(error);
    count = 0;
  }
  count ++;
}


//OTHER FUNCTIONS//////////////////////////////////////////////////////////////////////////////
/**
 * steering based on the argument "deg". not really the degrees of turning. just loosely 
 *  approximated. 
 *  
 * should go straight with deg = 0, left with deg > 0, right with deg < 0
 */
void steer(int deg){
  if(deg > 0){
    if(deg > FULL_F) deg = FULL_F;
    setMotorPower(FULL_F, FULL_F - deg);
    Serial.println("deg > 0");
    Serial.println(deg);
  }
  else if(deg < 0){
    if(-deg > FULL_F) deg = -FULL_F;
    Serial.println("deg < 0");
    Serial.println(deg);
    setMotorPower(FULL_F + deg, FULL_F);
  }
  else{
    Serial.println("deg = 0");
    Serial.println(deg);
    setMotorPower(FULL_F, FULL_F);
  }
}

/**
 * hard stop - briefly slams motors into reverse and then comes to a 
 *  complete stop for t milliseconds
 */
void hardStop(int t){
  //slam on brakes
  setMotorPower(FULL_R, FULL_R);
  delay(10);
  setMotorPower(0,0);
  delay(t);
}

void setMotorPower(int l, int r){
//  motor.speed(RIGHT_MOTOR, -r);
//  motor.speed(LEFT_MOTOR, l);
}

void encoderLeft() {
    int time = millis();
}

void encoderRight() {
    int time = millis();
}
