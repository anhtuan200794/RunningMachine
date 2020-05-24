#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial Speaker(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer; // speaker
void printDetail(uint8_t type, int value); // for speaker detail

SoftwareSerial keyChanel(3, 2); // RX, TX // Key from NXP

String cmd;
char key;
int mode = 1;
int nSetup = 1;
bool isStart = false;
bool isSetup = false;
bool isMode = false;


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
  playAudio(1);
  //myDFPlayer.playFolder(15,1);
}

void loop() {
  if (keyChanel.available()) {
    //Serial.write(mySerial.read());
    key = keyChanel.read();
    Serial.print(key);
    Serial.println("-mode: " + String(mode) + "-isStart " + String(isStart) + "-isSetup " + String(isSetup)
                 + "-Setup: " + String(nSetup));
  }
  if (key == '9' && isStart == false && isMode == false) // setup
  {
    isSetup = true;
    switch (nSetup) {
      case 1:
        playAudio(8);
        break;
      case 2:
        playAudio(9);
        break;
      case 3:
        playAudio(10);
        break;
      case 4:
        myDFPlayer.pause();
        nSetup = 0;
        isSetup = false;
        break;
      default:
        break;
    }
    nSetup++;
    key = 0;
  }
  else if (key == '7' && isStart == false) { // start
    playAudio(7);
    isStart = true;
    mode = 1 ; // reset mode
    key = 0;// resset key after process
    nSetup = 1; // reset nSetup
    isSetup = false;
    isMode = false;
  }
  else if (key == '8' )
  {
    myDFPlayer.pause();
    if (isStart == true) {  // stop
      playAudio(2);
    }
    isStart = false;
    mode = 1 ; // reset mode
    key = 0;// resset key after process
    nSetup = 1; // reset nSetup
    isSetup = false;
    isMode = false;
  }
  else if (key == '6' && isStart == false && isSetup == false) {
    isMode = true;
    switch (mode) {
      case 1:
        playAudio(3);
        break;
      case 2:
        playAudio(4);
        break;
      case 12:
        playAudio(5);
        break;
      case 13:
        playAudio(6);
        break;
      case 14:
        mode = 0; // mode will be set to 1 in the end of switch
        break;
      default:
        break;
    }
    mode++;
    key = 0;// resset key after process
  }

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

void playAudio(int index) {
  Serial.println("Play: " + String(index));
  myDFPlayer.pause();
  delay(500);
  myDFPlayer.play(index);
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
