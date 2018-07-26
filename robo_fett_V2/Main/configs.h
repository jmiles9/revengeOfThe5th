#include <phys253.h>  

#ifndef CONFIGS_H
#define CONFIGS_H

namespace configs {

    //overall use
    #define DEBUG() 0
    #define LOOP_DELAY() 10

    // let's put all ports here, then value-based constants in the specific categories
    // eg. MOTOR_SPEED_FAST

    // Analog ports
    // KNOB is using port 0. keep in mind they are reversed on TINAH
    const int IR_1KHZ = 1;
    const int IR_10KHZ = 2;
    const int EWOK_SENSOR = 3;
    
    // Digital ports
    // pls go backwards. don't use interrupt ports (0-3) for switches
    
    //multiplexer inputs 16 to 23 (0 on multiplexer is 16 for here)
    //all must be inputs
       
    //tinah inputs 15 to 8
    //must be outputs

    const int EWOK_IR_OUT = 8; //when HI, the ewok IR
    
    //tinah inputa 0 to 7 (including interrupts)
    //must be inputs cuz interrupts

    const int ENCODER_LEFT = 0;
    const int ENCODER_RIGHT = 1;
    const int EDGE_QRD = 2;
    const int TAPE_QRD_LEFT = 3;
    const int TAPE_QRD_RIGHT = 4;  
    const int RIGHT_CLAW_STUFFY_SWITCH = 5;
    
    //all ports below arbitrary

    const int LEFT_CLAW_STUFFY_SWITCH = 15;
    const int ZIP_SWITCH_EXTENDED = 13;
    const int ZIP_SWITCH_CLOSED = 12;
    const int ZIPPED_UP_SWITCH = 11;

    const int RCSERVO7 = 7;
    const int RCSERVO6 = 6;
    const int BRIDGE_QRD_RIGHT = 5;
    const int BRIDGE_QRD_LEFT = 4;

    // Servo ports
    
    #define ARM_RIGHT RCServo0
    #define CLAW_RIGHT RCServo1
    #define ARM_LEFT RCServo7
    #define CLAW_LEFT RCServo6
    #define BASKET RCServo2

    // Motor ports
    const int LEFT_MOTOR = 0;
    const int RIGHT_MOTOR = 1;
    const int ZIP_ARM_MOTOR = 2;
    const int ZIP_WHEEL_MOTOR = 3;

    //drivetrain
    #define LEFT_HANDICAP
    #define RIGHT_HANDICAP
    #define ENCODER_WHEEL_RATIO
    const float wheelRadius = 3;
    const float cmPerWheelIndex = wheelRadius * 3.1415 * 2 / 6;
    #define WHEEL_DIAM
    const float wheelSeparation = 17.5;
    // amount of rotation if left and right wheels move in opposite directions
    const float degreesPerCm = 360 / (3.1415 * wheelSeparation);


    // Speeds
    const int FULL_F = 255;
    const int HALF_F = 220;
    const int FULL_R = -255;
    const int HALF_R = -220;

    #define TF_KP1 16
    #define TF_KD1 10
    #define TF_SPEED1 0
    #define TF_SPEED2 0
    #define TF_GAIN1 10
    #define TF_GAIN2 0
    const int ZIP_ARM_EXTENDING = 255;
    const int ZIP_ARM_CONTRACTING = -255;
    const int ZIPPING_UP = 255;
    // in cm/s
    const int MAX_SPEED = 50;

    enum Speed{
        SPEED
    };

    //servo stuff
    const int ROTATE_SERVO = 0;
    const int CLOSE_SERVO = 1;
    const int CLAW_ARM_RAISED_POSITION = 0; 
    const int CLAW_ARM_LOWERED_POSITION = 115;
    const int CLAW_TONG_CLOSED_POSITION = 0;
    const int CLAW_TONG_OPEN_POSITION = 110;

    //IR
    const int IR_THRESHOLD = 512;
    const int EWOK_THRESH = 650;
    //digitalOut

    //claws


    //distances to move

    const int DIST_CONV = 2;
    
    const int STUFFY_GRAB_MANEUVER = 10/DIST_CONV;

    const int PLAT1_CRUISE = 30/DIST_CONV;
    const int PRE_BRIDGE_MOVE = 5/DIST_CONV;
    const int BRIDGE_REVERSE = -10/DIST_CONV;
    const int BRIDGE_CRUISE = 60/DIST_CONV;
    const int PLAT2_CRUISE = 130/DIST_CONV;
    const int DUMP_PREP_DIST = 15/DIST_CONV;
    const int DUMP_RAM_DISTANCE = 10/DIST_CONV;
    

    // Angles
    const int ARMS_UP = 120;
    const int ARMS_DOWN_EWOK = 0;
    const int ARMS_DOWN_CHEWIE = 30;
    const int CLAWS_CLOSED = 90;
    const int CLAWS_OPEN = 90;
    const int DRAWBRIDGE_OPENED = 90;
    const int DRAWBRIDGE_CLOSED = 0;
    const int BASKET_OPENED = 90;
    const int BASKET_DROPBRIDGE = 45;
    const int BASKET_CLOSED = 0;
    const int ZIPLINE_ATTACH_ROTATION = 15;
    const int TURN_90 = 90;
    const int PRE_BRIDGE_TURN = 30;

    //maneuvering

    //other
    const int LEFT = 0;
    const int RIGHT = 1;  
    const int EWOK = 0;
    const int CHEWIE = 1;
    const int ON_EDGE = 0;
    const int NOT_ON_EDGE = 1;
}
#endif //CONFIGS
