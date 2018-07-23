//Functions for use in robot.
#include <phys253.h>

#ifndef FUNCS_H
#define FUNCS_H

namespace funcs {
    class Funcs {
        private:
            bool tapeFollow(int kp, int kd, int gain, int speed);
            void tapeFollowForDistance(int distance);
            void hardStop();
            bool pickUp(int side, int stuffy);
            double record1KIRBeacon();
            double record10KIRBeacon();
            void lowerBridge();
            int checkBeacon();
            void steer(int deg);
            void setMotorPower(int l, int r);
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