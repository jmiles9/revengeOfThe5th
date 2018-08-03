#include <phys253.h>          
#include <LiquidCrystal.h>     

const int IR_1KHZ = 0;
const int IR_10KHZ = 1;

void setup() {
    #include <phys253setup.txt>
    Serial.begin(9600);
    LCD.clear();  LCD.home() ;
}
void loop() {
    int k = analogRead(IR_1KHZ);
    int kk = analogRead(IR_10KHZ);
    LCD.clear(); LCD.setCursor(0,0);
    LCD.print("1k: "); LCD.print(k);
    LCD.setCursor(0,1); LCD.print("10k: "); LCD.print(kk);
    delay(750); 
    // if(analogRead(IR_1KHZ) > analogRead(IR_10KHZ)) {
    //     LCD.clear(); LCD.setCursor(0,0); 
    //     LCD.print("WAIT (1k HI)");
    // } else if(analogRead(IR_1KHZ) > analogRead(IR_10KHZ)) {
    //     LCD.clear(); LCD.setCursor(0,0); 
    //     LCD.print("GO (10k HI)");
    // } else {
    //     LCD.clear(); LCD.setCursor(0,0);
    //     LCD.print("ERROR WTF");
    // }
}
