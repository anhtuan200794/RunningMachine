const byte interruptPin = 2;
const int pin6 = 3;
const int pin7 = 4;
const int pin8 = 5;
const int pin9 = 6;
volatile int pressedButton = 0;

void setup() {
  Serial.begin(9600);
  //pinMode(ledPin, OUTPUT);
  pinMode(pin6, INPUT);
  pinMode(pin7, INPUT);
  pinMode(pin8, INPUT);
  pinMode(pin9, INPUT);
  pinMode(interruptPin, INPUT);
  //pinMode(interruptPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), checkKey, RISING  );
}

void loop() {
  //digitalWrite(ledPin, state);
}

void checkKey() {
  Serial.print("Pin 6: ");
  Serial.println(digitalRead(pin6));
   Serial.print("Pin 7: ");
  Serial.println(digitalRead(pin7));
  //detachInterrupt(0);
  //Serial.println("Interrupt !");
//  if(digitalRead(pin6) == LOW)
//  {
//    Serial.println("Interrupt pin 6");
//    pressedButton = 6;
//  } 
//  if(digitalRead(pin7) == LOW)
//  {
//    Serial.println("Interrupt pin 7");
//    pressedButton = 7;
//  } 
//attachInterrupt(digitalPinToInterrupt(interruptPin), checkKey, RISING  );
//  else if(digitalRead(pin8) == LOW)
//  {
//    Serial.println("Interrupt pin 8");
//    pressedButton = 8;
//  }
//  if(digitalRead(pin9) == LOW)
//  {
//    Serial.println("Interrupt pin 9");
//    pressedButton = 9;
//  }
}
