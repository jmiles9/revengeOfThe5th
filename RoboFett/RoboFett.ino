#include "Robot.cpp"

//reference https://github.com/WalkervilleElementary/robot/blob/master/src/main.cpp

//will need to do imports for all other stuff
//need to be very mindful of ints being used!

using namespace std;

Robot roboFett;

void setup() {
	roboFett.ewoksSaved = 0;
	//always assume every ewok is saved even if we try to pick up and can't get
	roboFett.cmTravelled = 0;
	roboFett.state = runState::CruisePlat1;

	//startUp sequence

}

void loop() {
	roboFett.run();
	//change this to a switch function once able
}