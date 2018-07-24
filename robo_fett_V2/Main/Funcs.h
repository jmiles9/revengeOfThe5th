//Functions for use in robot.
#include <phys253.h> //this is for the uints

#ifndef FUNCS_H
#define FUNCS_H

namespace funcs {
    class Funcs {
        public:
            static uint16_t leftWheelIndex;
            static uint16_t rightWheelIndex;
            static uint32_t leftWheelLastTime;
            static uint32_t rightWheelLastTime;
            static uint8_t leftSpeed;
            static uint8_t rightSpeed;
            static uint8_t nextEwok;
            static uint8_t error;

        private:
            void setMotorPower(int left, int right);
            void steer(int deg);
            void hardStop();
            bool tapeFollow(int kp, int kd, int gain, Speed speed);
            void tapeFollowForDistance(int distance);
            bool pickUp(int side, int stuffy);
            double record1KIRBeacon();
            double record10KIRBeacon();
            void lowerBridge();
            int checkBeacon();
            bool ewokDetect();
            void turn(int deg);
            void move(int distance);
            void moveLeftWheel(int distance, int speed);
            void moveRightWheel(int distance, int speed);
            int speedToPower(int speed);
            void dumpBasket();
            void moveZipline(int distance);
            void zipppp();
            void findEdge();
            void bridgeFollow();
            bool isOnEdge();
            double distanceTravelled(int newIndex, int oldIndex);

    };
}


#endif