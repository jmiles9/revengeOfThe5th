#include <phys253.h>

const int COUNTS_PER_ROTATION = 16;
const int WHEEL_RADIUS = 30; //millimeters
const int WHEEL_CIRCUMFERENCE = 2*PI*WHEEL_RADIUS; //millimetres/rotation ==  

volatile int encoderCount;
int saveTime;

void setup() {
  #include <phys253setup.txt>
  Serial.begin(9600);
  Serial.print("circ = "); Serial.println(WHEEL_CIRCUMFERENCE);
  attachInterrupt(3, encoder, RISING);
  motor.speed(0,255);
  saveTime = 0; 
}

bool reset;
int count = 0;

void loop() {
  /*count++;
  if(count>100){
    Serial.println(encoderCount);
    count = 0;
  }*/
  int dt = millis() - saveTime;
  if(dt > 1000){
    int temp = (encoderCount*100)/COUNTS_PER_ROTATION; //centirotations
    int temp2 = temp*WHEEL_CIRCUMFERENCE; //distance travelled (cm*1000) the 1000 will disappear when /dt
    Serial.print("count = "); Serial.println(encoderCount);
    Serial.print("rotations = "); Serial.println(temp);
    Serial.print("distance = "); Serial.println(temp2);
    Serial.print("speed = "); Serial.print(temp2/dt); Serial.println(" cm/s"); 
    Serial.println();
    saveTime = millis();
    encoderCount=0;
  }
  while(startbutton()){ reset = true;}
  if(reset) encoderCount = 0;
  reset = false;

}

void encoder(){
  encoderCount++;
}

