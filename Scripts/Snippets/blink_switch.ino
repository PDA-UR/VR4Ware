/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

// #define ledPin 2;
#define SWITCH_PIN 4;  //D2

int val = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);  // BUILTIN_LED
  pinMode(0, OUTPUT); // SWITCH_LED
  pinMode(4, INPUT_PULLUP); // SWITCH

  // Begin serial communication at a baud rate of 9600:
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  val = digitalRead(4);

  if (val == LOW) {
    Serial.println("Off");
    digitalWrite(2, HIGH); 
    digitalWrite(0, HIGH); 
  } else {
    Serial.println("On");
    digitalWrite(2, LOW);
    digitalWrite(0, LOW);
  }
  Serial.println(val);
}