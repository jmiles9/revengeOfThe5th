#include "Robot.h"
#include "Funcs.h"
#include "configs.h"

//reference https://github.com/WalkervilleElementary/robot/blob/master/src/main.cpp

//will need to do imports for all other stuff
//need to be very mindful of ints being used!

using namespace std;


Robot roboFett;
//TODO: FIX shitty servo passing

void setup() {
	//startUp sequence
  Serial.begin(9600);
  Serial.println("setup");
  LCD.begin();
	roboFett = Robot();
	attachInterrupt(3, encoderLeft, CHANGE);
	attachInterrupt(2, encoderRight, CHANGE);
	pinMode(0, INPUT);
  pinMode(1, INPUT); //dont set 2 and 3 cuz interrupts
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
	pinMode(7, INPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(14, OUTPUT);
	pinMode(15, OUTPUT);
	LCD.clear();  LCD.home();
}

void loop() {
	switch(roboFett.runState) {
		case STARTUP :
      Serial.println("MAIN STARTUP");
			roboFett.STARTUP();
			break;
		case CRUISE_PLAT1 :
        Serial.println("MAIN CRUISE");
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
		case ZIP_HOOK :
			roboFett.ZIP_HOOK();
			break;
		case FIND_ZIP_PLAT2 :
			roboFett.FIND_ZIP_PLAT2();
			break;
		case ZIP_UP :
			roboFett.ZIP_UP();
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
		case SAVE_CHEWIE :
			roboFett.SAVE_CHEWIE();
			break;
		case ZIP_DOWN :
			roboFett.ZIP_DOWN();
			break;
		case PICKUP_THIRD :
			roboFett.PICKUP_THIRD();
			break;
	}
}

/// MARK: Encoder interrupt functions
void encoderRight() {
    int time = millis();
    if(time - roboFett.rightWheelLastTime < 5) {
        return;
    }
    roboFett.rightWheelIndex++;
		roboFett.rightSpeed = configs::umPerWheelIndex / (time - roboFett.rightWheelLastTime);
		roboFett.rightWheelLastTime = time;
}

void encoderLeft() {
	int time = millis();
    if(time - roboFett.leftWheelLastTime < 5) {
        return;
    }
    roboFett.leftWheelIndex++;
		roboFett.leftSpeed = configs::umPerWheelIndex / (time - roboFett.leftWheelLastTime);
		roboFett.leftWheelLastTime = time;
}
