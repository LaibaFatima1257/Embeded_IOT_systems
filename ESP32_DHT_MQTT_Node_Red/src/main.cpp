#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- Pins ----------------
#define DHTPIN 23
#define DHTTYPE DHT22
#define BUTTON_PIN 5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ---------------- Wi-Fi ----------------
const char* ssid = "Umar";  // Change if real WiFi
const char* password = "17337960";

// ---------------- MQTT ----------------
const char* mqtt_server = "192.168.0.107"; // PC IP
const int mqtt_port = 1883;

const char* TOPIC_TEMP = "home/lab1/temp";
const char* TOPIC_HUM = "home/lab1/hum";

// ---------------- Objects ----------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClient espClient;
PubSubClient client(espClient);

// For button
bool lastButtonState = HIGH;

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

void readAndDisplayAndSend() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("DHT read failed!");
    return;
  }

  // Serial Monitor
  Serial.print("Temp: "); Serial.print(t); Serial.print(" C | ");
  Serial.print("Hum: "); Serial.print(h); Serial.println(" %");

  // OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 DHT22 Monitor");
  display.println("------------------");
  display.print("Temp: "); display.print(t,1); display.println(" C");
  display.print("Hum : "); display.print(h,1); display.println(" %");
  display.display();

  // MQTT Publish
  char tBuf[8], hBuf[8];
  dtostrf(t, 4, 2, tBuf);
  dtostrf(h, 4, 2, hBuf);
  client.publish(TOPIC_TEMP, tBuf);
  client.publish(TOPIC_HUM, hBuf);
}

// ---------------- Setup ----------------
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println("SSD1306 init failed!");
    while(1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Booting...");
  display.display();

  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

// ---------------- Loop ----------------
void loop() {
  if (!client.connected()) connectMQTT();
  client.loop();

  bool buttonState = digitalRead(BUTTON_PIN);
  if(lastButtonState == HIGH && buttonState == LOW){
    delay(50); // debounce
    if(digitalRead(BUTTON_PIN) == LOW){
      readAndDisplayAndSend();
    }
  }
  lastButtonState = buttonState;

  // Optional: auto-send every 5 sec
  static unsigned long lastTime = 0;
  if(millis() - lastTime > 5000){
    lastTime = millis();
    readAndDisplayAndSend();
  }
}
