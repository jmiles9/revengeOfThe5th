#include <phys253.h>          
#include <LiquidCrystal.h>     


void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  attachInterrupt(2, encoderLeft, FALLING);
  attachInterrupt(2, encoderLeft, RISING);

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
    if(time - lastLeftTime > 4) {
        leftCount++;
        lastLeftTime = time;
    }
}
