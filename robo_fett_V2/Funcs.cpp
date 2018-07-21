#include <phys253.h>
#include "configs.h"
#include "Funcs.h"


using namespace configs;

class Funcs {

    private:
    // TODO: do something about error
    /**
     * tape following - reads sensor values, sets motor speeds using pd
     * param: kp = proportional constant
     *        kd = derivative constant
     *        gain = gain for pd
     */
    void FUNCS::tapeFollow(int kp, int kd, int gain, Speed speed_) {
        //defining what speeds to use
        switch(speed_){
            case sdfsdsa:
                int highSpeed = 11000000;
                int lowSpeed = 31324;
                break;
            default:
                int highSpeed = 255;
                int lowSpeed = 200;
                break;
        }

        bool rightOnTape = digitalRead(TAPE_QRD_RIGHT);
        bool leftOnTape = digitalRead(TAPE_QRD_LEFT);
        bool edgeDetect = digitalRead(EDGE_QRD);
        //boolean edgeDetect = false; //this turns off edge detecting for testing purposes

        //setting error values
        int lasterr = error; //saving the old error value
  
        if(rightOnTape && leftOnTape){
            if(edgeDetect){
                hardStop(10);
                delay(1000);
            }
            error = 0; 
        }
        if(!rightOnTape && leftOnTape)error = -1;
        if(rightOnTape && !leftOnTape)error = 1;
        if(!rightOnTape && !leftOnTape){
            if(error == -1 || error == -5) error = -5;
            else error = 5;
        }
        //steering for error
        steer((kp*error + kd*(error - lasterr))*gain) ;
    }

    void hardStop() {
        setMotorPower(FULL_R, FULL_R);
        delay(10); //DO WE WANT A DELAY HERE?? check this
        setMotorPower(0,0);
    }

    // Used in tapefollow
    void FUNCS::steer(int deg, int speed) {
        if(deg > 0) {
            if(deg > speed*2) {
                deg = speed*2;
            }
            setMotorPower(speed - deg, speed);
        } else if(deg < 0) {
            if(-deg > speed*2) {
                deg = -speed*2;
            }
            setMotorPower(speed, speed + deg);
        } else {
            setMotorPower(speed, speed);
        }
    }

    // Used in tapeFollow
    void FUNCS::setMotorPower(int left, int right) {
        motor.speed(RIGHT_MOTOR, right);
        motor.speed(LEFT_MOTOR, left);
    }

    // TODO: complete
    // Param - distance in cm
    // NOTE: currently assuming only one set of pid constants
    void FUNCS::tapeFollowForDistance(int distance) {
        originalLeftIndex = leftWheelIndex;
        originalRightIndex = rightWheelIndex;
        while((distanceTravelled(leftWheelIndex, originalLeftIndex) 
        + distanceTravelled(rightWheelIndex, originalRightIndex)) / 2 < distance) {
            tapeFollow(TF_KP1,TF_KD1,TF_GAIN1,TF_SPEED1);
        }
        stop();
    }

