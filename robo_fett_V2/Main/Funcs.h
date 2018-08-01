//Functions for use in robot.
#include <phys253.h> //this is for the uints
#include "configs.h"

#ifndef FUNCS_H
#define FUNCS_H

class Funcs {
    public:
        uint16_t leftWheelIndex;
        uint16_t rightWheelIndex;
        uint32_t leftWheelLastTime;
        uint32_t rightWheelLastTime;
        uint16_t leftSpeed;
        uint16_t rightSpeed;
        uint8_t nextEwok;
        int8_t error;
        TINAH::Servo RCServo7;
        TINAH::Servo RCServo6;

        void setMotorPower(int left, int right);
        void steer(int deg);
        void hardStop();
        void tapeFollow(int kp, int kd, int gain, int power);
        void tapeFollowForDistance(int distance, int power);
        void pickUp(int side, int stuffy);
        double record1KIRBeacon();
        double record10KIRBeacon();
        void lowerBridge();
        bool checkBeacon();
        bool ewokDetectRight();
        bool ewokDetectLeft();
        void turn(int deg);
        void move(int distance, int speed);
        void moveWheels(int leftDistance, int rightDistance, int leftPower, int rightPower);
        int speedToPower(int speed);
        void dumpBasket();
        void extendZipline();
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


};

#endif
