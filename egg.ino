#include <Wire.h>
#include <U8g2lib.h>


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


const int BUTTON_LEFT = 35;
const int BUTTON_RIGHT = 33;
const int BUTTON_SELECT = 32;


enum GameState { MENU, PLAYING };
GameState gameState = MENU;


const char* modes[] = {"EASY", "MEDIUM", "HARD"};
int selectedMode = 0;
int eggSpeed = 50;


int basketX = 50;
const int basketWidth = 20;
const int basketY = 60;


int eggX;
int eggY = 0;
bool eggCaught = false;
int score = 0;

void setup() {
  u8g2.begin();
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  randomSeed(analogRead(0));
  eggX = random(0, 128 - 5);
}

void loop() {
  if (gameState == MENU) {
    handleMenu();
  } else if (gameState == PLAYING) {
    handleGame();
  }
}

void handleMenu() {
  if (digitalRead(BUTTON_LEFT) == LOW) {
    selectedMode = (selectedMode + 2) % 3;
    delay(200);
  }
  if (digitalRead(BUTTON_RIGHT) == LOW) {
    selectedMode = (selectedMode + 1) % 3;
    delay(200);
  }
  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (selectedMode == 0) eggSpeed = 100;
    else if (selectedMode == 1) eggSpeed = 50;
    else eggSpeed = 30;
    gameState = PLAYING;
    delay(200);
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setCursor(10, 15);
  u8g2.print("Select Difficulty");

  for (int i = 0; i < 3; i++) {
    int x = 20 + i * 40;
    int y = 40;
    if (i == selectedMode) {
      u8g2.drawBox(x - 2, y - 10, 30, 15);
      u8g2.setDrawColor(0);
      u8g2.setCursor(x, y);
      u8g2.print(modes[i]);
      u8g2.setDrawColor(1);
    } else {
      u8g2.setCursor(x, y);
      u8g2.print(modes[i]);
    }
  }

  u8g2.sendBuffer();
}



void handleGame() {
  static unsigned long remenu = 0;

  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (remenu == 0) remenu = millis();
    if (millis() - remenu >= 3000) {
      gameState = MENU;
      remenu = 0;
      eggY = 0; score = 0; basketX = 50;
      while (digitalRead(BUTTON_SELECT) == LOW) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_5x8_tr);
        u8g2.setCursor(0, 10);
        u8g2.print("return to menu... "); 
        u8g2.setCursor(20, 25);
        u8g2.print("Release the button."); 
        u8g2.sendBuffer();
        delay(10);
        }
        return;
    }
  } else {
    remenu = 0;
  }
  if (digitalRead(BUTTON_LEFT) == LOW && basketX > 0) {
    basketX -= 2;
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && basketX < (128 - basketWidth)) {
    basketX += 2;
  }

  eggY += 2;
  if (eggY >= basketY - 2 && eggY <= basketY + 4) {
    if (eggX >= basketX && eggX <= basketX + basketWidth) {
      score++;
      eggCaught = true;
    }
  }

  if (eggY > 64 || eggCaught) {
    eggY = 0;
    eggX = random(0, 128 - 5);
    eggCaught = false;
  }

  u8g2.clearBuffer();
  u8g2.drawDisc(eggX, eggY, 2); 
  u8g2.drawBox(basketX, basketY, basketWidth, 4); 
  u8g2.setFont(u8g2_font_5x8_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Score: ");
  u8g2.print(score);

  u8g2.sendBuffer();
  delay(eggSpeed);
}
