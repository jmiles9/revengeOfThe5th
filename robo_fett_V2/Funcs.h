//Functions for use in robot.

#ifndef FUNCS_H
#define FUNCS_H

class Funcs{
public:
    void tapeFollow(int kp, int kd, int gain, int speed);
    int pickUp(int side);
    double recordVisualSensor();
    void lowerBridge();
    int checkBeacon();
    void turn(int deg);
    void move(int distance);
    void dumpBasket();
    void moveZipline(int distance);
    void zipppp();
    void bridgeFollow();
    
};


#endif