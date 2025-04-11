#include <Wire.h>
#include <U8g2lib.h>


U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);


const int BUTTON_LEFT = 35;
const int BUTTON_RIGHT = 33;
const int BUTTON_SELECT = 32;


enum GameState { MENU, PLAYING };
GameState gameState = MENU;

unsigned long leftRightHoldStart = 0;
bool specialModeMenuActive = false;

const char* modes[] = {"EASY", "MEDIUM", "HARD"};
int selectedMode = 0;
int eggSpeed = 50;

int basketX = 50;
const int basketWidth = 20;
const int basketY = 60;

bool timerEnabled = false;
unsigned long gameStartTime;
const int gameDuration = 30000;

int minScore = 5;

int eggX;
int eggY = 0;
bool eggCaught = false;
int score = 0;

enum SpecialMode {
  NONE,
  SUPER_HARD_MODE,
  ULTIMATE_MODE
};
SpecialMode currentMode = NONE;

void showSpecialModeMenu() {
  int selected = 0;
  gameStartTime = millis();
  while (true) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(20, 12, "== Special Mode ==");
    u8g2.drawStr(25, 30, selected == 0 ? "> Super Hard" : "  Super Hard");
    u8g2.drawStr(25, 45, selected == 1 ? "> Ultimate" : "  Ultimate");
    u8g2.sendBuffer();

    if (digitalRead(BUTTON_RIGHT) == LOW) selected = (selected + 1) % 2;
    if (digitalRead(BUTTON_LEFT) == LOW) selected = (selected + 1) % 2;

    if (digitalRead(BUTTON_SELECT) == LOW) {
      currentMode = selected == 0 ? SUPER_HARD_MODE : ULTIMATE_MODE;

      if (currentMode == SUPER_HARD_MODE) {
        eggSpeed = 30;
        minScore = 15;
      } else if (currentMode == ULTIMATE_MODE) {
        eggSpeed = 12;
        minScore = 20;
      }

      gameState = PLAYING;
      break;
    }

    delay(150);
  }
}

void showGameOverScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setCursor(30, 20);
  u8g2.print("Game Over!");
  u8g2.setCursor(20, 35);
  u8g2.print("Score: ");
  u8g2.print(score);
  u8g2.setCursor(20, 50);
  u8g2.print("Min Score: ");
  u8g2.print(minScore);
  u8g2.sendBuffer();
  delay(5000);
  score = 0;
}

void setup() {
  u8g2.begin();
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
  randomSeed(analogRead(0));
  eggX = random(0, 128 - 5);
}

void loop() {
  bool leftPressed = digitalRead(BUTTON_LEFT) == LOW;
  bool rightPressed = digitalRead(BUTTON_RIGHT) == LOW;

  if (leftPressed && rightPressed) {
    if (leftRightHoldStart == 0) {
      leftRightHoldStart = millis();
    } else if (millis() - leftRightHoldStart >= 3000) {
      specialModeMenuActive = true;
      showSpecialModeMenu();
      leftRightHoldStart = 0;
    }
  } else {
    leftRightHoldStart = 0;
  }

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
    askEnableTimer();
    delay(200);
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tr);
  u8g2.drawFrame(0, 0, 128, 64);
  u8g2.setCursor(24, 15);
  u8g2.print("Catch The Egg!");
  u8g2.setCursor(22, 35);
  u8g2.print("Difficulty:");
  u8g2.setFont(u8g2_font_7x14B_tr);
  String displayMode = String("> ") + modes[selectedMode] + " <";
  int textWidth = u8g2.getStrWidth(displayMode.c_str());
  u8g2.setCursor((128 - textWidth) / 2, 52);
  u8g2.print(displayMode);
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.setCursor(8, 62);
  u8g2.print("Hold LEFT + RIGHT 3s for special mode");
  u8g2.sendBuffer();
}

void askEnableTimer() {
  bool choosing = true;
  bool choice = false;
  unsigned long buttonPressStart = 0;
  bool buttonHeld = false;

  while (choosing) {
    u8g2.clearBuffer();
    u8g2.drawFrame(0, 0, 128, 64);
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.setCursor(10, 20);
    u8g2.print("Enable Timer?");
    u8g2.setCursor(20, 40);
    u8g2.print(choice ? " > Yes" : "   Yes");
    u8g2.setCursor(20, 50);
    u8g2.print(!choice ? " > No" : "   No");
    u8g2.sendBuffer();

    if (digitalRead(BUTTON_SELECT) == LOW) {
      if (!buttonHeld) {
        buttonPressStart = millis();
        buttonHeld = true;
      } else if (millis() - buttonPressStart >= 2000) {
        timerEnabled = choice;
        choosing = false;
        u8g2.clearBuffer();
        u8g2.setCursor(20, 0);
        u8g2.print("Release the button. to start game !");
        u8g2.sendBuffer();
        while (digitalRead(BUTTON_SELECT) == LOW);
        gameState = PLAYING;
        delay(500);
      }
    } else {
      if (buttonHeld) {
        if (millis() - buttonPressStart < 1000) {
          choice = !choice;
          delay(200);
        }
        buttonHeld = false;
      }
    }
  }
}

void handleGame() {
  static bool started = false;
  static unsigned long remenu = 0;

  if (!started) {
    gameStartTime = millis();
    started = true;
  }

  if (timerEnabled) {
    int remaining = (gameDuration - (millis() - gameStartTime)) / 1000;
    if (remaining <= 0) {
      if (score < minScore) {
        showGameOverScreen();
        gameState = MENU;
        started = false;
        return;
      }
    }
  }

  if (digitalRead(BUTTON_SELECT) == LOW) {
    if (remenu == 0) remenu = millis();
    if (millis() - remenu >= 3000) {
      gameState = MENU;
      remenu = 0;
      eggY = 0;
      score = 0;
      basketX = 50;
      started = false;
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
    basketX -= 3;
  }
  if (digitalRead(BUTTON_RIGHT) == LOW && basketX < (128 - basketWidth)) {
    basketX += 3;
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

  if (timerEnabled) {
    u8g2.setCursor(90, 10);
    u8g2.print("Time:");
    u8g2.print((gameDuration - (millis() - gameStartTime)) / 1000);
  }

  u8g2.sendBuffer();
  delay(eggSpeed);
}
