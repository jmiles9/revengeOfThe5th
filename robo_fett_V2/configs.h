#ifndef CONFIGS
#define CONFIGS

namespace configs {

    //overall use
    #define DEBUG() 0
    #define LOOP_DELAY() 10

    // let's put all ports here, then value-based constants in the specific categories
    // eg. MOTOR_SPEED_FAST
    // Analog ports
    const int IR_1KHZ = 0;
    const int IR_10KHZ = 1;
    // Digital ports

    const int LEFT_CLAW_STUFFY_SWITCH = 0;
    const int RIGHT_CLAW_STUFFY_SWITCH = 1;
    const int ZIP_SWITCH_EXTENDED = 2;
    const int ZIP_SWITCH_CLOSED = 3;
    const int ZIPPED_UP_SWITCH = 4;

    // Servo ports
    #define ARMS RCServo0
    #define CLAWS RCServo1
    #define CLAWS RCServo1
    #define DRAWBRIDGE RCServo1
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
    #define WHEEL_DIAM


    // Speeds
    #define PROPORTIONAL1
    #define DERIVITIVE1
    #define TAPEFOLLOW_SPEED1
    #define TAPEFOLLOW_SPEED2
    #define TAPEFOLLOW_GAIN1
    #define TAPEFOLLW_GAIN2
    const int ZIP_ARM_EXTENDING = 255;
    const int ZIP_ARM_CONTRACTING = -255;
    const int ZIPPING_UP = 255;

    //more threshhold stuff
    const int IR_THRESHOLD = 512;

    //claws


    //distances to move

    // Angles
    const int ARMS_UP = 120;
    const int ARMS_DOWN_EWOK = 0;
    const int ARMS_DOWN_CHEWIE = 30;
    const int CLAWS_CLOSED = 90;
    const int CLAWS_OPEN = 90;
    const int DRAWBRIDGE_OPENED = 90;
    const int DRAWBRIDGE_CLOSED = 0;
    const int BASKET_OPENED = 90;
    const int BASKET_CLOSED = 0;

    //maneuvering
}
#endif //CONFIGS
