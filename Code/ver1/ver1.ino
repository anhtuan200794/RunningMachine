#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial Speaker(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer; // speaker
void printDetail(uint8_t type, int value); // for speaker detail

SoftwareSerial keyChanel(3, 2); // RX, TX // Key from NXP

String cmd;
char key;

void setup() {
  Speaker.begin(9600);
  keyChanel.begin(9600);
  Serial.begin(9600);

  Serial.println("Start");
  delay(1000);
  if (!myDFPlayer.begin(Speaker)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Speaker not good !"));
  }
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
}

void loop() {
  if (keyChanel.available()) {
    //Serial.write(mySerial.read());
    key = keyChanel.read();
    Serial.println(key);
  }
  
  if (key == '6') {
    myDFPlayer.pause();
    delay(1000);
    myDFPlayer.play(1);
    delay(2000);
    key = ' ';
  }

  if (cmd == "S")
  {
    Serial.println("Stop !");
    myDFPlayer.pause();
    delay(1000);
    cmd = "";
  }


  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
