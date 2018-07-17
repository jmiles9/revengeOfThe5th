#include "Robot.h"
#include "Menu.h"

using namespace configs;

class Robot : public Funcs {

public:
	
	// Constructor
	ROBOT::Robot() {
		state = STARTUP;
        nextEwok = 1;
		leftWheelIndex = 0;
        rightWheelIndex = 0;
		cruiseIndex = 0;
		Menu menu;
	}

	void STARTUP() {
		//TODO: Add button to begin - ie. from menu 
		while(!menu.quitMenu){
			bool start = false;
   			bool stopp = false;
			   
    		while(startbutton()){start = true;}
    		if(start) menu.handleInput(BTN_START);

    		while(stopbutton()){stopp = true;}
    		if(stopp) menu.handleInput(BTN_STOP);
		}
		state = CRUISE;
	}

	/// Tape follows until reaching the first gap.
	void CRUISE_PLAT1() {
		while() {
			if(isOnEdge()) {
				stop()
				state = DRAWBRIDGE;
				break;
			}
			tapeFollow(PROPORTIONAL1,DERIVITIVE1,TAPEFOLLOW_GAIN1,TAPEFOLLOW_SPEED1);
		}
	}

	/// Lowers drawbridge then backs up.
	void DRAWBRIDGE() {
		lowerBridge();
		tapeFollowForDistance(-10);
	}

	// TODO: write
	// look for extrema in value?
	void EWOK_SEARCH() {

	}

	/// Grabs ewok. 
	/// If fails, attemps twice more, once a little in front, once behind.
	void EWOK_GRAB() {
		int side;
		int stuffy;
		// Gets side and stuffy parameters
		// 0 = left, ewok
		// 1 = right, chewie
		if(nextEwok == 3) {
			side = 0;
			stuffy = 0;
		} else if(nextEwok == 5) {
			side = 0;
			stuffy = 1;
		} else {
			side = 1;
			stuffy = 0;
		}

		// If don't get ewok, try again on either side
		int count = 1;
		while(count <= 3) {
			if(pickUp(side, stuffy)) {
				break;
			}
			if count == 1 {
            	tapeFollowForDistance(10);
			} else {
				tapeFollowForDistance(-10);
			}
			count++;
		}
		// Update ewok count
		nextEwok++;
	}

	/// Follows tape until IR signals are strong.
	void CRUISE_IR() {
		while(record10KIRBeacon() < IR_THRESHOLD && record1KIRBeacon < IR_THRESHOLD) {
			tapeFollow(PROPORTIONAL1, DERIVITIVE1, TAPEFOLLOW_GAIN1, TAPEFOLLOW_SPEED1);
		}
		stop();
	}
	// TODO: Write
	void IR_WAIT() {

	}
	//TODO: Write
	void CRUISE_PLAT2() {

	}

	// TODO: Write
	void DUMP_PREP() {

	}
	void DUMP_EWOKS() {
		dumpBasket();
	}
	// TODO: Write
	void ZIP_HOOK() {

	}
	//TODO: WRite
	void ZIP_UP() {

	}
	// TODO: Write
	void ZIP_UNHOOK() {

	}
	// TODO: Write
	void EWOK_4() {

	}
	// TODO: Write
	void BRIDGE_FOLLOW() {

	}
	// TODO: WRite
	void CHEWIE() {

	}
	// TODO: Write
	void ZIP_DOWN() {

	}

	void cruisePlat2_1() {

		//just dropped bridge

		uint16_t initialDist = cmTravelled;

		while ((cmTravelled - initialDist) < 69) {
			tapeFollow();
		}

		state = runState::EwokPickup;
	}

	void IRHandle() {

		//move until IR signal is strong?

		while (!(readIR())) {
		}

		state = runState::CruisePlat2_2;

	}
}