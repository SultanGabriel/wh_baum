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

    PWM // Wiring:  --think-- PWM LEFT : 5 6 9 10 11
        --> 1.
        --> 2.
        --> 3.
        --> 4.

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

ShiftRegisterPWM shiftRegister(1, 16);

State state = State();
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

    lcdMenu.init();

    lcdMenu.drawMenu();

    // temporary function pointers

    int (*LED_POINTER)(){&LED_HANDLER};
    int (*PWM_POINTER)(){&PWM_HANDLER};
    int (*HC_POINTER)(){&HC_HANDLER};

    // Declare state !

    state = State(LED_POINTER, PWM_POINTER, HC_POINTER);

    //-----

    Serial.begin(9600);
}
int LED_HANDLER()
{
    return 0;
}
int PWM_HANDLER()
{
    return 0;
}
int HC_HANDLER()
{
    int mode = state.getMode();
    switch (mode)
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

    return 0;
}

void loop()
{
    state.updateHandlers();
    lcdMenu.update();

    state.setMode(lcdMenu.led_selected_mode);
}

// FUNCTIONS TO CONTROL LEDS

/*

*/
