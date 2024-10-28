#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include "Adafruit_LEDBackpack.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// #include <FastLED.h>
// download FastLEd, PubSubClient and Adafruit-Libs via Library Manager

extern "C" {
   #include "gpio.h"
   #include "user_interface.h"
}

// WORKS, but only if mqtt is disabled. Has to reconnect to WIFI and MQTT every time after wake up

/*
  based on:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink and
  https://github.com/knolleary/pubsubclient/blob/v2.8/examples/mqtt_esp8266/mqtt_esp8266.ino
*/

// PINS
const int BOARD_LED = 2;
const int SWITCH_PIN = 4; // D2-Pin --> 1
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

#define WAKE_UP_PIN 4

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

  // gpio_init();
}

void callback() {
  Serial.println("Calback");
  Serial.println(millis());
  Serial.flush();
}

void light_sleep(){
  gpio_pin_wakeup_enable(GPIO_ID_PIN(4), GPIO_PIN_INTR_LOLEVEL);
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_set_wakeup_cb(callback);
  wifi_fpm_do_sleep(0xFFFFFFF); // set max. sleep time
}

int count = 0;
void loop() {

  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting");
    }
    client.loop();
  // can't seem to reconnect after coming out of light_sleep


  switch_val = digitalRead(SWITCH_PIN);
  //Serial.println(switch_val);
  
  if (switch_val == LOW) { // NO study

    if (has_sent == 0) { // do once per click
      has_sent = 1;
      Serial.println("Publish message: off");
      client.publish("study_lamp", "false");
      digitalWrite(BOARD_LED, HIGH);
    } 

  } else if (switch_val == HIGH) { // STUDY
    
    if (has_sent == 1) {
      has_sent = 0;
      Serial.println("Publish message: on");
      client.publish("study_lamp", "true");
      digitalWrite(BOARD_LED, LOW);
    }
  }

  delay(1000);
    Serial.println("Going to sleep now");
    client.publish("out_topic", "sleep");
  light_sleep();
  delay(1000);
  Serial.println("Wake up");
  client.publish("out_topic", "wake");

  //count++;

  // setup_wifi();
  // wifi_set_sleep_type(NONE_SLEEP_T);

}

