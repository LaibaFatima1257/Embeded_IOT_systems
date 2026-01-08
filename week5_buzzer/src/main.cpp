#include <Arduino.h>

#define BUZZER_PIN  27
#define PWM_CH      0
#define FREQ        2000
#define RESOLUTION  10

void setup() {
  ledcSetup(PWM_CH, FREQ, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH);
}

void loop() {
  for (int i = 0; i < 3; i++) {
    ledcWriteTone(PWM_CH, 2000 + i * 400);
    delay(150);
    ledcWrite(PWM_CH, 0);
    delay(150);
  }

  for (int f = 400; f <= 3000; f += 100) {
    ledcWriteTone(PWM_CH, f);
    delay(20);
  }
  ledcWrite(PWM_CH, 0);
  delay(500);

  int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
  for (int i = 0; i < 8; i++) {
    ledcWriteTone(PWM_CH, melody[i]);
    delay(250);
  }
  ledcWrite(PWM_CH, 0);
  delay(1000);
}
