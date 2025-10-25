// Name: Laiba Fatima
// Reg# 23-ntu-cs-1257
// Question3_TaskB(Assignment1)
// Subject: Embedded IOT Systems

#include <Arduino.h>           // Basic Arduino functions
#include <Wire.h>              // For communication with OLED
#include <Adafruit_GFX.h>      // Helps draw on OLED
#include <Adafruit_SSD1306.h>  // OLED display driver

// OLED setup
#define SCREEN_WIDTH 128       // OLED width
#define SCREEN_HEIGHT 64       // OLED height
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Create OLED object

// Pins
#define BUTTON 23              // Button pin
#define LED1 18                // LED pin
#define BUZZER 5               // Buzzer pin

// Variables
bool ledState = false;         // LED ON or OFF
unsigned long buttonPressTime = 0; // When button was pressed
bool buttonHeld = false;       // Is button being held?
const unsigned long LONG_PRESS_TIME = 1500; // 1.5 seconds for long press

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);  // Button input with pull-up resistor
  pinMode(LED1, OUTPUT);          // LED output
  pinMode(BUZZER, OUTPUT);        // Buzzer output

  Wire.begin(14, 22);             // Start OLED I2C: SDA=14, SCL=22

  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed"); // Stop if OLED not working
    for(;;);
  }

  // Show "Ready..." on OLED
  display.clearDisplay();         
  display.setTextSize(1);       
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("Ready...");
  display.display();
}

void loop() {
  int buttonState = digitalRead(BUTTON); // Read button state

  // Button pressed now
  if (buttonState == LOW && !buttonHeld) {
    buttonPressTime = millis(); // Remember time of press
    buttonHeld = true;          // Mark button as held
  }

  // Button released now
  if (buttonState == HIGH && buttonHeld) {
    unsigned long pressDuration = millis() - buttonPressTime; // How long button was pressed
    buttonHeld = false;          // Reset held flag

    display.clearDisplay();      // Clear OLED
    display.setCursor(10, 10);   // Set text position

    if (pressDuration >= LONG_PRESS_TIME) {
      // Long press
      display.println("Long press detected"); // Show message
      display.display();
      tone(BUZZER, 1000, 500);               // Play sound for 0.5 sec
    } else {
      // Short press
      display.println("Short press detected"); // Show message
      display.display();
      ledState = !ledState;                   // Switch LED ON/OFF
      digitalWrite(LED1, ledState);           // Apply new LED state
    }

    delay(200); // Small wait to avoid double detection
  }
}
