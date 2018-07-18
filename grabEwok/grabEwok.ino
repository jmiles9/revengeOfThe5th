#include <phys253.h>          
#include <LiquidCrystal.h>     

#define ROTATE_SERVO 0
#define CLOSE_SERVO 1
#define CLAW_ARM_RAISED_POSITION 95 
#define CLAW_ARM_LOWERED_POSITION 0
#define CLAW_TONG_CLOSED_POSITION 0
#define CLAW_TONG_OPEN_POSITION 120

int clawArmPosition;
int clawTongPosition;

void setup()
{  
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  clawArmPosition = CLAW_ARM_RAISED_POSITION;
  clawTongPosition = CLAW_TONG_CLOSED_POSITION;
}


void loop() {
 
 }

void grabEwok(){
  //assume starts vertical, closed
  //RCServo0 is rotation arm
  //RCServo1 is claw tong

  RCServo0.write(CLAW_ARM_LOWERED_POSITION);
  delay(100);
  RCServo1.write(CLAW_TONG_CLOSED_POSITION);
  delay(150);
  RCServo0.write(CLAW_ARM_RAISED_POSITION);
  delay(100);
  RCServo1.write(CLAW_TONG_OPEN_POSITION);
  delay(50);
  RCServo1.write(CLAW_TONG_CLOSED_POSITION);
  
}
