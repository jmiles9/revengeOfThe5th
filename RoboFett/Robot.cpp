#include <phys253.h>

enum class runState {
	Idle,
	CruisePlat1,
	EwokPickup,
	Bridge,
	CruisePlat2_1,
	CruisePlat2_2,
	IRHandle,
	ZiplineUp,
	BridgeCross,
	Done
};

class Robot {

public:

	//define fields
	runState state;
	int8_t ewoksSaved;
	uint16_t cmTravelled;
	bool BRIDGEDROPPED;
	bool DEBUG;

	void tapeFollow() {

		if (startButton()) {

		}

		//takes in dist travlled from encoders
	}

	void findEdge() {

	}

	void dropBridge() {
		//end with
		BRIDGEDROPPED = true;
	}

	void grab(String stuffy) {
		//starts and ends in upright position
		//decline angle is the angle to pick stuff up at, will be different for ewoks vs chewie

		int8_t pickupAngle;

		if ((stuffy == "Ewok") || (stuffy == "ewok")) {
			pickupAngle = -69;
			//CHANGE ANGLES
		}
		else {
			pickupAngle = -69;
		}

	}

	void alignToEwok() {

	}

	bool readIR() {

		//needs freq to be same for some time increment
		//returns true if frequency is 10kHz

	}

	void goDistance(uint8_t dist) {
		//calls tapeFollow
	}

	void getAngle() {
		//reads gyro angle
	}

	//might be error not doing PlatformState::Platform1 ??
	void run() {
		while (state != runState::Done) {
			if (state == runState::CruisePlat1) {
				cruisePlat1();
			}
			if (state == runState::EwokPickup) {
				ewokPickup();
			}
			if (state == runState::Bridge) {
				bridge();
			}
			if (state == runState::CruisePlat2_1) {
				cruisePlat2_1();
			}
			if (state == runState::IRHandle) {
				IRHandle();
			}
			if (state == runState::CruisePlat2_2) {
				cruisePlat2_2();
			}
			//WOULD NEED TO ADD SOMETHING OUTSIDE OF HERE FOR IF IT COMPLETED THE ENTIRE THING
			LCD.setCursor(0, 0);
			LCD.print("win");
		}
	}

	void cruisePlat1() {

		uint16_t initialDist = cmTravelled;

		//NEED TO CHANGE VALUE
		while ((cmTravelled - initialDist) < 69) {
			tapeFollow();
		}

		state = runState::EwokPickup;
	}

	void ewokPickup() {

		alignToEwok();

		//could possible use some code around for cases where it is not picked up, 
		//probably want to try ~3 times then give up
		grab("Ewok");

		ewoksSaved++;

		if (ewoksSaved == 1) {
			state = runState::Bridge;
		}
		if (ewoksSaved == 2) {
			state = runState::IRHandle;
		}
		if (ewoksSaved == 3) {
			state = runState::ZiplineUp;
		}
		if (ewoksSaved == 4) {
			state = runState::BridgeCross;
		}
	}

	void bridge() {

		findEdge();
		dropBridge();
		BRIDGEDROPPED == true;

		state = runState::CruisePlat2_1;

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

	void cruisePlat2_2() {

		//needs to go real fast at start?

		uint16_t initialDist = cmTravelled;

		//or could possibly do until edge is detected? probs better idea
		while ((cmTravelled - initialDist) < 69) {
			tapeFollow();
		}


	}

private:

};