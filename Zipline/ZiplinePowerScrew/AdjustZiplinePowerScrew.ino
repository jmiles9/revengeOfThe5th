#include <phys253.h>          
#include <LiquidCrystal.h>

 //motor speeds
#define FULL_F 255
#define HALF_F 200
#define FULL_R -255
#define HALF_R -200
#define MOTORSCREW 0  //motor port 0
#define MOTORZIP 1
#define DIRECTIONSCREW 0
#define DIRECTIONZIP 1
#define SPEEDSCREW 2
#define SPEEDZIP 3
#define LOWERSCREWLIM 15
#define DOWN 0
#define UP 1
#define STOP 0
 

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home();
}

void loop() {
  setScrewMotorSpd();
}

void setScrewMotorSpd(){
  int spd = analogRead(SPEEDSCREW);
  int dir = digitalRead(DIRECTIONSCREW);
  int limitSwitch = digitalRead(LOWERSCREWLIM);

  int setspd = map(spd, 0, 1023, 0, 255);

  if(dir == DOWN){
    setspd = -setspd;
  }
  if(dir == DOWN) {
    if(limitSwitch == STOP) {
      motor.stop(MOTORSCREW);
      return;
    }
  }
  motor.speed(MOTORSCREW,setspd);
  
//
//  Serial.println("spd");
//  Serial.println(setspd);
//  
//  motor.speed(MOTORSCREW, setspd);
//
//  //Read Limit Switches
//
//
//  if (lz == 0){
//    // lower limit has been hit
//    screwLowerLim = 0;
//    return 0;
//  }
//  return 1;

}


  


