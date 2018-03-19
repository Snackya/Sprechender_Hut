#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <CapacitiveSensor.h>

const static int RXpin = 11;
const static int TXpin = 10;

DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(RXpin, TXpin);
CapacitiveSensor cs_8_7 = CapacitiveSensor(8,7);

void setup() {
  cs_8_7.set_CS_AutocaL_Millis(0xFFFFFFFF);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  setupGyro();
  setupDFPlayer();
}

void loop() {
  /* Get a new sensor event */
  long start = millis();
  long total1 =  cs_8_7.capacitiveSensor(30);
  sensors_event_t event; 
  gyro.getEvent(&event);

   Serial.print(millis() - start);        // check on performance in milliseconds
   Serial.print("\t");                    // tab character for debug window spacing
   Serial.print(total1);                  // print sensor output 1
   Serial.println();
  
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");
  Serial.println();
  delay(500);
}

void setupDFPlayer(){
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true);
  }
  
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(5); //set volume (0-30)
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //----Mp3 play----
  myDFPlayer.play(2);  //Play next mp3
}

void setupGyro(){
  gyro.enableAutoRange(true); 
  if(!gyro.begin())
  {
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
}

