#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*
  based on:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink and
  https://github.com/knolleary/pubsubclient/blob/v2.8/examples/mqtt_esp8266/mqtt_esp8266.ino
*/
// First try of full setup

// PINS
const int BOARD_LED = 2;
// const int SWITCH_PIN = 0; // D3-Pin
// const int SWITCH_LED = 4; // D2-Pin -> D2 is used for matrix
const int SWITCH_PIN = 13; // D7-Pin
const int SWITCH_LED = 15; // D8-Pin
int switch_val = 0; 

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

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

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
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


/* ----- BOARD SETUP ----- */
void setup() {
  Serial.begin(9600);
  Serial.println("setting up for [STUDY-LAMP]");

  // Init digital pins
  pinMode(BOARD_LED, OUTPUT);  // BOARD_LED
  pinMode(SWITCH_PIN, INPUT_PULLUP); // SWITCH

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  // Init LED-Matrix
  matrix.begin(0x70);  // pass in the address

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

void loop() {
  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting...");
    }
    client.loop();

  switch_val = digitalRead(SWITCH_PIN);

  if (switch_val == LOW) { // NO study

    if (has_sent == 0) { // check to only publish the mqtt once per switch
      Serial.println("Publish message: off");
      digitalWrite(BOARD_LED, HIGH);
      digitalWrite(SWITCH_LED, HIGH); 
      client.publish("study_lamp", "false");
      draw_no_study();
      has_sent = 1;
    } 
  } else if (switch_val == HIGH) { // STUDY
    
    if (has_sent == 1) {
      Serial.println("Publish message: on");
      digitalWrite(BOARD_LED, LOW);
      digitalWrite(SWITCH_LED, LOW); 
      client.publish("study_lamp", "true");
      draw_study();
      has_sent = 0;
    }

  }

}

