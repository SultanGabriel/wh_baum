#ifndef STATE_H
#define STATE_H
#include <Arduino.h>

class State
{

private:
    int Led_Brightness;
    int Led_Speed;

    int Mode;

    int Lcd_cursorPos;
    bool Lcd_update;

public:
    State();

    void init();

    //

    // Setters

    void setLedBrightness(int newBrightness);
    void setMode(int newMode);
    void setSpeed(int newSpeed);
    //

    // Getters

    int getLedBrightness();
    int getMode();
    int getSpeed();
};
#endif