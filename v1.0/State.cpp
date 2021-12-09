#include "State.h"
/**
 *
 */
State::State()
{
    // Use 'this->'

    int Led_Brightness = 100;
    int Led_Speed = 50;
    int Led_Rgb[3] = {0, 100, 100};
    bool Led_RgbSync = true;

    int Mode = 0; // 0 - Fade ; 1 - x ;

    int Lcd_cursorPos = 0;
    bool Lcd_update = true; // Default: true -- so it refreshes after it starts
}

void State::init()
{

    // void State::on()
    // {
    //     digitalWrite(pin, HIGH);
    // }
}

//

//

// Setters

void State::setLedBrightness(int newBrightness)
{

    Led_Brightness = newBrightness % 101; // % 101 so its bound to 0 - 100 :)
    Lcd_update = true;
}

void State::setMode(int newMode)
{
    Mode = newMode;
}

void State::setSpeed(int newSpeed)
{
    Led_Speed = newSpeed;
}
//

// Getters

int State::getMode()
{
    return Mode;
}

int State::getLedBrightness()
{
    return Led_Brightness;
}

int State::getSpeed()
{
    return Led_Speed;
}
