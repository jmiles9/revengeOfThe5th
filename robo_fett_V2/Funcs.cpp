#include <phys253.h>
#include "configs.h"

using namespace configs;

class Funcs {

    private:
    // TODO: complete function
    void FUNCS::tapeFollow(int kp, int kd, int gain, int speed) {

    }

    // TODO: complete
    void tapeFollowForDistance(int distance) {

    }

    // TODO: complete
    void stop() {

    }
    /// Parameters:
    /// side   - 0 represents left, 1 represents right.
    /// stuffy - 0 represents ewok, 1 represents chewie.
    /// returns true if pickup was successful.
    bool FUNCS::pickUp(int side, int stuffy) {
        bool stuffyPicked = false;
        if(stuffy == 0) {
            ARMS.write(ARMS_DOWN_EWOK);
        } else {
            ARMS.write(ARMS_DOWN_CHEWIE);
        }

        usleep(1000);

        CLAWS.write(CLAWS_CLOSED);

        usleep(1000);

        if(side == 0 && digitalRead(LEFT_CLAW_STUFFY_SWITCH) || side == 1 && digitalRead(RIGHT_CLAW_STUFFY_SWITCH)) {
            stuffyPicked = true;
        }

        // If pickup is unsuccessful, can raise arm and open claw at the same time.
        if(!stuffyPicked) {
            ARMS.write(ARMS_UP);
            CLAWS.write(CLAWS_OPEN);
            return stuffyPicked;
        }

        else {
            ARMS.write(ARMS_UP);
            usleep(1000);
            CLAWS.write(CLAWS_OPEN);
            return stuffyPicked;
        }
    }

    // CURRENTLY ASSUMINE ONE IR SENSOR
    double FUNCS::record1KIRBeacon() {
        return analogRead(IR_1KHZ);
    }

    double FUNCS::record10KIRBeacon() {
        return analogRead(IR_10KHZ);
    }

    void FUNCS::lowerBridge() {
        DRAWBRIDGE.write(DRAWBRIDGE_OPENED);
        usleep(1000);
        DRAWBRIDGE.write(DRAWBRIDGE_CLOSED);
    }

    bool FUNCS::checkBeacon() {
        return record10KIRBeacon() > record1KIRBeacon();
    }

    //TODO: Write this
    void FUNCS::turn(int deg) {

    }

    //TODO: Write this
    void FUNCS::move(int distance) {

    }

    void FUNCS::dumpBasket() {
        BASKET.write(BASKET_OPENED);
        usleep(1000);
        BASKET.write(BASKET_CLOSED);
    }

    void FUNCS::extendZipline() {
        motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_EXTENDING);
        double startTime = millis();
        while(!digitalRead(ZIP_SWITCH_EXTENDED)) {
            // Give the arm 5 seconds to extend
            if(millis() > startTime + 5000) {
                break;
            }
        }
        motor.stop(ZIP_ARM_MOTOR);
    }

    void FUNCS::contractZipline() {
        motor.speed(ZIP_ARM_MOTOR, ZIP_ARM_CONTRACTING);
        double startTime = millis();
        while(!digitalRead(ZIP_SWITCH_CONTRACTED)) {
            // Give the arm 5 seconds to extend
            if(millis() > startTime + 5000) {
                break;
            }
        }
        motor.stop(ZIP_ARM_MOTOR);
    }

    void FUNCS::zipUp() {
        motor.speed(ZIP_WHEEL_MOTOR, ZIPPING_UP);
        double startTime = millis();
        while(!digitalRead(ZIPPED_UP)) {
            // Give 10 seconds to make it up zipline
            if(millis() > startTime + 10000) {
                break;
            }
        }
        motor.stop(ZIP_WHEEL_MOTOR);
    }

    //TODO:
    void FUNCS::findEdge() {

    }

    //TODO: write this. Should pretty much be tapefollow, different sensors.
    void FUNCS::bridgeFollow() {

    }

    //TODO: Write this
    bool FUNCS::isOnEdge() {

    }
}