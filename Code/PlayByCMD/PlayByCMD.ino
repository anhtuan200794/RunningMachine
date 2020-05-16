
#include <Wtv020sd16p.h>

int resetPin = 2;  // The pin number of the reset pin.
int clockPin = 3;  // The pin number of the clock pin.
int dataPin = 4;  // The pin number of the data pin.
int busyPin = 5;  // The pin number of the busy pin.

Wtv020sd16p wtv020sd16p(resetPin, clockPin, dataPin, busyPin);
String cmd;

void setup() {
  Serial.begin(9600);
  wtv020sd16p.reset();

  delay(2000);

  Serial.println("Typing your cmd ?");
  Serial.println("Note: Just index of song ex: 1");
  Serial.println("Type S to stop");

}

void loop() {
  if (Serial.available()) {
    cmd = Serial.readStringUntil('\n');
    Serial.println("CMD, " + cmd + "!");
  }
  if (cmd == "S")
  {
    Serial.println("Stop !");
    wtv020sd16p.stopVoice();
    cmd = "";
  }
  else if (cmd != "")
  {
    Serial.print("Play, ");
    Serial.println(cmd.toInt());
    wtv020sd16p.asyncPlayVoice(cmd.toInt());
    delay(2000);
    cmd = "";
  }

}
