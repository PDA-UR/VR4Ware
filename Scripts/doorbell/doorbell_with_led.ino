#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <string.h>


const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* mqtt_server = "PLACEHOLDER";

const char* mqtt_username = "PLACEHOLDER";
const char* mqtt_password = "PLACEHOLDER";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

const int buttonPin = 0;  // Button Shield ist D3 = 0
const int ledPin = 2;    // LED pin

const int LED = 4;
const int NUM_LEDS = 7;

int previousMillis = 0;
int buzzpreviousMillis = 0;

int blinkCount = 0;
int blinkAmount = 0;
int ledState = 0;

int buttonPushCounter = 0;
int buttonState = 0; 
int lastButtonState = 1; // variable for reading the pushbutton status

CRGB leds[NUM_LEDS];

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
  if (strcmp(topic, "doorbell") == 0) { // if doorbell
    if ((char)payload[0] == 't') { // if true 
      
    } 
  }

  if ((char)payload[0] == '1') {
    // 2 or BUILTIN_LED
    digitalWrite(ledPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(ledPin, HIGH);  // Turn the LED off by making the voltage HIGH
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
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("doorbell");
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
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(ledPin, HIGH);
  FastLED.addLeds<NEOPIXEL, LED>(leds, NUM_LEDS);
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
  }
  client.loop();

  buttonState = digitalRead(buttonPin);

  if(buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      digitalWrite(ledPin, HIGH);
      client.publish("doorbell", "false");
    //  Serial.println("Doorbell rang.");
      
    } else {
      digitalWrite(ledPin, LOW);
      client.publish("doorbell", "true");
      Serial.println("Doorbell rang(not).");

      Serial.print("Bell rung");
      blinkCount = 0;
      blinkAmount = 1;
      ledState = 1;
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      FastLED.show();
      delay(1000);
      ledState = 0;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }
  }  
  
}