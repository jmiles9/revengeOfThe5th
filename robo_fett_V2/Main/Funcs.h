//Functions for use in robot.
#include <phys253.h> //this is for the uints
#include "configs.h"

#ifndef FUNCS_H
#define FUNCS_H

class Funcs {
    public:
        uint32_t leftWheelLastTime;
        uint32_t leftWheelIndex;
        uint32_t rightWheelLastTime;
        uint32_t rightWheelIndex;
        int16_t leftSpeed;
        int16_t rightSpeed;
        uint8_t nextEwok;
        int8_t error;
        int32_t firstExtensionStartTime;
        int32_t leftIndexPlat3;
        bool irReady;
//        TINAH::Servo ARM_RIGHT;
//        TINAH::Servo CLAW_RIGHT;
//        TINAH::Servo ARM_LEFT;
//        TINAH::Servo CLAW_LEFT;
//        TINAH::Servo BASKET;

        TINAH::Servo ARM_RIGHT = RCServo2;
        TINAH::Servo CLAW_RIGHT = TINAH::Servo(configs::RCSERVO7);
        TINAH::Servo ARM_LEFT = RCServo0;
        TINAH::Servo CLAW_LEFT = TINAH::Servo(configs::RCSERVO6);
        TINAH::Servo BASKET = RCServo1;

        void setMotorPower(int left, int right);
        void steer(int deg);
        void hardStop();
        void tapeFollow(int kp, int kd, int ki, int gain, int power);
        void tapeFollowForDistance(int distance);
        void pickUp(int side, int stuffy);
        double record1KIRBeacon();
        double record10KIRBeacon();
        void lowerBridge();
        bool checkBeacon();
        bool ewokDetectRight();
        bool ewokDetectLeft();
        void turn(int deg);
        void move(int distance, int power);
        void moveWheels(int leftDistance, int rightDistance, int32_t lSpeed, int32_t rSpeed);
        int speedToPower(int speed);
        void dumpBasket();
        void extendZipline();
        void extendZipline(int);
        void contractZipline();
        void zipUp();
        void moveZipline(int distance);
        void zipppp();
        void findEdge();
        void bridgeFollow(int kp, int kd, int gain);
        bool edgeDetect();
        int distanceTravelled(int newIndex, int oldIndex);
        void tapeFollowToEdge(int speed);
        void rotateUntilTape();
        void rotateUntilTapeCCW();
        void sweepServo(TINAH::Servo servo, int startAngle, int endAngle);
        int maintainSpeed(int side, int targetSpeed, int power);
        void rotateUntilTape(int direction);
        void findTape();
        bool digitalReadMultiplex(int port);
        int checkEwokSensor(int side);
        void centerOffEdge();
        void moveStraight(long originalLeftIndex, long originalRightIndex);
        void steer2(int error);
        void centerOnZipline();
        void contractZipline(int time);
        void centreOnBridgeEdge();
};

#endif
