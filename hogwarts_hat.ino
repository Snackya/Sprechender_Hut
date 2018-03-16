#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(11, 10); // RX, TX

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  myDFPlayer.setTimeOut(500);

  myDFPlayer.volume(20); //set volume (0-30)

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  //----Mp3 play----
  myDFPlayer.next();  //Play next mp3
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
  digitalWrite(7, LOW);
}
