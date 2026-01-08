/****************************************************
 * Smart Plant Monitoring System (Button Free)
 * ESP32 + DHT11 + Soil Sensor + OLED + Relay + Blynk
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

// ---------------- Variables ----------------
int soilValue = 0;
int threshold = 500;      // Soil dry limit
bool motorState = false;

// ---------------- Blynk Motor Control ----------------
BLYNK_WRITE(V3) {   // Button widget (Switch Mode)
  motorState = param.asInt();
  digitalWrite(RELAY_PIN, motorState ? HIGH : LOW);
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
  }
  else if (soilValue >= threshold && !motorState) {
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V3, 0);
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

  // Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Timer
  timer.setInterval(3000L, readAndSendData);
}

// ---------------- Loop ----------------
void loop() {
  Blynk.run();
  timer.run();
}
