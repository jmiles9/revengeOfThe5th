#include <phys253.h>          
#include <LiquidCrystal.h>     

int count = 0;

void setup() {
    #include <phys253setup.txt>
    Serial.begin(9600);
   
    attachInterrupt(1, interuptTest, CHANGE);
    LCD.clear();  LCD.home() ;
}
void loop() {
  if (startbutton()){
    count = 0;
  }
  delay(1000);
  LCD.clear();
  LCD.print(count);
}

void testServos(){
  RCServo0.write(0);
  RCServo1.write(0);
  RCServo2.write(0);
  
  RCServo0.write(180);
  RCServo1.write(180);
  RCServo2.write(180);
  delay(4000);
}

void testAnalogInput(){
// analog input testing
  
    int an0 = analogRead(0);
    int an1 = analogRead(1);
    int an2 = analogRead(2);
    int an3 = analogRead(3);
    int an4 = analogRead(4);
    int an5 = analogRead(5);
    int an6 = analogRead(6);
    int an7 = analogRead(7);

        

    Serial.print("an0:  "); Serial.println(an0);
    Serial.print("an1:  "); Serial.println(an1);
    Serial.print("an2:  "); Serial.println(an2);
    Serial.print("an3:  "); Serial.println(an3);
    Serial.print("an4:  "); Serial.println(an4);
    Serial.print("an5:  "); Serial.println(an5);
    Serial.print("an6:  "); Serial.println(an6);
    Serial.print("an7:  "); Serial.println(an7);

    delay(2000);
}
void testDigitalOutput(){
  
}

void testMotorOutputs(){
  LCD.print("m0");
  delay(5000);
  motor.speed(0,250);
  delay(2000);
  motor.stop(0);
  LCD.clear();
  LCD.print("m1");
  delay(5000);
  motor.speed(1,250);
  delay(2000);
  motor.stop(1);
  LCD.clear();
  LCD.print("m2");
  delay(5000);
  motor.speed(2,250);
  delay(2000);
  motor.stop(2);
  LCD.clear();
  LCD.print("m3");
  delay(5000);
  motor.speed(3,250);
  delay(2000);
  motor.stop(3);
}

void testDigitalInput(){
// digital input testing


    pinMode(0, INPUT);
    pinMode(1, INPUT);
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    

    int dig0 = digitalRead(0);
    int dig1 = digitalRead(1);
    int dig2 = digitalRead(2);
    int dig3 = digitalRead(3);
    int dig4 = digitalRead(4);
    int dig5 = digitalRead(5);
    int dig6 = digitalRead(6);
    int dig7 = digitalRead(7);
    


    Serial.print("d0:  "); Serial.println(dig0);
    Serial.print("d1:  "); Serial.println(dig1);
    Serial.print("d2:  "); Serial.println(dig2);
    Serial.print("d3:  "); Serial.println(dig3);
    Serial.print("d4:  "); Serial.println(dig4);
    Serial.print("d5:  "); Serial.println(dig5);
    Serial.print("d6:  "); Serial.println(dig6);
    Serial.print("d7:  "); Serial.println(dig7);

    Serial.println();
    Serial.println();

    delay(3000); 
}

void interuptTest(){
  LCD.clear(); LCD.print("Triggered");
  count++;
}

