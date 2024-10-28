#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/*
  Based on:
  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
  and
  https://github.com/knolleary/pubsubclient/blob/v2.8/examples/mqtt_esp8266/mqtt_esp8266.ino
*/

// PINS
const int BOARD_LED = 2; // d1 mini board led
const int SWITCH_PIN = 0; // D3-Pin
const int SWITCH_LED = 4; // D2-Pin
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
  Serial.println();

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


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pins
  pinMode(BOARD_LED, OUTPUT);  // BOARD_LED
  pinMode(SWITCH_PIN, INPUT_PULLUP); // SWITCH

  // Serial.begin(115200);
  Serial.begin(9600);
  Serial.println("setup");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);
}

// the loop function runs over and over again forever
void loop() {

  if (!client.connected()) {
    reconnect();
    Serial.println("reconnecting");
  }
  client.loop();

  switch_val = digitalRead(SWITCH_PIN);
  // Serial.println(has_sent);

  if (switch_val == LOW) {

    if (has_sent == 0) { // check to only publish the mqtt once per switch
      Serial.println("Publish message: off");
      digitalWrite(BOARD_LED, HIGH); // HIGH = voltage level
      client.publish("study_lamp", "false");
      has_sent = 1;
    } 
    
  } else if (switch_val == HIGH) {

    if (has_sent == 1) {
      Serial.println("Publish message: on");
      digitalWrite(BOARD_LED, LOW);
      client.publish("study_lamp", "true");
      has_sent = 0;
    }
    
  }

}
