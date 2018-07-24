#ifndef FUNCS_H
#define FUNCS_H

namespace funcs {
    class Funcs {
        public:
            uint16_t leftWheelIndex;
            uint16_t rightWheelIndex;
            uint32_t leftWheelLastTime;
            uint32_t rightWheelLastTime;
            uint8_t leftSpeed;
            uint8_t rightSpeed;
            uint8_t nextEwok;
            uint8_t error;

        private:
            void setMotorPower(int left, int right);
            void steer(int deg);
            void hardStop();
            bool tapeFollow(int kp, int kd, int gain, int speed);
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