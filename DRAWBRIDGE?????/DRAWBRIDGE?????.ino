#include "Robot.h"
#include "Funcs.h"
#include "configs.h"

using namespace std;
Robot roboFett;

void setup() {
  Serial.begin(9600);
  LCD.begin();
	roboFett = Robot();
	attachInterrupt(2, encoderLeft, RISING);
	attachInterrupt(3, encoderRight, RISING);
	pinMode(configs::EWOK_IR_OUT, OUTPUT);
	//startUp sequence
  	LCD.clear();  LCD.home() ;
    LCD.setCursor(0,0); LCD.print("HELLOOOO "); 
    roboFett.sweepServo(RCServo2, 20, configs::DRAWBRIDGE_CLOSED);
}

void loop() {
	roboFett.tapeFollowToEdge();
	LCD.clear(); LCD.setCursor(0,0);
	LCD.print("DONETAPE");
	roboFett.setMotorPower(-180,-180);
	delay(600);
	roboFett.setMotorPower(180,180);
	roboFett.setMotorPower(0,0);
	delay(2000);
	roboFett.sweepServo(RCServo2,configs::DRAWBRIDGE_CLOSED,90);
	roboFett.sweepServo(RCServo2,90,configs::DRAWBRIDGE_CLOSED);
	delay(2000);
	roboFett.setMotorPower(-180,-180);
	delay(500);
	roboFett.setMotorPower(0,0);
	delay(1000);
	roboFett.setMotorPower(255,255);
	delay(1500);
	roboFett.hardStop();
	roboFett.findTape();
	while(true) {
		if(roboFett.ewokDetect()) {
			roboFett.hardStop();
			roboFett.pickUp(RIGHT,EWOK);
		}
		roboFett.tapeFollow(TF_KP1, TF_KD1, TF_GAIN1, SPEED);
	}
	delay(50000);
	// switch(roboFett.runState) {
	// 	case STARTUP :
  //     //Serial.println("MAIN STARTUP");
  //     LCD.clear();LCD.home();
  //     LCD.setCursor(0,0); LCD.print("MAIN"); 
	// 		roboFett.STARTUP();
	// 		break;
	// 	case CRUISE_PLAT1 :
  //       //Serial.println("MAIN CRUISE");
  //       LCD.clear();LCD.home();
  //     LCD.setCursor(0,0); LCD.print("CRUISE");
  // 			roboFett.CRUISE_PLAT1();
	// 		break;
	// 	case EWOK_SEARCH :
  //   LCD.clear();LCD.home();
  //     LCD.setCursor(0,0); LCD.print("SEARCH");
	// 		roboFett.EWOK_SEARCH();
	// 		break;
	// 	case EWOK_GRAB :
  //   LCD.clear();LCD.home();
  //     LCD.setCursor(0,0); LCD.print("GRABS");
	// 		roboFett.EWOK_GRAB();
	// 		break;
	// 	case DRAWBRIDGE :
	// 		roboFett.DRAWBRIDGE();
	// 		break;
	// 	case IR_WAIT :
	// 		roboFett.IR_WAIT();
	// 		break;
  //   case CRUISE_PLAT2 :
  //     roboFett.CRUISE_PLAT2();
  //     break;
	// 	case DUMP_PREP :
	// 		roboFett.DUMP_PREP();
	// 		break;
	// 	case DUMP_EWOKS :
	// 		roboFett.DUMP_EWOKS();
	// 		break;
	// 	case FIND_ZIP_PLAT2 :
	// 		roboFett.FIND_ZIP_PLAT2();
	// 		break;
	// 	case ZIP_HOOK :
	// 		roboFett.ZIP_HOOK();
	// 		break;
	// 	case ZIP_UNHOOK :
	// 		roboFett.ZIP_UNHOOK();
	// 		break;
	// 	case EWOK_4 :
	// 		roboFett.EWOK_4();
	// 		break;
	// 	case BRIDGE_FOLLOW :
	// 		roboFett.BRIDGE_FOLLOW();
	// 		break;
	// 	case CHEWIE :
	// 		roboFett.CHEWIE();
	// 		break;
	// 	case ZIP_DOWN :
	// 		roboFett.ZIP_DOWN();
	// 		break;
	// }
}

void encoderLeft() {
		int time = millis();
		if(time - roboFett.leftWheelLastTime < 40) {
			return;
		}
		roboFett.leftWheelIndex++;
		roboFett.leftWheelLastTime = time;
}

void encoderRight() {
		int time = millis();
		if(time - roboFett.rightWheelLastTime < 40) {
			return;
		}
		roboFett.rightWheelIndex++;
		roboFett.rightWheelLastTime = time;
}
