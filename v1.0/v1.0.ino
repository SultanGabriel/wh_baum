#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <ShiftRegisterPWM.h>

#include "Led.h"
#include "LcdMenu.h"

/*  Point of the project: Led Controlling system -- Designed to go on a fake

  ----------------------Wiring---------------------------

    LCD // Wiring:
        --> SDA: A4
        --> SCL: A5

    Buttons : // Wiring:
        --> Left Button: 2
        --> Right Button: 4
        --> Select Button: 7

    74HC595 // Wiring:
        --> SHCP: 12
        --> STCP: 8
        --> DS: 11

  -------------------------------------------------------
*/

#define leftButtonPin 7
#define rightButtonPin 8
#define selectButtonPin 12

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

LcdMenu lcdMenu(lcd);

ShiftRegisterPWM shiftRegister(1, 16);

// Led led1(5);

int PIN_SHCP = 4;
int PIN_STCP = 3;
int PIN_DS = 2;

int PINS_PWM[5] = {5, 6, 9, 10, 11};
int PINS_PWM_VALUES[5] = {255, 255, 255, 255, 255};

int modes[5] = {"Fade", "Blink", "Shift", "Random", "Off"};
int LED_modeIndex = 0;

int fadeDelay = 250;

void setup()
{

    // init pins

    pinMode(leftButtonPin, INPUT);   // LEFT
    pinMode(rightButtonPin, INPUT);  // RIGHT
    pinMode(selectButtonPin, INPUT); // SELECT

    // 74HC595 Initialization
    shiftRegister.interrupt(ShiftRegisterPWM::UpdateFrequency::Fast);
    pinMode(PIN_SHCP, OUTPUT);
    pinMode(PIN_STCP, OUTPUT);
    pinMode(PIN_DS, OUTPUT);

    for (int i = 0; i < sizeof(PINS_PWM); i++)
    {
        pinMode(PINS_PWM, OUTPUT);
    }
    // PWM Initialization

    // led1.init();

    // -------INIT LCD:

    lcd.init();
    lcd.backlight();
    lcd.noCursor();

    // Initialize Setup

    //lcd_drawMenu();

    // -- Debug way
    lcdMenu.init();

    lcdMenu.drawMenu();

    Serial.begin(9600);
}

void loop()
{
    lcdMenu.update();

    switch (LED_modeIndex)
    {
    case 0:
        for (uint8_t i = 0; i < 8; i++)
        {
            uint8_t val = (uint8_t)(((float)
                                         sin(millis() / fadeDelay + i / 8.0 * 2.0 * PI) +
                                     1) *
                                    128);
            shiftRegister.set(i, val);
        }
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        for (uint8_t i = 0; i < 8; i++)
        {
            shiftRegister.set(i, 0);
        }
    }
}

// FUNCTIONS TO CONTROL LEDS

/*

*/
