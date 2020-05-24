const byte interruptPin2 = 2;
const byte interruptPin6 = 3;
//const int pin2 = 4;

volatile int pressedButton = 0;

void setup() {
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);
  //pinMode(pin2, INPUT);

  pinMode(interruptPin6, INPUT);
   //pinMode(interruptPin2, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin6), checkKey6, RISING     );
  //attachInterrupt(digitalPinToInterrupt(interruptPin2), checkKey2, FALLING    );
}

void loop() {
  //digitalWrite(ledPin, state);
  //Serial.println(pressedButton);
}

void checkKey6() {
  Serial.println("Interrupt 6");
//  if(digitalRead(pin2) == HIGH)
//  {
//    Serial.println("Interrupt pin 6");
//    pressedButton = 6;
//  } 
}

void checkKey2() {
  Serial.println("Interrupt 2");
//  if(digitalRead(pin2) == HIGH)
//  {
//    Serial.println("Interrupt pin 7");
//    pressedButton = 7;
//  } 
}
