#ifndef LcdMenu_H
#define LcdMenu_H
#include <Arduino.h>

#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

class LcdMenu
{
private:
    LiquidCrystal_I2C *lcd;

    bool selected = false;
    unsigned long millsPress;
    bool cursorInverted = true;
    unsigned long millsBlink; // time from last invert
    int cursorPosition = 0;

    // Custom chars
    /********************/
    byte Speed[8] = {
        B00000,
        B10100,

        B01010,
        B00101,

        B00101,
        B01010,

        B10100,
        B00000};
    byte Speed_Inverted[8]{
        B11111,
        B01011,
        B10101,
        B11010,
        B11010,
        B10101,
        B01011,
        B11111};

    byte IDK[8] = {
        B00000,
        B00100,
        B01010,
        B10001,
        B10001,
        B01010,
        B00100,
        B00000,
    };
    byte IDK_Inverted[8] = {
        B11111,
        B11011,
        B10101,
        B01110,
        B01110,
        B10101,
        B11011,
        B11111,
    };
    byte Check[8] = {
        B00000,
        B00001,
        B00011,
        B10110,
        B11100,
        B01000,
        B00000,
        B00000};
    byte Check_Inverted[8] = {
        B11111,
        B11110,
        B11100,
        B01001,
        B00011,
        B10111,
        B11111,
        B11111};

    byte NOT_DEFINED[8] = {
        B01110,
        B10001,
        B00001,
        B00010,
        B00100,
        B00000,
        B00100,
        B00000};
    byte NOT_DEFINED_INVERTED[8] = {
        B10001,
        B01110,
        B11110,
        B11101,
        B11011,
        B11111,
        B11011,
        B11111};

    byte Bulb[8] = {
        B01110,
        B10001,
        B10001,

        B10001,
        B01010,
        B01010,

        B00100,
        B00000};
    byte Buld_Inverted[8] = {
        B10001,
        B01110,
        B01110,
        B01110,
        B10101,
        B10101,
        B11011,
        B11111};
    /******************/

public:
    LcdMenu(LiquidCrystal_I2C &lcd);

    int pressDelay = 250;
    /***/

    /***/
    int blinkInterval = 500; // blinking interval;

    /***/

    String led_modes[6] = {
        "Default  ", "Blink    ", "Blink*   ", "Fade", "On       ", "Off      "};
    int led_selected_mode = 0;

    int leftButtonPin = 7;
    int rightButtonPin = 8;
    int selectButtonPin = 12;

    void init();
    void idk();
    void drawMenu();
    void update();
    void drawChar(int x, int y, int c);
    void cursorBlink();

    // ??????

    int leds_brightness = 100;
    int leds_speed = 50;

    bool drawing = false;
};
#endif
