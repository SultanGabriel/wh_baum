#include "LcdMenu.h"
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

LcdMenu::LcdMenu(LiquidCrystal_I2C &lcd)
{
  // Use 'this->' to make the difference between the
  // 'pin' attribute of the class and the
  // local variable 'pin' created from the parameter.
  this->lcd = &lcd;
  //init();
}
void LcdMenu::init()
{

}

void LcdMenu::idk() 
{
  lcd->setCursor(0, 0);
  lcd->print("Hello World!");
}
