#include <phys253.h>          
#include <LiquidCrystal.h> 

  const int ARM_UP_RIGHT = 40;
  const int ARM_DOWN_EWOK_RIGHT = 180;
  const int ARM_DOWN_CHEWIE_RIGHT = 150;
  const int ARM_UP_LEFT = 140; 
  const int ARM_DOWN_EWOK_LEFT = 0;
  const int ARM_DOWN_CHEWIE_LEFT = 30;
  const int CLAW_CLOSED_RIGHT = 0;
  const int CLAW_OPEN_RIGHT = 180;
  const int CLAW_CLOSED_LEFT = 180;
  const int CLAW_OPEN_LEFT = 0;

  const int RCSERVO7 = 12;
  const int RCSERVO6 = 11;

  const int BASKET_REST = 115;
  const int BASKET_DUMP = 0;
  const int BASKET_DROPBRIDGE = 80;
  
  //can just copy and paste entire configs file to keep updated 

  TINAH::Servo RCServo7(RCSERVO7);
  TINAH::Servo RCServo6(RCSERVO6);

  #define ARM_RIGHT RCServo0
  #define CLAW_RIGHT RCServo1
  #define ARM_LEFT RCServo7
  #define CLAW_LEFT RCServo6
  #define BASKET RCServo2

//runs through servos to make sure everything is in right positions 
//use tinah rcservo0 and rcservo1, can only do one side at once

void setup()
{  
  #include <phys253setup.txt>
  Serial.begin(9600) ;
}

void loop() {

  //start by testing basket
  //starts at basket resting, then into dropbridge, then dump, then back to rest
  
  BASKET.write(BASKET_REST);  
  LCD.setCursor(0,0);
  LCD.print("Basket rest");

  while(!(startbutton())){delay(100);}
  
  sweepServo(BASKET, BASKET_REST, BASKET_DROPBRIDGE);
  LCD.setCursor(0,0);
  LCD.print("dropbridge");

  while(!(startbutton())){delay(100);}  
  

  sweepServo(BASKET, BASKET_DROPBRIDGE, BASKET_DUMP);
  LCD.setCursor(0,0);
  LCD.print("dumping");

   while(!(startbutton())){delay(100);} 

  sweepServo(BASKET, BASKET_DUMP, BASKET_REST);
  delay(100);

  BASKET.detach();
  
  //now right side
  //arm starts at bottom, then goes up, then goes down and closes and opens claws
  
  ARM_RIGHT.write(ARM_DOWN_EWOK_RIGHT);
  LCD.setCursor(0,0);
  LCD.print("R Arm Down");

  while(!(startbutton())){delay(100);} 

  sweepServo(ARM_RIGHT, ARM_DOWN_EWOK_RIGHT, ARM_UP_RIGHT);
  LCD.setCursor(0,0);
  LCD.print("R Arm Up");

  while(!(startbutton())){delay(100);} 

  sweepServo(ARM_RIGHT, ARM_UP_RIGHT, ARM_DOWN_EWOK_RIGHT);

  CLAW_RIGHT.write(CLAW_CLOSED_RIGHT);
  LCD.setCursor(0,0);
  LCD.print("R Claw closed");

  while(!(startbutton())){delay(100);}

  sweepServo(CLAW_RIGHT, CLAW_CLOSED_RIGHT, CLAW_OPEN_RIGHT);
  LCD.setCursor(0,0);
  LCD.print("R Claw Open");

  while(!(startbutton())){delay(100);}

  //now left claw

  ARM_LEFT.write(ARM_DOWN_EWOK_LEFT);
  LCD.setCursor(0,0);
  LCD.print("L Arm Down");

  while(!(startbutton())){delay(100);} 

  sweepServo(ARM_LEFT, ARM_DOWN_EWOK_LEFT, ARM_UP_LEFT);
  LCD.setCursor(0,0);
  LCD.print("L Arm Up");

  while(!(startbutton())){delay(100);} 

  sweepServo(ARM_LEFT, ARM_UP_LEFT, ARM_DOWN_EWOK_LEFT);i

  CLAW_LEFT.write(CLAW_CLOSED_LEFT);
  LCD.setCursor(0,0);
  LCD.print("L Claw closed");

  while(!(startbutton())){delay(100);}

  sweepServo(CLAW_LEFT, CLAW_CLOSED_LEFT, CLAW_OPEN_LEFT);
  LCD.setCursor(0,0);
  LCD.print("L Claw Open");

  while(!(startbutton())){delay(100);}

  LCD.setCursor(0,0);
  LCD.print("donzo");

  while(!(startbutton())){delay(100);}
      
}

void sweepServo(TINAH::Servo servo, int startAngle, int endAngle) {
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
