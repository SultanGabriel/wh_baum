
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <ShiftRegisterPWM.h>

#include "Led.h"
#include "LcdMenu.h"
#include "State.h"

/*  Point of the project: Led Controlling system -- Designed to go on a fake

  ----------------------Wiring---------------------------

    LCD // Wiring:
        --> SDA: A4
        --> SCL: A5

    Buttons : // Wiring:
        --> Left Button: 7
        --> Right Button: 8
        --> Select Button: 12

    74HC595 // Wiring:
        --> SHCP: 4
        --> STCP: 3
        --> DS: 2

    12V LEDS // Wiring:
        --> 6

    RGB LEDS // Wiring:
        --> R:
        --> G:
        --> B:

  -------------------------------------------------------
*/
/*
    Let's Talk next Step

        thinking about how to fucking control the leds


*/

#define leftButtonPin 7
#define rightButtonPin 8
#define selectButtonPin 12

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

LcdMenu lcdMenu(lcd);

ShiftRegisterPWM shiftRegister(1, 255);

State state = State();
// Led led1(5);

int PIN_SHCP = 4;
int PIN_STCP = 3;
int PIN_DS = 2;

int PIN_LED = 6;
int PINS_RGB[3] = {9, 10, 11};

int modes[5] = {"Fade", "Blink", "Shift", "Random", "Off"};
int LED_modeIndex = 0;

void setup()
{

    // init pins

    pinMode(leftButtonPin, INPUT);   // LEFT
    pinMode(rightButtonPin, INPUT);  // RIGHT
    pinMode(selectButtonPin, INPUT); // SELECT

    // 74HC595 Initialization
    shiftRegister.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);

    pinMode(PIN_SHCP, OUTPUT);
    pinMode(PIN_STCP, OUTPUT);
    pinMode(PIN_DS, OUTPUT);

    // 12v Leds
    for (int i = 0; i < sizeof(PINS_RGB); i++)
    {
        pinMode(PINS_RGB[i], OUTPUT);
    }

    // PWM Initialization

    // led1.init();

    // -------INIT LCD:

    lcd.init();
    lcd.backlight();
    lcd.noCursor();

    // Initialize Setup

    lcdMenu.init();

    lcdMenu.drawMenu();

    // Initialise state !

    state = State();

    //-----

    Serial.begin(9600);
}
int led_handler(int val)
{

    analogWrite(PIN_LED, val);

    return 0;
}

int hc_handler(int val)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        shiftRegister.set(i, val);
    }
    return 0;
}

int rgb_handler(int r, int g, int b)
{
    analogWrite(PINS_RGB[0], r);
    analogWrite(PINS_RGB[1], g);
    analogWrite(PINS_RGB[2], b);
}

void loop()
{
    modeHandler();

    lcdMenu.update();

    if (state.getMode() != lcdMenu.led_selected_mode)
    {
        state.setMode(lcdMenu.led_selected_mode);
    }
    state.setSpeed(lcdMenu.leds_speed);

    // hc_handler();
    // led_handler();
}

// 0:
unsigned long millsBlink;
int BlinkDelay = 750; // 1500 * (state.getSpeed() / 100);
int blinkValue = 170;
// 1:
int fadeValue = 0;
int fadeInv = 1;

void modeHandler()
{
    // int fadeValue = 0;

    int mode = state.getMode();
    Serial.println(mode);

    int fadeSpeed = map(state.getSpeed(), 0, 100, 1250, 100); // 150; // 100 -> 500 ?
    switch (mode)
    {
    case 0: // RANDOM

        rgb_handler(255 - state.Led_Rgb[0], 255 - state.Led_Rgb[1], 255 - state.Led_Rgb[2]);

        // HC
        for (uint8_t i = 0; i < 8; i++)
        {
            uint8_t val = (uint8_t)(((float)
                                         sin(millis() / BlinkDelay + i / 8.0 * 2.0 * PI) +
                                     1) *
                                    128);
            shiftRegister.set(i, val);
        }

        // 12v Leds
        if (millis() - millsBlink > BlinkDelay)
        {
            if (blinkValue == 170)
            {
                blinkValue = 0;
            }
            else
            {
                blinkValue = 170;
            }
            led_handler(blinkValue);
            millsBlink = millis();
        }

        break;

    case 1: // FADE

        int fadeInc = map(state.getSpeed(), 0, 100, 1, 5);

        fadeValue += fadeInc * fadeInv;

        if (fadeValue > 255)
        {
            fadeInv = fadeInv * -1;
            fadeValue = 255;
        }
        else if (fadeValue < 0)
        {
            fadeInv = fadeInv * -1;
            fadeValue = 0;
        }

        hc_handler(fadeValue);
        led_handler(fadeValue);

        break;

    case 3:
        for (uint8_t i = 0; i < 8; i++)
        {
            shiftRegister.set(i, 0);
        }

        break;

    default:
        Serial.println("Current Mode: " + String(mode));
        break;
    }
}
