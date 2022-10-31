#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <ESPDateTime.h>
#include "secrets.h"
#include "DHT.h"

#define MQTT_VERSION MQTT_VERSION_3_1_1
 
#define DHTPIN 4        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
 
DHT dht(DHTPIN, DHTTYPE);
 
float h ;
float t;

WiFiClient net;
PubSubClient client(net);
 
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(THINGNAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println(String("Attempting to connect to SSID: ") + String(WIFI_SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("IP GW: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
}

void NTPConnect(void) {
  DateTime.setServer("ntp.ubuntu.com");
  DateTime.setTimeZone(TIME_ZONE);
  DateTime.begin();
  delay(3000);
  if (!DateTime.isTimeValid()) {
    Serial.println("NTP server: fail.");
  } else {
    Serial.printf("Now is %s\n", DateTime.toISOString().c_str());
  }
}

void messageReceived(char *topic, byte *payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connetMQTT() {
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(messageReceived);
 
  Serial.println("Connecting to MQTT");

  while (!client.connect(THINGNAME)) {
    //Serial.print("ERROR CODE (PubSubClient.h): ");
    //Serial.println(client.state());
    Serial.print(".");
    delay(1000);
  }
 
  if (!client.connected()) {
    Serial.println("MQTT Timeout!");
    return;
  }
  client.subscribe(SUBSCRIBE_TOPIC);
 
  Serial.println("MQTT Connected!");
}
 
void publishMessage() {
  StaticJsonDocument<200> doc;
  doc["time"] = DateTime.toISOString();
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  NTPConnect();
  connetMQTT();
  dht.begin();
}
 
void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
 
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(2000);
    return;
  }
 
  Serial.print(DateTime.toISOString().c_str());
  Serial.print(F(" - Humidity: "));
  Serial.print(h);
  Serial.print(F("%  - Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
 
  if (!client.connected()) {
    connetMQTT();
  } else {
    client.loop();
    publishMessage();
  }
  delay(FREQ);
}
