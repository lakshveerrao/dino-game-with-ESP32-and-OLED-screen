# Dinosaur Game on ESP32 with OLED

## 🦖 Overview
This project implements a simple dinosaur jumping game on an ESP32 using a 0.96" OLED display (I2C). The dinosaur jumps over obstacles when the push button is pressed.

## 🦖 Components Required
- **ESP32**
- **0.96" OLED Display (I2C, SSD1306)**
- **Push Button** (for jump control)
- **Jumper Wires & Breadboard**

## 🦖 Wiring Diagram
### **Connect OLED to ESP32**
| OLED Pin | ESP32 Pin |
|----------|----------|
| VCC      | 3.3V     |
| GND      | GND      |
| SDA      | GPIO 21  |
| SCL      | GPIO 22  |

⚠ **Note:** The ESP32 operates at **3.3V logic level**, so ensure the OLED is compatible.

### **Connect the Jump Button**
| Button Pin | ESP32 Pin |
|------------|----------|
| One Side to GND |  |
| Other Side to GPIO 15 | *(Configured as INPUT_PULLUP in code)* |

## 🦖 Installing Required Libraries
1. Open **Arduino IDE**.
2. Go to **Sketch → Include Library → Manage Libraries**.
3. Install the following libraries:
   - **Adafruit SSD1306**
   - **Adafruit GFX**

## 🦖 Code Implementation
The following code initializes the OLED display, reads button presses for jumping, applies gravity mechanics, and updates the screen with the game state.

### **Code**
```cpp
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // Not used with ESP32
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define JUMP_BUTTON 15  // Button connected to GPIO 15

bool isJumping = false;
int dinoY = 50;
int gravity = 2;
int jumpVelocity = -10;
int velocity = 0;
int obstacleX = SCREEN_WIDTH;
int score = 0;

void setup() {
  pinMode(JUMP_BUTTON, INPUT_PULLUP);
  Wire.begin(21, 22); // ESP32 I2C SDA=GPIO 21, SCL=GPIO 22
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
}

void loop() {
  // Handle Jump
  if (digitalRead(JUMP_BUTTON) == LOW && !isJumping) {
    isJumping = true;
    velocity = jumpVelocity;
  }

  // Apply Jump Physics
  if (isJumping) {
    dinoY += velocity;
    velocity += gravity;
    if (dinoY >= 50) {
      dinoY = 50;
      isJumping = false;
    }
  }

  // Move Obstacle
  obstacleX -= 4;
  if (obstacleX < -10) {
    obstacleX = SCREEN_WIDTH;
    score++;  // Increase score for successful dodge
  }

  // Collision Detection
  if (obstacleX < 20 && obstacleX > 5 && dinoY > 40) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(15, 30);
    display.println("Game Over!");
    display.display();
    delay(2000);
    obstacleX = SCREEN_WIDTH;
    dinoY = 50;
    score = 0;
  }

  // Clear Display
  display.clearDisplay();
  
  // Draw Dinosaur
  display.fillRect(10, dinoY, 10, 10, SSD1306_WHITE);

  // Draw Ground
  display.drawLine(0, 60, SCREEN_WIDTH, 60, SSD1306_WHITE);

  // Draw Obstacle
  display.fillRect(obstacleX, 50, 10, 10, SSD1306_WHITE);

  // Display Score
  display.setTextSize(1);
  display.setCursor(100, 5);
  display.print("Score: ");
  display.print(score);

  display.display();
  delay(30);
}
```

## 🦖 Uploading and Testing the Code
1. **Connect ESP32** to your PC via USB.
2. Open **Arduino IDE**, and go to **Tools → Board**:
   - Select **ESP32 Dev Module**.
3. Select the correct **COM Port**.
4. Click **Upload** and test the game.

## 🦖 Enhancements & Future Improvements
✅ Increase game speed as score increases.
✅ Improve graphics (use sprites instead of rectangles).
✅ Add sound effects (buzzer on jump/fail).
✅ Store the high score in EEPROM to keep track of progress.

---
🎮 **Enjoy playing your ESP32 Dinosaur Game!** 🚀

