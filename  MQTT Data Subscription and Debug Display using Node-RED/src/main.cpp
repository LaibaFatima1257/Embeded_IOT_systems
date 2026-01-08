#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// ---------------- Wi-Fi ----------------
const char* ssid = "Umar";   // ya apna Wi-Fi SSID
const char* password = "17337960";           // Wi-Fi password

// ---------------- MQTT ----------------
const char* mqtt_server = "192.168.0.107";  // tumhare PC ka IPv4
const int mqtt_port = 1883;

const char* TOPIC_TEMP = "home/lab1/temp";
const char* TOPIC_HUM  = "home/lab1/hum";

// ---------------- DHT ----------------
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ---------------- MQTT Client ----------------
WiFiClient espClient;
PubSubClient client(espClient);

// ---------------- Functions ----------------
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32_Publisher")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);
  dht.begin();

  connectWiFi();

  client.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

// ---------------- Loop ----------------
void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  // Read DHT22
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT read failed");
    delay(2000);
    return;
  }

  // Convert to string and publish
  char tBuf[8], hBuf[8];
  dtostrf(temperature, 4, 2, tBuf);
  dtostrf(humidity,    4, 2, hBuf);

  client.publish(TOPIC_TEMP, tBuf);
  client.publish(TOPIC_HUM,  hBuf);

  Serial.print("Published -> Temp: ");
  Serial.print(tBuf);
  Serial.print(" C | Hum: ");
  Serial.print(hBuf);
  Serial.println(" %");

  delay(5000);  // every 5 sec
}
