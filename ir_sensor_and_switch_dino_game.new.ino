#include <Wire.h>
#include <U8g2lib.h>

#define IR_SENSOR 15      // IR sensor for jumping
#define PAUSE_SWITCH 14   // Switch for pausing the game

bool isJumping = false;
bool isPaused = false;
int dinoY = 50;
int gravity = 2;
int jumpVelocity = -10;
int velocity = 0;
int obstacleX = 128;
int score = 0;

// Initialize OLED display
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  pinMode(IR_SENSOR, INPUT);          // IR sensor input
  pinMode(PAUSE_SWITCH, INPUT_PULLUP); // Switch input with internal pull-up
  
  Wire.begin(21, 22);  // ESP32 I2C SDA=21, SCL=22
  u8g2.begin();
}

void loop() {
  // Handle Pause/Resume Switch
  if (digitalRead(PAUSE_SWITCH) == LOW) {
    isPaused = !isPaused;
    delay(500); // Debounce delay
  }

  // Show Pause Screen if Paused
  if (isPaused) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(35, 30, "PAUSED");
    u8g2.drawBox(50, 40, 8, 20);  // Pause symbol ||
    u8g2.drawBox(70, 40, 8, 20);
    u8g2.sendBuffer();
    
    while (digitalRead(PAUSE_SWITCH) == HIGH);  // Wait until switch is toggled again
    return;  // Skip the game loop while paused
  }

  // IR Sensor for Jumping
  if (digitalRead(IR_SENSOR) == LOW) {  // Hand detected
    isJumping = true;
    velocity = jumpVelocity;
  }

  // Apply Gravity
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
    obstacleX = 128;
    score++;  // Increase score
  }

  // Collision Detection (Game Over)
  if (obstacleX < 20 && obstacleX > 5 && dinoY > 40) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(20, 30, "Game Over!");
    u8g2.sendBuffer();

    while (digitalRead(PAUSE_SWITCH) == HIGH);  // Wait for switch to restart
    score = 0;
    obstacleX = 128;
    dinoY = 50;
    return;
  }

  // Draw Frame
  u8g2.clearBuffer();

  // Draw Play Icon (top right corner)
  u8g2.drawTriangle(110, 5, 110, 15, 120, 10);

  // Draw Dinosaur
  u8g2.drawBox(10, dinoY, 10, 10);

  // Draw Ground
  u8g2.drawLine(0, 60, 128, 60);

  // Draw Obstacle
  u8g2.drawBox(obstacleX, 50, 10, 10);

  // Display Score
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setCursor(90, 10);
  u8g2.print("Score: ");
  u8g2.print(score);

  // Send buffer to display
  u8g2.sendBuffer();
  delay(30);
}
