#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
int is_clicked = 0;

int buzzer=14; //Buzzer control port, default 14

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
  if ((char)payload[0] == 't') {
    is_clicked = 1;
    Serial.print("Bell rung");
    ringBell(3);
  } else {
    is_clicked = 0;
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
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
//  ringBell(3);
  Serial.begin(115200);
  Serial.println("setup");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting");
  }
  client.loop();
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