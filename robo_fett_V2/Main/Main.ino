#include "Robot.h"
#include "Funcs.h"
#include "configs.h"

using namespace std;
Robot roboFett;
//TODO: FIX shitty servo passing
TINAH::Servo RCServo6 = TINAH::Servo(configs::RCSERVO6);
TINAH::Servo RCServo7 = TINAH::Servo(configs::RCSERVO7);

void setup() {
	//startUp sequence
  Serial.begin(9600);
  LCD.begin();
	roboFett = Robot();
	attachInterrupt(2, encoderLeft, RISING);
	attachInterrupt(2, encoderLeft, FALLING);
	attachInterrupt(3, encoderRight, RISING);
	attachInterrupt(3, encoderRight, FALLING);
	pinMode(configs::EWOK_IR_OUT_RIGHT, OUTPUT);
	pinMode(configs::EWOK_IR_OUT_LEFT, OUTPUT);
	LCD.clear();  LCD.home();
	roboFett.sweepServo(BASKET, 20, configs::DRAWBRIDGE_CLOSED);
	roboFett.sweepServo(ARM_RIGHT,configs::ARM_DOWN_EWOK_RIGHT,configs::ARM_UP_RIGHT);
	roboFett.sweepServo(ARM_LEFT,configs::ARM_DOWN_EWOK_LEFT,configs::ARM_UP_LEFT);
	roboFett.sweepServo(CLAW_RIGHT,configs::CLAW_OPEN_RIGHT,configs::CLAW_CLOSED_RIGHT);
	roboFett.sweepServo(CLAW_LEFT,configs::CLAW_OPEN_LEFT,configs::CLAW_CLOSED_LEFT);
}

void loop() {
	switch(roboFett.runState) {
		case STARTUP :
      //Serial.println("MAIN STARTUP");
      LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("MAIN"); 
			roboFett.STARTUP();
			break;
		case CRUISE_PLAT1 :
        //Serial.println("MAIN CRUISE");
        LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("CRUISE");
  			roboFett.CRUISE_PLAT1();
			break;
		case EWOK_SEARCH_RIGHT :
    	LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("SEARCHRIGHT");
			roboFett.EWOK_SEARCH_RIGHT();
			break;
		case EWOK_GRAB :
    	LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("GRABS");
			roboFett.EWOK_GRAB();
			break;
		case DRAWBRIDGE :
		  LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("BRIDGE");
			roboFett.DRAWBRIDGE();
			break;
		case IR_WAIT :
			roboFett.IR_WAIT();
			break;
    case CRUISE_PLAT2 :
      roboFett.CRUISE_PLAT2();
      break;
		case EWOK_SEARCH_LEFT :
    	LCD.clear();LCD.home();
      LCD.setCursor(0,0); LCD.print("SEARCHLEFT");
			roboFett.EWOK_SEARCH_LEFT();
			break;
		case DUMP_PREP :
			roboFett.DUMP_PREP();
			break;
		case DUMP_EWOKS :
			roboFett.DUMP_EWOKS();
			break;
		case FIND_ZIP_PLAT2 :
			roboFett.FIND_ZIP_PLAT2();
			break;
		case ZIP_HOOK :
			roboFett.ZIP_HOOK();
			break;
		case ZIP_UNHOOK :
			roboFett.ZIP_UNHOOK();
			break;
		case EWOK_4 :
			roboFett.EWOK_4();
			break;
		case BRIDGE_FOLLOW :
			roboFett.BRIDGE_FOLLOW();
			break;
		case CHEWIE :
			roboFett.CHEWIE();
			break;
		case ZIP_DOWN :
			roboFett.ZIP_DOWN();
			break;
	}
}

void encoderLeft() {
		int time = millis();
		if(time - roboFett.leftWheelLastTime < 5) {
			return;
		}
		roboFett.leftWheelIndex++;
		roboFett.leftSpeed = configs::mmPerWheelIndex / (time - roboFett.leftWheelLastTime);
		roboFett.leftWheelLastTime = time;
}

void encoderRight() {
		int time = millis();
		if(time - roboFett.rightWheelLastTime < 5) {
			return;
		}
		roboFett.rightWheelIndex++;
		roboFett.rightSpeed = configs::mmPerWheelIndex / (time - roboFett.rightWheelLastTime);
		roboFett.rightWheelLastTime = time;
}

