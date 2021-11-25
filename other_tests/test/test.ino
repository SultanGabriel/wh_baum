#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

#include "LcdMenu.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

LcdMenu menu(lcd);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.noCursor();

  menu.idk();

}

void loop() {
  //lcd.setCursor(0, 0);
  //lcd.print("Hello World!");

  
}
