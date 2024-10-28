// via: https://www.roboter-bausatz.de/projekte/lichtsensor-fotowiderstand-mit-arduino-auslesen

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* mqtt_server = "PLACEHOLDER";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


int LDR = A0; // Analoger Pin als LDR Eingang
int LED = 2; // LED-Pin
int sensorWert = 0; // Variable für den Sensorwert mit 0 als Startwert


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    // 2 or BUILTIN_LED
    digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "PLACEHOLDER", "PLACEHOLDER")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("motionSensor1", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode (LED, OUTPUT);
  setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  sensorWert = analogRead(LDR); //
  Serial.println(sensorWert); //Ausgabe am Serial-Monitor.

  if (sensorWert > 512 ) { // Wenn der Schwellenwert 512 überschritten wird, soll die LED leuchten
    digitalWrite(LED, LOW);
    client.publish("lightSensor1", "true");
    Serial.println("Publish message: true"); // LED an
  }
// ansonsten...
  else {
    digitalWrite(LED, HIGH);
    client.publish("lightSensor1", "false");
    Serial.println("Publish message: false"); // LED aus
    delay (50);
  }
}
