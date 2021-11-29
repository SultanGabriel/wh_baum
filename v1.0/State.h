#ifndef STATE_H
#define STATE_H
#include <Arduino.h>

typedef void (*fpointer)(); // pointer to function with no args and void return

class State
{

private:
    int Led_Brightness;
    int Led_Speed;

    int Mode;

    int Lcd_cursorPos;
    bool Lcd_update;

    fpointer Led_handlerFunction;
    fpointer Pwm_handlerFunction;
    fpointer Hc_handlerFunction;

public:
    State();
    State(fpointer _Led_handlerFunction,
          fpointer _Pwm_handlerFunction,
          fpointer _Hc_handlerFunction);

    void init();
    void updateHandlers();

    //

    // Setters

    void setLedBrightness(int newBrightness);
    int getMode();

    //

    // Getters

    int getLedBrightness();
    void setMode(int newMode);
};
#endif