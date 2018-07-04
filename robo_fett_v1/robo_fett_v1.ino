/**
 * Version 2 of tape follower code - with PD 
 * this version has set kp/kd vals and uses analog QRDs
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
#define LEFT_SENSOR 10  //digital
#define RIGHT_SENSOR 9  //digital
#define EDGE_SENSOR 8  
#define LEFT_MOTOR 1   //motor port
#define RIGHT_MOTOR 0  //motor port

//motor speeds
#define FULL_F 180
#define HALF_F 120
#define FULL_R -180
#define HALF_R -120

//PD constants
#define KP 16    
#define KD 10    
#define GAIN 23

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hello there");
}

int count;
int error = 0;

void loop() {
  tapeFollow(16,7,10);
}


//MAIN FUNCTIONS/////////////////////////////////////////////////////////////////////

/**
 * tape following - reads sensor values, sets motor speeds using pd
 * param: kp = proportional constant
 *        kd = derivative constant
 *        gain = gain for pd
 */
void tapeFollow(int kp, int kd, int gain){
  boolean rightOnTape = digitalRead(RIGHT_SENSOR);
  boolean leftOnTape = digitalRead(LEFT_SENSOR);
  boolean edgeDetect = digitalRead(EDGE_SENSOR);

  //setting error values
  int lasterr = error; //saving the old error value
  
  if(rightOnTape && leftOnTape){
    if(edgeDetect){
      hardStop(10);
      delay(1000);
      setMotorPower(FULL_R,HALF_R);
      delay(250);
      setMotorPower(HALF_F,FULL_F);
      delay(100);
    }
    error = 0; 
  }
  if(!rightOnTape && leftOnTape)error = -1;
  if(rightOnTape && !leftOnTape)error = 1;
  if(!rightOnTape && !leftOnTape){
    if(error == -1 || error == -5) error = -5;
    else error = 5;
  }

  //steering for error
  steer((kp*error + kd*(error - lasterr))*gain) ;

  if(count > 10){
    //printing k values on LCD -- for debugging purposes
    LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("left= "); LCD.print(leftOnTape); LCD.print("righ= "); LCD.print(rightOnTape);
    LCD.setCursor(0,1); LCD.print("edge= "); LCD.print(edgeDetect);
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
    if(deg > FULL_F*2) deg = FULL_F*2;
    setMotorPower(FULL_F - deg, FULL_F);
    Serial.println("deg > 0");
    Serial.println(deg);
  }
  else if(deg < 0){
    if(-deg > FULL_F*2) deg = -FULL_F*2;
    Serial.println("deg < 0");
    Serial.println(deg);
    setMotorPower(FULL_F, FULL_F + deg);
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
  motor.speed(RIGHT_MOTOR, r);
  motor.speed(LEFT_MOTOR, l);
}
