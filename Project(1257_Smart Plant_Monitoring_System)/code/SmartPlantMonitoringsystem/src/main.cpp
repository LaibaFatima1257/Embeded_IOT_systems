/****************************************************
 * Smart Plant Monitoring System (MQTT + Node-RED + Blynk)
 * ESP32 + DHT11 + Soil Sensor + OLED + Relay + MQTT
 * Group Members:
 *               23-ntu-cs-1257(Laiba Fatima)
 *               23-ntu-cs-1259(Mahnoor Sajjad)
 * BSAI_5th
 ****************************************************/

#define BLYNK_TEMPLATE_ID   "TMPL6Wj9hIWLz"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring System"
#define BLYNK_AUTH_TOKEN    "Y1586IR5zmLZyro0jc1Tvgs7lx07aw3d"

#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <PubSubClient.h>

// ---------------- WiFi ----------------
char ssid[] = "Umar";
char pass[] = "17337960";

// ---------------- Pins ----------------
#define DHTPIN     27
#define DHTTYPE    DHT11
#define SOIL_PIN   34
#define RELAY_PIN  26

// ---------------- OLED ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------------- Objects ----------------
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ---------------- MQTT ----------------
const char* mqtt_server = "192.168.0.107"; // PC IP with Mosquitto
const int   mqtt_port   = 1883;

const char* TOPIC_TEMP  = "smartplant/temp";
const char* TOPIC_HUM   = "smartplant/hum";
const char* TOPIC_SOIL  = "smartplant/soil";
const char* TOPIC_MOTOR = "smartplant/motor";

WiFiClient espClient;
PubSubClient mqtt(espClient);

// ---------------- Variables ----------------
int soilValue = 0;
int threshold = 500;      // Soil dry limit
bool motorState = false;

// ---------------- Blynk Motor Control ----------------
BLYNK_WRITE(V3) {   // Button widget (Switch Mode)
  motorState = param.asInt();
  digitalWrite(RELAY_PIN, motorState ? HIGH : LOW);
}

// ---------------- MQTT Connect ----------------
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.println("Connecting to MQTT...");
    if (mqtt.connect("ESP32_SmartPlant")) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

// ---------------- Read Sensors ----------------
void readAndSendData() {

  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();
  soilValue  = analogRead(SOIL_PIN);

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT Error");
    return;
  }

  // ---------- OLED Display ----------
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Smart Plant System");
  display.println("------------------");
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Hum : "); display.print(hum); display.println(" %");
  display.print("Soil: "); display.println(soilValue);

  if (soilValue < threshold)
    display.println("Soil: DRY");
  else
    display.println("Soil: WET");

  display.display();

  // ---------- Auto Watering ----------
  if (soilValue < threshold && !motorState) {
    digitalWrite(RELAY_PIN, HIGH);
    Blynk.virtualWrite(V3, 1);
    motorState = true;
  }
  else if (soilValue >= threshold && !motorState) {
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V3, 0);
    motorState = false;
  }

  // ---------- Send to Blynk ----------
  Blynk.virtualWrite(V0, soilValue); // Soil
  Blynk.virtualWrite(V1, temp);      // Temperature
  Blynk.virtualWrite(V2, hum);       // Humidity

  // Soil Status LED
  if (soilValue < threshold)
    Blynk.virtualWrite(V4, 255);  // DRY
  else
    Blynk.virtualWrite(V4, 0);    // WET

  // ---------- Publish to MQTT ----------
  char tBuf[8], hBuf[8], sBuf[8], mBuf[2];
  dtostrf(temp, 4, 2, tBuf);
  dtostrf(hum,  4, 2, hBuf);
  itoa(soilValue, sBuf, 10);
  itoa(motorState, mBuf, 10);

  mqtt.publish(TOPIC_TEMP, tBuf);
  mqtt.publish(TOPIC_HUM,  hBuf);
  mqtt.publish(TOPIC_SOIL, sBuf);
  mqtt.publish(TOPIC_MOTOR, mBuf);

  Serial.print("Temp: "); Serial.print(tBuf);
  Serial.print(" | Hum: "); Serial.print(hBuf);
  Serial.print(" | Soil: "); Serial.print(sBuf);
  Serial.print(" | Motor: "); Serial.println(mBuf);
}

// ---------------- Setup ----------------
void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // OLED
  Wire.begin(21, 22);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Sensors
  dht.begin();

  // WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // MQTT
  mqtt.setServer(mqtt_server, mqtt_port);
  connectMQTT();

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Timer
  timer.setInterval(3000L, readAndSendData);
}

// ---------------- Loop ----------------
void loop() {
  Blynk.run();
  timer.run();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}
