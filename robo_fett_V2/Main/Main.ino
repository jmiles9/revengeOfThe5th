#include "Robot.h"

//reference https://github.com/WalkervilleElementary/robot/blob/master/src/main.cpp

//will need to do imports for all other stuff
//need to be very mindful of ints being used!

using namespace std;

robot::Robot roboFett;

void setup() {
	roboFett = robot::Robot();
	attachInterrupt(3, encoderLeft, RISING);
	attachInterrupt(4, encoderRight, RISING);
	//startUp sequence
}

void loop() {
	switch(roboFett.state) {
		case STARTUP :
			roboFett.STARTUP();
			break;
		case CRUISE_PLAT1 :
			roboFett.CRUISE();
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

	void encoderLeft() {
  		    roboFett.leftWheelIndex++;
			time = millis();
			roboFett.leftSpeed = cmPerWheelIndex / (time - roboFett.leftWheelLastTime);
            roboFett.leftWheelLastTime = time;
	}

	void encoderRight() {
  		    roboFett.rightWheelIndex++;
			time = millis();
			roboFett.rightSpeed = cmPerWheelIndex / (time - roboFett.rightWheelLastTime);
            roboFett.rightWheelLastTime = time;
	}
}