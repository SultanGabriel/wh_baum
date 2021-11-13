#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD


// Custom chars
/********************/


byte Heart[] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000
};
byte Speed[] = {
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B01010,
  B00100,
  B00000,
};
byte Speed_Inverted[] = {
  B11111,
  B11011,
  B10101,
  B01110,
  B01110,
  B10101,
  B11011,
  B11111,
};
byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};
byte Check_Inverted[] = {
  B11111,
  B11110,
  B11100,
  B01001,
  B00011,
  B10111,
  B11111,
  B11111
};
byte Sound[] = {
  B00001,
  B00011,
  B00101,
  B01001,
  B01001,
  B01011,
  B11011,
  B11000
};
byte NOT_DEFINED[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

/******************/

/*  TODO
      -> connect
          - LCD
          - 74HC595

          LCD // Wiring: SDA pin is connected to A4 and SCL pin to A5.
*/

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

void setup() {
  // init pins

  pinMode(1, INPUT);  // LEFT
  pinMode(2, INPUT);  // RIGHT
  pinMode(3, INPUT);  // SELECT

  // init display and custom chars

  // -------INIT LCD:

  lcd.init();
  lcd.backlight();
  lcd.cursor();

  // -------INIT Chars

sss
  lcd.createChar(1, Heart);
  lcd.createChar(2, NOT_DEFINED);
  lcd.createChar(3, Check);
  lcd.createChar(4, Speed_Inverted);
  lcd.createChar(5, NOT_DEFINED);
  lcd.createChar(6, NOT_DEFINED);
  lcd.createChar(7, Check_Inverted);

  //----------



}
int lcd_cursorPos = 0;

void lcd_update() {
  lcd.setCursor(2, 0);
  lcd.print("Hello World!");

  lcd_drawMenu();

  /*  int c = 0;
    for (int pos = 0; pos < 16; pos += 2) {
      lcd_drawChar(pos, 1, c);
      if (c < 6) {
        c = c + 1;
      }
    }*/
}

void lcd_drawMenu() {
  int c = 0;
  for (int pos = 0; pos < 8; pos += 2) {
    if (lcd_cursorPos * 2 == pos) {
      lcd_drawChar(pos, 1, c + 4);
    } else {
      lcd_drawChar(pos, 1, c);
    }
    if (c < 4) {
      c = c + 1;
    }
  }
}

void lcd_drawChar(int x, int y, int c) {
  // POSITION X , Y ------ custom character code 0-7

  lcd.setCursor(x, y);
  lcd.write(c);
}

void loop() {
  lcd_update();
}
