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

PWM // Wiring:  --think-- PWM LEFT : 5 /6 9 10 11
        --> 1.
        --> 2.
        --> 3.
        --> 4.

    12V LEDS // Wiring:
        --> 6

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

int PINS_PWM[5] = {5, 6, 9, 10, 11};
int PINS_PWM_VALUES[5] = {255, 255, 255, 255, 255};

int PIN_LED = 6;

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

    lcdMenu.init();

    lcdMenu.drawMenu();

    // Initialise state !

    state = State();

    //-----

    Serial.begin(9600);
}
int led_handler(int val)
{
    //     switch (mode)
    //     {
    //     case 0: // RANDOM
    //         analogWrite(PIN_LED, 0);
    //         break;

    //     case 1: // FADE

    //         int fadeInc = map(state.getSpeed(), 0, 100, 1, 5);

    //         fadeValue += fadeInc * fadeInv;

    //         if (fadeValue > 255)
    //         {
    //             fadeInv = fadeInv * -1;
    //             fadeValue = 255;
    //         }
    //         else if (fadeValue < 0)
    //         {
    //             fadeInv = fadeInv * -1;
    //             fadeValue = 0;
    //         }

    //         analogWrite(PIN_LED, fadeValue);

    //         break;

    //     case 3:

    //         break;

    //     default:
    //         Serial.println("Current Mode: " + String(mode));
    //         break;
    //     }

    analogWrite(PIN_LED, val);

    return 0;
}
int fadeValue = 0;
int fadeInv = 1;
int hc_handler(int val)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        shiftRegister.set(i, val);
    }
    return 0;
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

/*
int PWM_HANDLER()
{
    return 0;
}
*/

void modeHandler()
{
    // int fadeValue = 0;

    int mode = state.getMode();
    Serial.println(mode);

    int fadeSpeed = map(state.getSpeed(), 0, 100, 1250, 100); //150; // 100 -> 500 ?
    switch (mode)
    {
    case 0: // RANDOM

        // for (uint8_t i = 0; i < 8; i++)
        // {
        //     uint8_t val = (uint8_t)(((float)
        //                                  sin(millis() / fadeSpeed + i / 8.0 * 2.0 * PI) +
        //                              1) *
        //                             128);
        //     shiftRegister.set(i, val);
        // }
        break;

    case 1: // FADE

        // shiftRegister.set(0, 50);
        // shiftRegister.set(1, 100);
        // shiftRegister.set(2, 150);
        // shiftRegister.set(3, 175);
        // shiftRegister.set(4, 200);
        // shiftRegister.set(5, 220);
        // shiftRegister.set(6, 255);
        // shiftRegister.set(7, 255);

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

        Serial.println(fadeValue);

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

void writeLeds()
{
}

void writeHc()

{
}