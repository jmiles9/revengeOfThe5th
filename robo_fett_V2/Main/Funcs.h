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
        uint8_t leftSpeed;
        uint8_t rightSpeed;
        uint8_t nextEwok;
        int8_t error;

        void setMotorPower(int left, int right);
        void steer(int deg);
        void hardStop();
        int tapeFollow(int kp, int kd, int gain, configs::Speed speed);
        void tapeFollowForDistance(int distance);
        bool pickUp(int side, int stuffy);
        void pickUpAndHoldHalfway(int side, int stuffy);
        double record1KIRBeacon();
        double record10KIRBeacon();
        void lowerBridge();
        bool checkBeacon();
        bool ewokDetect();
        void turn(int deg);
        void move(int distance);
        void moveWheels(float leftDistance, float rightDistance, int leftPower, int rightPower);
        int speedToPower(int speed);
        void dumpBasket();
        void extendZipline();
        void contractZipline();
        void zipUp();
        void moveZipline(int distance);
        void zipppp();
        void findEdge();
        void bridgeFollow(int kp, int kd, int gain);
        bool isOnEdge();
        float distanceTravelled(int newIndex, int oldIndex);
        void tapeFollowToEdge();
        void rotateUntilTape();
        void rotateUntilTapeCCW();
        void sweepServo(TINAH::Servo servo, int startAngle, int endAngle);
};

#endif
