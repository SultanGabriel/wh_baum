
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

int modes[4] = {"Default  ", "Fade     ", "Blink    ", "On       "};
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

// 0:
unsigned long millsBlink = 0;

// 1:
int fadeValue = 0;
int fadeInv = 1;

// blink

bool blinkVal = true;

void loop()
{
    lcdMenu.update();

    if (!lcdMenu.drawing)
    {
        modeHandler();
    }

    if (state.getMode() != lcdMenu.led_selected_mode)
    {
        state.setMode(lcdMenu.led_selected_mode);
    }
    state.setSpeed(1550 - map(lcdMenu.leds_speed, 0, 100, 150, 1500));
    state.setLedBrightness(map(lcdMenu.leds_brightness, 0, 100, 0, 255));
}
int r = 255, g = 0, b = 0;
int rgbIdx = 0;
unsigned long millsRGB;

int BlinkDelay = state.getSpeed();
int led_maxValue = state.getLedBrightness();

void modeHandler()
{
    BlinkDelay = state.getSpeed();
    led_maxValue = state.getLedBrightness();

    Serial.print("Blink Delay: ");
    Serial.println(BlinkDelay);
    Serial.print("Led Max Value: ");
    Serial.println(led_maxValue);

    if (millis() - millsRGB > BlinkDelay)
    {
        millsRGB = millis();
        switch (rgbIdx)
        {
        case 0:
            r = 255;
            g = 0;
            b = 0;

            rgbIdx += 1;
            break;

        case 1:
            r = 0;
            g = 255;
            b = 0;

            rgbIdx += 1;

            break;

        case 2:
            r = 0;
            g = 255;
            b = 255;

            rgbIdx += 1;

            break;

        case 3:
            r = 255;
            g = 0;
            b = 255;
            rgbIdx += 1;

            break;

        case 4:
            r = 0;
            g = 245;
            b = 120;
            rgbIdx += 1;
            break;

        case 5:
            r = 255;
            g = 125;
            b = 70;
            rgbIdx += 1;
            break;
        case 6:
            r = 0;
            g = 0;
            b = 255;
            rgbIdx += 1;

            break;
        }
        rgb_handler(255 - r, 255 - g, 255 - b);
    }
    rgbIdx = rgbIdx % 7;
    // ---
    int mode = state.getMode();
    Serial.print("Mode: ");
    Serial.println(mode);

    int fadeSpeed = map(state.getSpeed(), 100, 0, 1250, 100); // 150; // 100 -> 500 ?

    switch (mode)
    {
    case 0: // RANDOM
        Serial.print("Default");

        //        HC
        // for (uint8_t i = 0; i < 8; i++)
        // {
        //     uint8_t val = (uint8_t)(((float)
        //                                  sin(millis() / 350 + i / 8.0 * 2.0 * PI) +
        //                              1) *
        //                             128);
        //     shiftRegister.set(i, val);
        // }


    case 1:
        Serial.print("Blink");
        if (millis() - millsBlink > BlinkDelay)
        {
            millsBlink = millis();
            if (blinkVal)
            {
                led_handler(led_maxValue);
                hc_handler(led_maxValue);
                blinkVal = false;
            }
            else
            {
                led_handler(0);
                hc_handler(0);
                blinkVal = true;
            }
        }
        break;

    case 2: // Blink*
        Serial.println("Blink*");
        if (millis() - millsBlink > BlinkDelay)
        {
            millsBlink = millis();
            if (blinkVal)
            {
                led_handler(0);
                hc_handler(led_maxValue);
                blinkVal = false;
            }
            else
            {
                led_handler(led_maxValue);
                hc_handler(0);
                blinkVal = true;
            }
        }
        break;
    case 3: // FADE
        Serial.print("FADE");
        int fadeInc = map(state.getSpeed(), 0, 100, 1, 5);

        fadeValue += fadeInc * fadeInv;

        if (fadeValue > led_maxValue)
        {
            fadeInv = fadeInv * -1;
            fadeValue = led_maxValue;
        }
        else if (fadeValue < 0)
        {
            fadeInv = fadeInv * -1;
            fadeValue = 0;
        }

        hc_handler(fadeValue);
        led_handler(fadeValue);

        break;

    case 4: // On
        Serial.print("On");

        led_handler(255);
        hc_handler(255);

        break;

    case 5: // Off
        Serial.print("Off");

        led_handler(0);
        hc_handler(0);
        break;

    default:

        Serial.write("Error default case triggered ..");
        break;
    }
}
