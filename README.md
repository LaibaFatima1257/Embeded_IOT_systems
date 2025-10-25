# Assignment 1 — ESP32 LED & OLED Project
**Name:** Laiba Fatima  
**Registration Number:** 23-ntu-cs-1257  
**Course:** Embedded IoT Systems  

---

## Overview
This project demonstrates the use of an ESP32 microcontroller with LEDs, push buttons, a buzzer, and an OLED display.  
The project contains **two tasks**:

- **Task A:** Cycle through multiple LED modes using buttons and display the current mode on OLED.  
- **Task B:** Detect short and long button presses using a single button to toggle an LED or play a buzzer, with OLED showing the event.

All interactions include **visual feedback (OLED)** and **audio feedback (buzzer)**.

---

## Task A — LED Modes with OLED Display

**Wokwi Link:** [Open Task A in Wokwi](https://wokwi.com/projects/445788205628171265)  

### Pin Mapping

| Component | ESP32 Pin |
|-----------|-----------|
| LED1      | 18        |
| LED2      | 19        |
| LED3      | 21        |
| Mode Button | 23      |
| Reset Button | 25     |
| Buzzer    | 5         |
| OLED SDA  | 14        |
| OLED SCL  | 22        |

### Description
- **Mode Button:** Cycles through 4 LED modes:
  1. Both LEDs OFF
  2. Alternate Blink (LEDs blink alternately)
  3. Both LEDs ON
  4. PWM Fade (LEDs slowly fade in and out)  
- **Reset Button:** Resets LEDs to OFF (Mode 0).  
- **OLED Display:** Shows current mode number and description.  
- **Buzzer:** Beeps shortly when mode changes or reset is pressed.  

### Screenshots
![Circuit Diagram](screenshots/TaskA(Screenshots)/circuitDiagram(taskA).png)  
![Mode 0 - Both OFF](screenshots/TaskA(Screenshots)/MOD0.png)  
![Mode 1 - Alternate Blink Step 1](screenshots/TaskA(Screenshots)/mode1(AlternateBlink1).png)  
![Mode 1 - Alternate Blink Step 2](screenshots/TaskA(Screenshots)/mode1(AlternateBlink2).png)  
![Mode 2 - Both ON](screenshots/TaskA(Screenshots)/mode2(allON).png)  
![Mode 3 - PWM Fade Step 1](screenshots/TaskA(Screenshots)/mode3(Fade1).png)  
![Mode 3 - PWM Fade Step 2](screenshots/TaskA(Screenshots)/mode3(Fade2).png)  

---

## Task B — Single Button Press Detection

**Wokwi Link:** [Open Task B in Wokwi](https://wokwi.com/projects/445794379500897281)  

### Pin Mapping

| Component | ESP32 Pin |
|-----------|-----------|
| Button    | 23        |
| LED1      | 18        |
| Buzzer    | 5         |
| OLED SDA  | 14        |
| OLED SCL  | 22        |

### Description
- **Short Press (<1.5s):** Toggles LED ON/OFF.  
- **Long Press (>1.5s):** Plays buzzer sound for 0.5 seconds.  
- **OLED Display:** Shows "Short press detected" or "Long press detected".  

### Screenshots
![Circuit Diagram](screenshots/TaskB(Screenshots)/circuitDiagram(taskB).png)  
![Ready to Detect](screenshots/TaskB(Screenshots)/READYTODETECT.png)  
![Short Press 1](screenshots/TaskB(Screenshots)/ShortPress1.png)  
![Short Press 2](screenshots/TaskB(Screenshots)/ShortPress2.png)  
![Long Press 1](screenshots/TaskB(Screenshots)/LongPress1.png)  
![Long Press 2](screenshots/TaskB(Screenshots)/LongPress2.png)  

---

## Folder Structure

assignment1-23-ntu-cs-1257/
├─ src/ ← Arduino code files (Task A & B)
├─ include/ ← Optional header/helper files
├─ wokwi/ ← Wokwi Projects Zip Folders
├─ screenshots/
│ ├─ TaskA(Screenshots)/ ← Task A images (LED modes & circuit)
│ └─ TaskB(Screenshots)/ ← Task B images (button press & circuit)
├─ docs/ ← PDF documentation
└─ README.md ← This file


---

## Notes
- Buttons use **internal pull-up resistors** in the code.  
- OLED provides **real-time visual feedback** of LED modes and button events.  
- Buzzer gives **audio feedback** for button interactions.  
- All pin numbers follow the **pin map tables** above.  

---

## How to Use

1. Open **Task A** Wokwi project → test mode cycling using Mode and Reset buttons.  
2. Open **Task B** Wokwi project → test short press to toggle LED and long press to play buzzer.  
3. Refer to **OLED display** for live feedback.  
4. Check screenshots in `/screenshots/TaskA(Screenshots)` and `/screenshots/TaskB(Screenshots)` folders for reference.  

---
