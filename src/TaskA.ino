// Name: Laiba Fatima
// Reg# 23-ntu-cs-1257
// Question3_TaskA(Assignment1)
// Subject: Embedded IOT Systems

#include <Arduino.h>           // Basic Arduino functions
#include <Wire.h>              // For I2C communication with OLED
#include <Adafruit_GFX.h>      // Graphics library for OLED
#include <Adafruit_SSD1306.h>  // OLED driver library

// OLED setup
#define SCREEN_WIDTH 128       // OLED width in pixels
#define SCREEN_HEIGHT 64       // OLED height in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Create OLED object

// Pin mapping
#define OLED_SDA 14            // OLED SDA pin
#define OLED_SCL 22            // OLED SCL pin
#define LED1 18                // LED1 pin
#define LED2 19                // LED2 pin
#define LED3 21                // LED3 pin
#define MODE_BUTTON 23         // Button to change mode
#define RESET_BUTTON 25        // Button to reset mode
#define BUZZER 5               // Buzzer pin

// Variables
int mode = 0;                  // 0=OFF, 1=Alternate Blink, 2=Both ON, 3=PWM Fade
int lastModeBtn = HIGH;        // Stores previous state of MODE button
int lastResetBtn = HIGH;       // Stores previous state of RESET button

// Function to show current mode on OLED
void showMode() {
  display.clearDisplay();      // Clear previous text
  display.setTextSize(1);      // Set text size
  display.setTextColor(SSD1306_WHITE); // White color text
  display.setCursor(10, 10);   // Set cursor position
  display.print("Mode ");       // Print "Mode "
  display.print(mode);          // Print current mode number
  display.setCursor(10, 30);   // Move to next line

  // Show mode description
  switch (mode) {
    case 0: display.print("Both OFF"); break;
    case 1: display.print("Alternate Blink"); break;
    case 2: display.print("Both ON"); break;
    case 3: display.print("PWM Fade"); break;
  }
  display.display();            // Update OLED screen
}

void setup() {
  // Set pins mode
  pinMode(LED1, OUTPUT);        
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(MODE_BUTTON, INPUT_PULLUP); // Button with internal pull-up
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);

  // Initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL); // Start I2C for OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed"); // Stop if OLED not working
    for(;;);
  }

  showMode(); // Show initial mode on OLED
}

void loop() {
  int modeBtn = digitalRead(MODE_BUTTON);   // Read MODE button
  int resetBtn = digitalRead(RESET_BUTTON); // Read RESET button

  // MODE button pressed (short press)
  if (modeBtn == LOW && lastModeBtn == HIGH) { 
    mode++;                     // Go to next mode
    if (mode > 3) mode = 0;     // Wrap around after mode 3
    tone(BUZZER, 1000, 100);    // Beep buzzer shortly
    showMode();                 // Update OLED
    delay(200);                 // Debounce delay
  }
  lastModeBtn = modeBtn;        // Save current button state

  // RESET button pressed
  if (resetBtn == LOW && lastResetBtn == HIGH) {
    mode = 0;                   // Reset mode to 0
    tone(BUZZER, 500, 200);     // Beep buzzer
    showMode();                 // Update OLED
    delay(200);                 // Debounce delay
  }
  lastResetBtn = resetBtn;      // Save current button state

  // Behaviour for each mode
  switch (mode) {
    case 0: // All LEDs OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      break;

    case 1: // Alternate blink
      digitalWrite(LED1, HIGH); // LED1 ON
      digitalWrite(LED2, LOW);  // LED2 OFF
      digitalWrite(LED3, LOW);  // LED3 OFF
      delay(300);               // Wait 0.3 sec
      digitalWrite(LED1, LOW);  // LED1 OFF
      digitalWrite(LED2, HIGH); // LED2 ON
      digitalWrite(LED3, HIGH); // LED3 ON
      delay(300);
      break;

    case 2: // Both ON
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      break;

    case 3: // PWM Fade for all LEDs
      for (int i = 0; i <= 255; i++) {     // Increase brightness
        analogWrite(LED1, i);
        analogWrite(LED2, i);
        analogWrite(LED3, i);
        delay(5);
      }
      for (int i = 255; i >= 0; i--) {     // Decrease brightness
        analogWrite(LED1, i);
        analogWrite(LED2, i);
        analogWrite(LED3, i);
        delay(5);
      }
      break;
  }
}
