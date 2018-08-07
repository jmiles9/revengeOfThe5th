#include <phys253.h>          
#include <LiquidCrystal.h>

#define MOTORSCREW 0  //motor port 0
#define MOTORZIP 1
#define DIRECTIONSCREW 0
#define DIRECTIONZIP 1
#define SPEEDSCREW 2
#define SPEEDZIP 3
#define LOWERSCREWLIM 15
#define UPPERSCREWLIM 12
#define DOWN 0
#define UP 1
#define STOP 0
 

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home();
}

void loop() {
  impressAndre();
  delay(50000000);
}

void impressAndre() {
  //go down
  motor.speed(MOTORSCREW,-255);
  while(true) {
    if(digitalRead(LOWERSCREWLIM) == STOP) {
      motor.stop(MOTORSCREW);
      break;
    }
  }
  delay(500);
  motor.speed(MOTORZIP,255);
  delay(14000);
  motor.stop(MOTORZIP);
  motor.speed(MOTORSCREW,255);
  while(true) {
    if(digitalRead(UPPERSCREWLIM) == STOP) {
      break;
    }
  }
  motor.stop(MOTORSCREW);
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
}


