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
    const int IR_1KHZ = 0;
    const int IR_10KHZ = 1;
    const int EWOK_SENSOR_LEFT = 2;
    const int EWOK_SENSOR_RIGHT = 3;
    const int TAPE_QRD_FAR_LEFT = 4;
    const int TAPE_QRD_MID_LEFT = 5;
    const int TAPE_QRD_MID_RIGHT = 6;
    const int TAPE_QRD_FAR_RIGHT = 7; 

    
    // Digital ports
    // pls go backwards. don't use interrupt ports (0-3) for switches
    
    //multiplexer inputs 16 to 23 (0 on multiplexer is 16 for here)
    //all must be inputs
       
    //tinah inputs 15 to 8
    //must be outputs

    const int EWOK_IR_OUT_RIGHT = 8; //when HI, the ewok IR
    const int EWOK_IR_OUT_LEFT = 9;
    
    //tinah inputa 0 to 7 (including interrupts)
    //must be inputs cuz interrupts are 0-3
    //pins 0-3 can only be interrupts!!!!!!!!!!!!

    const int ENCODER_LEFT = 2;
    const int ENCODER_RIGHT = 3;
    
    //all ports below arbitrary

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
    // mm
    const int wheelRadius = 32;
    const int mmPerWheelIndex = wheelRadius * 3 * 2 / 24; // 8mm
    #define WHEEL_DIAM
    // mm
    const int wheelSeparation = 175;
    // amount of rotation if left and right wheels move in opposite directions
    const float degreesPermm = 360 / (3.14 * wheelSeparation);


    // Speeds
    const int FULL_F = 255;
    const int HALF_F = 220;
    const int FULL_R = -255;
    const int HALF_R = -220;

    #define TF_KP1 9
    #define TF_KD1 12
    #define TF_SPEED1 0
    #define TF_SPEED2 0
    #define TF_GAIN1 9
    #define TF_GAIN2 0
    const int ZIP_ARM_EXTENDING = 255;
    const int ZIP_ARM_CONTRACTING = -255;
    const int ZIPPING_UP = 255;
    // in mm/s
    const int MAX_SPEED = 500;

    enum Speed{
        SPEED,
        LOWSPEED
    };

    //servo stuff
    const int ROTATE_SERVO = 0;
    const int CLOSE_SERVO = 1;
    const int CLAW_ARM_RAISED_POSITION = 0; 
    const int CLAW_ARM_LOWERED_POSITION = 115;
    const int CLAW_TONG_CLOSED_POSITION = 0;
    const int CLAW_TONG_OPEN_POSITION = 110;

    //TRESHHOLDS
    const int IR_THRESHOLD = 512;
    const int EWOK_THRESH = 250;
    const int TAPE_QRD_THRESHOLD = 512;
    const int EDGE_QRD_THRESHOLD = 100;
    //digitalOut

    //claws


    //distances to move

    const float DIST_CONV = 180/100 * (150 / 100) * 220 / 78*4/3 * 5.5/4;
    
    const int STUFFY_GRAB_MANEUVER = 10;

    const float PLAT1_CRUISE = 1500;
    const int PRE_BRIDGE_MOVE = -50;
    const int BRIDGE_REVERSE = -125;
    const int BRIDGE_CRUISE = 500;
    const int PLAT2_CRUISE = 130;
    const int DUMP_PREP_DIST = 15;
    const int DUMP_RAM_DISTANCE = 10;


    // Angles
    const int ARMS_UP = 140;
    const int ARM_HALF = 70;
    const int ARMS_DOWN_EWOK = 0;
    const int ARMS_DOWN_CHEWIE = 30;
    const int CLAWS_CLOSED = 0;
    const int CLAWS_OPEN = 110;
    const int DRAWBRIDGE_OPENED = 90;
    const int DRAWBRIDGE_CLOSED = 0;
    const int BASKET_OPENED = 90;
    const int BASKET_DROPBRIDGE = 45;
    const int BASKET_CLOSED = 0;
    const int ZIPLINE_ATTACH_ROTATION = 15;
    const int TURN_90 = 90;
    const int PRE_BRIDGE_TURN = 30;

    //maneuvering
    const int LARGE_LEFT_ERROR = 5;
    const int MED_LEFT_ERROR = 3;
    const int SMALL_LEFT_ERROR = 1;
    const int CENTERED_ERROR = 0;
    const int SMALL_RIGHT_ERROR = -1;
    const int MED_RIGHT_ERROR = -3;
    const int LARGE_RIGHT_ERROR = -5;

    //direction
    const int CLOCKWISE = 0;
    const int COUNTERCW = 1;

    //other
    const int LEFT = 0;
    const int RIGHT = 1;  
    const int EWOK = 0;
    const int CHEWIE = 1;
    const int ON_EDGE = 0;
    const int NOT_ON_EDGE = 1;
    const int ON_TAPE = 0;
    const int OFF_TAPE = 1;
}
#endif //CONFIGS
