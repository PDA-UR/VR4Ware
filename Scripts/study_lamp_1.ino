#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
// download FastLED, PubSubClient and Adafruit-Libs via Library Manager (Arduino IDE)

/*
  based on:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink and
  https://github.com/knolleary/pubsubclient/blob/v2.8/examples/mqtt_esp8266/mqtt_esp8266.ino
*/

// PINS
const int BOARD_LED = 2;
// const int SWITCH_PIN = 0; // D3-Pin
// const int SWITCH_LED = 4; // D2-Pin -> D2 is used for matrix
const int SWITCH_PIN = 13; // D7-Pin --> 1
const int SWITCH_LED = 15; // D8-Pin --> LED
int switch_val = 0; 
int prev_switch_val = 0;

// NETWORK VALUES
const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* mqtt_server = "PLACEHOLDER";

// MQTT VALUES
const char* mqtt_username = "PLACEHOLDER";
const char* mqtt_password = "PLACEHOLDER";
int has_sent = 0;
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// LED - Matrix
Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// LED - STRIP
const int NUM_LEDS = 48;
const int DATA_PIN = 14; // D5 --> DIN
CRGB leds[NUM_LEDS];
unsigned long previousMillis = 0; // last time LED was updated
const long interval = 1000;
int ledState = 1; // 1 = on, 0 = off

// STUDY - logic
int is_study = 0;
int switch_changed = 0;

/* ----- MQTT STUFF ----- */
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
      client.subscribe("remote_study_lamp");
      // This topic is used in ioBroker to send on/off msg to the lamp
      // This topic needs to be overwritten when the lamp is turned on
      // as the broker is configured to only resend mqtt once their value changed
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// callback from subscribed topic
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Turn the study on/off 
  if ((char)payload[0] == 't') { // MQTT got is "true"/"false"
    is_study = 1;
  } else {
    is_study = 0;
  }

}


/* ----- BOARD SETUP ----- */
void setup() {
  Serial.begin(9600);
  Serial.println("setting up for [STUDY-LAMP]");

  // Init digital pins
  pinMode(BOARD_LED, OUTPUT);  // BOARD_LED
  pinMode(SWITCH_PIN, INPUT_PULLUP); // SWITCH
  pinMode(SWITCH_LED, OUTPUT); // SWITCH_LED

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  // Init LED-Matrix
  // matrix.begin(0x70);  // uncomment for 8x8-Matrix

  // Init LED-Strip
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

// LED "icons"
static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };

void draw_study() {
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);
  Serial.println("[DRAW] study");
}

void draw_no_study() {
  matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(500);
  Serial.println("[DRAW] no_study");
}

// LED - STRIP
void lamp_on() {
  // blink the LED-strip independently
  // via: https://www.arduino.cc/en/Tutorial/BuiltInExamples/BlinkWithoutDelay

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval ) {
    previousMillis = currentMillis; // save last blink

    // Turn on/off
    if (ledState == 0) {
      ledState = 1;
      fill_solid(leds, NUM_LEDS, CRGB::Black);
    } else {
      ledState = 0;
      fill_solid(leds, NUM_LEDS, CRGB::Red);
    }
  }

  FastLED.show();
}

void lamp_off() {
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void loop() {
  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting");
  }
  client.loop();

  switch_val = digitalRead(SWITCH_PIN);
  // Serial.println(switch_val);

  // Check for a change in the switch
  if(prev_switch_val != switch_val) {
    // Switch the study on/off, when the switch is flipped;
    Serial.print(is_study);
    is_study = !is_study;
    Serial.print(" --switched-to--> ");
    Serial.println(is_study);
  } 
  prev_switch_val = switch_val;

  // Turn the lamp on/off
  if(is_study == 0) { // NO study

    if (has_sent == 0) { // do once per click
      has_sent = 1;
      Serial.println("Publish message: off");
      client.publish("study_lamp", "false");
      client.publish("remote_study_lamp", "false");
      digitalWrite(BOARD_LED, HIGH);
      digitalWrite(SWITCH_LED, LOW); 
      // draw_no_study(); // uncomment for 8x8-Matrix
    } 
    lamp_off();

  } 
  else if (is_study == 1) { // STUDY
    
    if (has_sent == 1) {
      has_sent = 0;
      Serial.println("Publish message: on");
      client.publish("study_lamp", "true");
      client.publish("remote_study_lamp", "true");
      digitalWrite(BOARD_LED, LOW);
      digitalWrite(SWITCH_LED, HIGH); 
      //draw_study(); // uncomment for 8x8-Matrix
      ledState = 1; // start lamp_on() with on
    }
    lamp_on();

  }

}
