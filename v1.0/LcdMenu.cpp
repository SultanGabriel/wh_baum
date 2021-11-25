#include "LcdMenu.h"
#include <Wire.h>              // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD

LcdMenu::LcdMenu(LiquidCrystal_I2C &lcd)
{
    // Use 'this->' to make the difference between the
    // 'pin' attribute of the class and the
    // local variable 'pin' created from the parameter.
    this->lcd = &lcd;

    // init();
}
void LcdMenu::init()
{
    // -------INIT Chars

    lcd->createChar(0, Speed);
    lcd->createChar(1, Bulb);
    lcd->createChar(2, IDK); // TODO Think of something to put here
    lcd->createChar(3, Check);

    // INVERTED

    lcd->createChar(4, Speed_Inverted);
    lcd->createChar(5, Buld_Inverted);
    lcd->createChar(6, IDK_Inverted);
    lcd->createChar(7, Check_Inverted);
}

void LcdMenu::idk()
{
    lcd->setCursor(0, 0);
    lcd->print("Hello World!");
}

void LcdMenu::update()
{
    // Button Press Handler
    // - -- --- --- -- -
    // LEFT
    if (!selected)
    {
        if (digitalRead(leftButtonPin) == 1 && millis() - millsPress > pressDelay)
        {
            millsPress = millis();
            //    Serial.write("LEFT\n");

            if (cursorPosition == 0)
            {
                cursorPosition = 3;
            }
            else
            {
                cursorPosition = (cursorPosition - 1) % 4;
            }

            drawMenu();
        }

        // RIGHT
        if (digitalRead(rightButtonPin) == 1 && millis() - millsPress > pressDelay)
        {
            millsPress = millis();
            //   Serial.write("RIGHT\n");
            cursorPosition = (cursorPosition + 1) % 4;

            drawMenu();
        }
    }

    // Select
    if (digitalRead(selectButtonPin) == 1 && millis() - millsPress > pressDelay)
    {
        millsPress = millis();

        selected = !selected;

        drawMenu();
        //    Serial.write(cursorPosition);
        //    Serial.write("SELECT\n");
    }

    /* --- --- --- --- --- --- --- --- --- --- --- --- ---*/
    // print information about selected option + control
    if (selected)
    {
        switch (cursorPosition)
        {

            //——————————————————Speed——————————————————//
            // speed of leds blinking or fading - 0%-100%

        case 0:
            // Serial.write("pos = 0");

            lcd->setCursor(0, 0);
            if (digitalRead(leftButtonPin) == 1 && leds_speed != 0 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                leds_speed = leds_speed - 2;
            }
            else if (digitalRead(rightButtonPin) == 1 && leds_speed != 100 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                leds_speed = leds_speed + 2;
            }

            lcd->print("1. Speed: " + String(leds_speed) + "%   ");
            break;
            //——————————————————Brightness——————————————————//

        case 1:
            lcd->setCursor(0, 0);
            if (digitalRead(leftButtonPin) == 1 && leds_brightness != 0 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                leds_brightness = leds_brightness - 2;
            }
            else if (digitalRead(rightButtonPin) == 1 && leds_brightness != 100 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                leds_brightness = leds_brightness + 2;
            }

            lcd->print("2. Bright: " + String(leds_brightness) + "%   ");
            break;
        //——————————————————Modes——————————————————//
        case 2:

            lcd->setCursor(0, 0);
            if (digitalRead(leftButtonPin) == 1 && led_selected_mode != 0 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                led_selected_mode = led_selected_mode - 1;
                //hc_write(hc_leds);
            }
            else if (digitalRead(rightButtonPin) == 1 && led_selected_mode != 4 && millis() - millsPress > pressDelay)
            {
                millsPress = millis();

                led_selected_mode = led_selected_mode + 1;
                //hc_write(hc_leds);
            }

            lcd->print("3. Mode: " + led_modes[led_selected_mode]);

            break;

            /*
            if (cursorPosition == 0)
            {
                cursorPosition = 3;
            }
            else
            {
                cursorPosition = (cursorPosition - 1) % 4;
            }*/

        //—————————————————— IDK ——————————————————//
        case 3:

            break;
        }
    }
    // CURSOR BLINK !
    cursorBlink();
}

void LcdMenu::drawMenu()
{
    int c = 0;
    unsigned long millsSel;

    // print option name based on cursor position
    if (!selected)
    {
        switch (cursorPosition)
        {
        case 0:
            lcd->clear();
            lcd->setCursor(1, 0);
            lcd->print("1. Speed");
            break;

        case 1:
            lcd->clear();
            lcd->setCursor(1, 0);
            lcd->print("2. Brightness");
            break;

        case 2:
            lcd->clear();
            lcd->setCursor(1, 0);
            lcd->print("3. Modes");
            break;

        case 3:
            lcd->clear();
            lcd->setCursor(1, 0);
            lcd->print("4. Back");
            break;
        }
    }
    lcd->setCursor(0, 1);

    for (int pos = 0; pos < 8; pos += 2)
    {
        if (cursorPosition * 2 == pos)
        { // draw inverted characted if currently selected, then
            drawChar(pos, 1, c + 4);
        }
        else
        {
            drawChar(pos, 1, c);
        }
        if (c < 4)
        {
            c = c + 1;
        }
    }

    lcd->setCursor(14, 1);
    lcd->print(cursorPosition);
}

void LcdMenu::drawChar(int x, int y, int c)
{
    // POSITION X , Y ------ custom character code 0-7
    lcd->setCursor(x, y);
    lcd->write(c);
}

void LcdMenu::cursorBlink()
{
    if (millis() - millsBlink > blinkInterval)
    {
        millsBlink = millis();

        if (cursorInverted)
        {
            drawChar(cursorPosition * 2, 1, cursorPosition + 4);
        }
        else
        {
            drawChar(cursorPosition * 2, 1, cursorPosition);
        }

        cursorInverted = !cursorInverted;
    }
}
