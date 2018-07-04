/**
 * TapeFollower 
 * cpp class for robo_fett V2
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
constexpr int LEFT_SENSOR 10  //digital
constexpr int RIGHT_SENSOR 9  //digital
constexpr int EDGE_SENSOR 8  
constexpr int LEFT_MOTOR 1   //motor port
constexpr int RIGHT_MOTOR 0  //motor port

//motor speeds
constexpr int FULL_F 180
constexpr int HALF_F 120
constexpr int FULL_R -180
constexpr int HALF_R -120

//PD constants
constexpr int KP 16    
constexpr int KD 10    
constexpr int GAIN 23

class TapeFollower {
    public:
    int kp; int kd; int gain;

    TapeFollower(int kpin, int kdin, int gainin){
        kp = kpin;
        kd = kdin;
        gain = gain;
    }

    void tapeFollow(bool lOnTape, bool rOnTape, bool edgeDetect){

    }
};