#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <string.h>

// NETWORK VALUES
const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* mqtt_server = "PLACEHOLDER";

// MQTT VALUES
const char* mqtt_username = "PLACEHOLDER";
const char* mqtt_password = "PLACEHOLDER";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

int buzzer = 14; //Buzzer control port, default 14
const int LED = 4; //Port D2
const int NUM_LEDS = 8;

int blinkInterval = 1000;
int buzzInterval = 1000;

int previousMillis = 0;
int buzzpreviousMillis = 0;

int blinkCount = 0;
int blinkAmount = 0;
int ledState = 0;

int buzzCount = 0;
int buzzAmount = 0;
int soundState = 0;

bool silent = true;

CRGB leds[NUM_LEDS];


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("setting up...");

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
  if (strcmp(topic, "doorbell") == 0) { // if doorbell
    if ((char)payload[0] == 't') { // if true 
      Serial.print("Bell rung");
      blinkCount = 0;
      blinkAmount = 3;
      if(!silent) { 
        buzzCount = 0;
        buzzAmount = 3;
      }
    } 
  } else if (strcmp(topic, "remote_study_lamp") == 0) {
 // topic = remote_study_lamp
    if ((char)payload[0] == 't') { 
      silent = true;
    } else {
      silent = false;
    }
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
    // if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("doorbell");
      client.subscribe("remote_study_lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, LED>(leds, NUM_LEDS);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  Serial.begin(115200);
  Serial.println("setup");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  ledState = 0;
}

void loop() {
  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting");
  }
  client.loop();
  blink();
  buzz();
}

void buzz () {
  unsigned long currentMillis = millis();
  if(currentMillis - buzzpreviousMillis >= buzzInterval ) {
    buzzpreviousMillis = currentMillis; // save last blink

    if (buzzCount < buzzAmount) {
      if (soundState == 0) {
          soundState = 1;
          analogWriteRange(2000);
          analogWrite(buzzer, 1024);
      } else {
          soundState = 0;
          pinMode(buzzer, OUTPUT);
          digitalWrite(buzzer, LOW);
          buzzCount++;
      }
    }
  }
}

void blink () {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= blinkInterval ) {
    previousMillis = currentMillis; // save last blink

    if (blinkCount < blinkAmount) {
      if (ledState == 0) {
        ledState = 1;
        fill_solid(leds, NUM_LEDS, CRGB::Blue);
        Serial.println("blinkingOn");
      } else {
        ledState = 0;
        fill_solid(leds, NUM_LEDS, CRGB::Black);
        Serial.println("blinkingOff");
        blinkCount++;
      }
      FastLED.show(); 
    } 
  } 
}
