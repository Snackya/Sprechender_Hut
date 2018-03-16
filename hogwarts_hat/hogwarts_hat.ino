#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX

unsigned long previousMillis = 0;
const long interval = 1000;
const long scanInterval = 100;
bool askingForInput = false;
float inputTreshold = 0.2;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  pinMode(7, OUTPUT);
  //digitalWrite(7, HIGH); keep Arduino on
  
  setupGyro();
  setupDFPlayer();
}

void loop() {
  unsigned long currentMillis = millis();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  gyro.getEvent(&event);

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
    Serial.println("rad/s ");
  }
  if(isAskingForInput(){
    float yData = event.gyro.y;
    float zData = event.gyro.z;

    if(yData > inputTreshold || yData < -inputTreshold || zData > inputThreshold || zData < -inputThreshold){
      //get average values
      //return nicken/schuetteln bool
    }
  }

  //digitalWrite(7, LOW); Turn off arduino
  
}

void setupDFPlayr(){
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true);
  }
  
  myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(20); //set volume (0-30)
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //----Mp3 play----
  myDFPlayer.next();  //Play next mp3
}

void setupGyro(){
  gyro.enableAutoRange(true);  
  if(!gyro.begin())
  {
    while(1);
  }
}

