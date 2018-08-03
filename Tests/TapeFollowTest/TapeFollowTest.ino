/**
 * Version 2 of tape follower code - with PD 
 * this version has set kp/kd vals and uses analog QRDs
 */
#include <phys253.h>          
#include <LiquidCrystal.h>     

//setting up sensor ports
    const int TAPE_QRD_FAR_LEFT = 3;
    const int TAPE_QRD_MID_LEFT = 4;
    const int TAPE_QRD_MID_RIGHT = 5;
    const int TAPE_QRD_FAR_RIGHT = 6; 
    const int LEFT_MOTOR = 0;
    const int RIGHT_MOTOR = 1;
    const int TAPE_QRD_THRESHOLD = 400;


        const int LARGE_LEFT_ERROR = 5;
    const int MED_LEFT_ERROR = 3;
    const int SMALL_LEFT_ERROR = 1;
    const int CENTERED_ERROR = 0;
    const int SMALL_RIGHT_ERROR = -1;
    const int MED_RIGHT_ERROR = -3;
    const int LARGE_RIGHT_ERROR = -5;

    const int ON_TAPE = 0;
    const int OFF_TAPE = 1;
    const int EDGE_THRESHOLD = 700;


//motor speeds
#define FULL_F 150
#define HALF_F 120
#define FULL_R -150
#define HALF_R -120

//PD constants
#define KP 16    
#define KD 10    
#define KI 0.02
#define GAIN 23

#define EWOK_THRESH 30

#define ROTATE_SERVO 0
#define CLOSE_SERVO 1
#define CLAW_ARM_RAISED_POSITION 0 
#define CLAW_ARM_LOWERED_POSITION 115
#define CLAW_TONG_CLOSED_POSITION 0
#define CLAW_TONG_OPEN_POSITION 110

int tf_power;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hello there");
//   RCServo0.write(CLAW_ARM_RAISED_POSITION);
//   RCServo1.write(CLAW_TONG_CLOSED_POSITION);
  attachInterrupt(2, encoderLeft, RISING);
  attachInterrupt(3, encoderRight, RISING);
  
}

int count;
int error = 0;
//cumulative error for KI
int cumError = 0;
int deg = 0;
int time = millis();

void loop() {
    int startTime = millis();
    while(millis() - startTime < 7000) {
        tapeFollow(5,6,4,KI,180);
    }
    setMotorPower(0,0);
    delay(10000);
    
    // if(edgeDetect()) {
    //   setMotorPower(0,0);
    //   delay(1000);
    //   setMotorPower(-95,-40);
    //   delay(1000);
    //   setMotorPower(0,0);
    //   delay(3000);
    //   setMotorPower(-50,-50);
    //   delay(2000);
    //   setMotorPower(110,100);
    //   delay(2500);
    // }
}

bool edgeDetect() {
  return analogRead(TAPE_QRD_FAR_LEFT) > EDGE_THRESHOLD
        && analogRead(TAPE_QRD_MID_LEFT) > EDGE_THRESHOLD 
        && analogRead(TAPE_QRD_MID_RIGHT) > EDGE_THRESHOLD 
        && analogRead(TAPE_QRD_FAR_RIGHT) > EDGE_THRESHOLD;
}

/**
 * tape following - reads sensor values, sets motor speeds using pd
 * param: kp   = proportional constant
 *        kd   = derivative constant
 *        ki   = integral constant
 *        gain = gain for pd
 */
void tapeFollow(int kp, int kd, int ki, int gain, int power) {
    tf_power = power;
    bool farLeftOnTape = (analogRead(TAPE_QRD_FAR_LEFT) < TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool midLeftOnTape = (analogRead(TAPE_QRD_MID_LEFT) < TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool midRightOnTape = (analogRead(TAPE_QRD_MID_RIGHT) < TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;
    bool farRightOnTape = (analogRead(TAPE_QRD_FAR_RIGHT) < TAPE_QRD_THRESHOLD) ? OFF_TAPE : ON_TAPE;

    const char *farLeft = farLeftOnTape ? "ON " : "OFF";
    const char *midLeft = midLeftOnTape ? "ON " : "OFF";
    const char *midRight = midRightOnTape ? "ON " : "OFF";
    const char *farRight = farRightOnTape ? "ON " : "OFF";
    LCD.clear();
    LCD.setCursor(0,0);
    LCD.print(farLeft); LCD.print(" "); LCD.print(midLeft); LCD.print(" "); 
    LCD.print(midRight); LCD.print(" "); LCD.print(farRight);


    //setting error values
    int lasterr = error; //saving the old error value

    // If one or two adjacent qrds are on tape, behaves as expected - gives error to center robot.
    // If three qrds are on tape, gives error to center robot if qrds are adjacent
    // If four qrds are on tape, or any combination of non-adjacent qrds are on tape,
    // this is unexpected, treat as anomaly, maintains last error.
    if(farLeftOnTape) {
        if(midLeftOnTape) {
            if(midRightOnTape) {
                if(farRightOnTape) {
                    error = lasterr;
                } else {
                    error = LARGE_LEFT_ERROR;
                }
            } else if(farRightOnTape) {
                error = lasterr;
            } else {
                error = MED_LEFT_ERROR;
            }
        } else if(midRightOnTape || farRightOnTape) {
            error = lasterr;
        } else {
            error = LARGE_LEFT_ERROR;
        }
    } else if(midLeftOnTape) {
        if(midRightOnTape) {
            if(farRightOnTape) {
                error = LARGE_RIGHT_ERROR;
            } else {
                error = CENTERED_ERROR;
                if(abs(deg) < 50) {
                  cumError = 0;
                 }
            }
        } else if(farRightOnTape) {
            error = lasterr;
        } else {
            error = SMALL_LEFT_ERROR;
            if(abs(deg) < 50) {
                cumError = 0;
            }
        }
    } else if(midRightOnTape) {
        if(farRightOnTape) {
            error = MED_RIGHT_ERROR;
        } else {
            error = SMALL_RIGHT_ERROR;
            if(abs(deg) < 50) {
                cumError = 0;
            }
        }
    } else if(farRightOnTape) {
        error = LARGE_RIGHT_ERROR;
    } else {
        error = lasterr;
    }
    cumError += error;
    cumError = min(400,cumError);
    cumError = max(-400,cumError);
    if(cumError==800 || cumError == -800) {
      LCD.clear(); LCD.setCursor(0,0);
      LCD.print("MAX");
    }
    deg = (kp*error + kd*(error - lasterr) + ki * cumError)*gain;
    steer(deg);
}


// Used in tapefollow
void steer(int error) {
    if(error > 0) {
        if(error > tf_power) error = tf_power;
        setMotorPower(tf_power, tf_power - error);
    } 
    else if(error < 0) {
        if(-error > tf_power) error = -tf_power;
        setMotorPower(tf_power + error, tf_power);
    } 
    else setMotorPower(tf_power, tf_power);
}

/**
 * hard stop - briefly slams motors into reverse and then comes to a 
 *  complete stop for t milliseconds
 */
void hardStop(int t){
  //slam on brakes
  setMotorPower(FULL_R, FULL_R);
  delay(10);
  setMotorPower(0,0);
  delay(t);
}

void setMotorPower(int l, int r){
    motor.speed(RIGHT_MOTOR, -r);
    motor.speed(LEFT_MOTOR, l);
}

void encoderLeft() {
    int time = millis();
}

void encoderRight() {
    int time = millis();
}