    /// Parameters:
    /// side and stuffy are defined in config now.
    /// returns true if pickup was successful.
    bool FUNCS::pickUp(int side, int stuffy) {
        bool stuffyPicked = false;
        if(stuffy == EWOK) ARMS.write(ARMS_DOWN_EWOK);
        else ARMS.write(ARMS_DOWN_CHEWIE);

        delay(1000);

        CLAWS.write(CLAWS_CLOSED);

        delay(1000);

        if(side == LEFT && digitalRead(LEFT_CLAW_STUFFY_SWITCH) || side == RIGHT && digitalRead(RIGHT_CLAW_STUFFY_SWITCH)) {
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
            delay(1000);
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
        BASKET.write(BASKET_DROPBRIDGE);
        delay(1000);
        // may not need to close
        BASKET.write(BASKET_CLOSED);
    }

    bool FUNCS::checkBeacon() {
        return record10KIRBeacon() > record1KIRBeacon();
    }

    void steer(int deg){
        if(deg > 0){
            if(deg > FULL_F*2) deg = FULL_F*2;
            setMotorPower(FULL_F - deg, FULL_F);
        }
        else if(deg < 0){
            if(-deg > FULL_F*2) deg = -FULL_F*2;
            setMotorPower(FULL_F, FULL_F + deg);
        }
        else setMotorPower(FULL_F, FULL_F);
    }

    void setMotorPower(int l, int r){
        motor.speed(RIGHT_MOTOR, -r);
        motor.speed(LEFT_MOTOR, -l);
    }

    /**
    * ewok detecting - reads IR sensor, decides if it's looking at an ewok
    * return: true if ewok, false if not
    */
    bool ewokDetect() {
        digitalWrite(IR_OUT,HIGH);
        double without = analogRead(EWOK_SENSOR);
        digitalWrite(IR_OUT,LOW);
        double with = analogReaD(EWOK_SENSOR);
        if(ewokValue > EWOK_THRESH) { 
            return true; 
        } else {
            return false;
        }
    }

    //PARAM: deg - degrees to turn clockwise
    void FUNCS::turn(int deg) {
        std::thread right(moveRightWheel, -1 * deg / degreesPerCm, MAX_SPEED / 2);
        std::thread left(moveLeftWheel, deg / degreesPerCm, MAX_SPEED / 2);

        right.join();
        left.join();
    }

    //PARAM: deg - degrees to turn clockwise
    void FUNCS::move(int distance) {
        std::thread right (moveRightWheel,distance,MAX_SPEED);
        std::thread left (moveLeftWheel,distance,MAX_SPEED);

        right.join();
        left.join();
    }

    //PARAM: distance - distance in cm (positive or negative)
    //       speed    - speed in cm/s (always positive)
    void FUNCS::moveRightWheel(int distance, int speed) {
        // makes distance absoute value, speed directional
        if(distance < 0) {
            distance = distance * -1;
            speed = speed * -1;
        }
        int currIndex = rightWheelIndex;
        int currDistance = 0;
        int currSpeed = speed;
        motor.speed(RIGHT_MOTOR, speedToPower(currSpeed));
        while(currIndex - rightWheelIndex < 2) {
            if currDistance >= distance {
                break;
            }
        }
        while(currDistance < distance) {
            if(rightSpeed < speed) {
                currSpeed--;
            }
            motor.speed(RIGHT_MOTOR, speedToPower(currSpeed));
        }
        motor.stop(RIGHT_MOTOR);
    }

    //PARAM: distance - distance in cm (positive or negative)
    //       speed    - speed in cm/s (always positive)
    void FUNCS::moveLeftWheel(int distance, int speed) {
        // makes distance absoute value, speed directional
        if(distance < 0) {
            distance = distance * -1;
            speed = speed * -1;
        }
        int currIndex = leftWheelIndex;
        int currDistance = 0;
        int currSpeed = speed;
        motor.speed(LEFT_MOTOR, speedToPower(currSpeed));
        while(currIndex - leftWheelIndex < 2) {
            if(currDistance >= distance) {
                break;
            }
        }
        while(currDistance < distance) {
            if(rightSpeed < speed) {
                currSpeed--;
            }
            motor.speed(LEFT_MOTOR, speedToPower(currSpeed));
        }
        motor.stop(LEFT_MOTOR);
    }

    int speedToPower(int speed) {
        int power = nt(float(speed) / MAX_SPEED * 255);
        if(power > MAX_SPEED) {
            return MAX_SPEED;
        } else if(power < MAX_SPEED * -1) {
            return MAX_SPEED * -1;
        } else {
            return power;
        }
    }

    void FUNCS::dumpBasket() {
        BASKET.write(BASKET_OPENED);
        delay(1000);
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
        //dont think we need this since it finds the edge in the 
        //tape follow function? 
    }

    //TODO: write this. Should pretty much be tapefollow, different sensors.
    void FUNCS::bridgeFollow() {

    }

    //TODO: Write this
    bool FUNCS::isOnEdge() {
        //probs dont need this since it finds edge with tapefollow?
    }

    double distanceTravelled(int newIndex, int oldIndex) {
        return newIndex - oldIndex * cmPerWheelIndex;
    }
}
