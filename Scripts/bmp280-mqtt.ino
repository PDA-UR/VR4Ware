#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <WiFiUdp.h>

// das müsst ihr selber ausfüllen
#define ID 1
#define SSID "PLACEHOLDER"
#define PW "PLACEHOLDER"
#define IP "PLACEHOLDER"
#define PORT 1883 // 1234
const char* ssid = "PLACEHOLDER";
const char* password = "PLACEHOLDER";
const char* mqtt_server = "PLACEHOLDER";

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
// WiFiUDP udpClient;

unsigned long delayTime;

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

  // return true;
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


void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000 * 60 * 15; // 15min

  // initWifi();
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  Serial.println();
}

void printValues() {
  
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
  
  // client.publish("bme_temp", bme.readTemperature() );
  // client.publish("bme_hum", bme.readHumidity() );
  char buff[10];
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  client.publish("bme_temp", dtostrf(temp, 4, 6, buff) );
  client.publish("bme_hum", dtostrf(hum, 4, 6, buff) );

  // char* packet = (char*) malloc(100);
  // sprintf(packet, "%d,%f,%f,%f,%f\n", ID,
  //         bme.readTemperature(),
  //         bme.readPressure() / 100.0F,
  //         bme.readAltitude(SEALEVELPRESSURE_HPA),
  //         bme.readHumidity());

  // int length = strlen(packet);

  // udpClient.beginPacket(IP, PORT);
  // udpClient.write(packet, length);
  // udpClient.endPacket();

  // free(packet);
}

void loop() { 
  if (!client.connected()) {
      reconnect();
      Serial.println("reconnecting");
    }
    client.loop();
  
  printValues();
  
  delay(delayTime);
}
