here is the code:
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
end of the code

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

