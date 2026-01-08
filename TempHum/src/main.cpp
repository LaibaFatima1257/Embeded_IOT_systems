#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
const char* mqtt_server = "10.0.2.2"; 
WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dhtSensor;

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(15, DHTesp::DHT22);
  
  // OLED Setup
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) { /* reconnect logic here */ }
  client.loop();

  float t = dhtSensor.getTemperature();
  float h = dhtSensor.getHumidity();

  // OLED Display Logic
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("IoT Temp Monitor");
  display.setTextSize(2);
  display.print("T: "); display.print(t); display.println("C");
  display.print("H: "); display.print(h); display.println("%");
  display.display();

  client.publish("home/lab1/temp", String(t).c_str());
  delay(2000);
}