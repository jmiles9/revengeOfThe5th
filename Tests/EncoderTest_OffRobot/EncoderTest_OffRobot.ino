#include <phys253.h>          
#include <LiquidCrystal.h>     


void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  attachInterrupt(3, encoderLeft, CHANGE);
}

int count = 0;
int lastLeftTime = 0;
int leftCount = 0;


void loop() {
    count++;
    if(count >= 1000) {
        count = 0;
        Serial.println(leftCount);
    }
}

void encoderLeft() {
    int time = millis();
    leftCount++;
    lastLeftTime = time;
}

