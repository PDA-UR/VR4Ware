// via: https://www.roboter-bausatz.de/projekte/lichtsensor-fotowiderstand-mit-arduino-auslesen

int LDR = A0; // Analoger Pin als LDR Eingang
int LED = 2; // LED-Pin
int sensorWert = 0; // Variable für den Sensorwert mit 0 als Startwert

void setup()
{
Serial.begin(9600);
pinMode (LED, OUTPUT);
}

void loop()
{
sensorWert =analogRead(LDR); //
Serial.println(sensorWert); //Ausgabe am Serial-Monitor.

if (sensorWert > 512 ) // Wenn der Schwellenwert 512 überschritten wird, soll die LED leuchten
{
digitalWrite(LED, LOW); // LED an
}
// ansonsten...
else{
digitalWrite(LED, HIGH); // LED aus
delay (50);
}
}
