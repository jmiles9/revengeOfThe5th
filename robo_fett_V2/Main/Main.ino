#include "Robot.h"
#include "Funcs.h"
#include "configs.h"

//reference https://github.com/WalkervilleElementary/robot/blob/master/src/main.cpp

//will need to do imports for all other stuff
//need to be very mindful of ints being used!

using namespace std;


Robot roboFett;

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  LCD.begin();
	roboFett = Robot();
	//attachInterrupt(3, encoderLeft, RISING);
	//attachInterrupt(4, encoderRight, RISING);
	//startUp sequence
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("HELLOOOO "); 
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
		case EWOK_SEARCH :
			roboFett.EWOK_SEARCH();
			break;
		case EWOK_GRAB :
			roboFett.EWOK_GRAB();
			break;
		case DRAWBRIDGE :
			roboFett.DRAWBRIDGE();
			break;
		case IR_WAIT :
			roboFett.IR_WAIT();
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
		roboFett.leftWheelIndex++;
		int time = millis();
		roboFett.leftSpeed = configs::cmPerWheelIndex / (time - roboFett.leftWheelLastTime);
		roboFett.leftWheelLastTime = time;
}

void encoderRight() {
		roboFett.rightWheelIndex++;
		int time = millis();
		roboFett.rightSpeed = configs::cmPerWheelIndex / (time - roboFett.rightWheelLastTime);
		roboFett.rightWheelLastTime = time;
}

