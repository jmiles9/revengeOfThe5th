#include <phys253.h>          
#include <LiquidCrystal.h> 

#define EWOK_DETECT 2
#define THRESH 200

void setup() {
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("hellooo");
  Serial.begin(9600);
}

void loop() {
  int ewokVal = analogRead(EWOK_DETECT);
  LCD.clear();  LCD.home() ;
  LCD.setCursor(0,0); LCD.print("sensor= "); LCD.print(ewokVal);
  Serial.println(ewokVal);
  delay(100);
}
