#ifndef LcdMenu_H
#define LcdMenu_h
#include <Arduino.h>

#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

class LcdMenu
{

  private:
    LiquidCrystal_I2C* lcd;

  public:
    LcdMenu(LiquidCrystal_I2C &lcd);
    void init();
    void idk();
};
#endif
