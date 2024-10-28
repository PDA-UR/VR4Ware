
int buzzer=14; //Buzzer control port, default 14

void setup() {
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  ringBell(3);
}

void loop() {
  // is emty :C
}

void ringBell (int rings) {
  for (int i = 0; i < rings; i++) {
    for(int i=0; i<20; i++) {
      analogWriteRange(2000);
      analogWrite(buzzer, 1024);
      delay(10);
      analogWrite(buzzer, 0);
      pinMode(buzzer, OUTPUT);
      digitalWrite(buzzer, LOW);
      delay(10);
    }
    delay(500);
  }
}