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
    int kp; int kd; int gain; int error;

    TapeFollower(int kpin, int kdin, int gainin){
        kp = kpin;
        kd = kdin;
        gain = gain;
        error = 0;
    }

    void tapeFollow(bool leftOnTape, bool rightOnTape, bool edgeDetect){
        int lasterr = error; //saving the old error value
  
        if(rightOnTape && leftOnTape){
            if(edgeDetect){
                hardStop(10);
                delay(1000);
                setMotorPower(FULL_R,HALF_R);
                delay(250);
                setMotorPower(HALF_F,FULL_F);
                delay(100);
            }
            error = 0; 
        }
        if(!rightOnTape && leftOnTape)error = -1;
        if(rightOnTape && !leftOnTape)error = 1;
        if(!rightOnTape && !leftOnTape){
            if(error == -1 || error == -5) error = -5;
            else error = 5;
        }

        //steering for error
        steer((kp*error + kd*(error - lasterr))*gain);
    }
};

//ask if ok to have controls in this class?