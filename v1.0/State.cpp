#include "State.h"
/**
     * 
     */
State::State()
{
}

/*

    New Plan for this
        use it as a variable object.. with everything needed
        getter / setters to 

*/
State::State(fpointer _Led_handlerFunction,
             fpointer _Pwm_handlerFunction,
             fpointer _Hc_handlerFunction)
{

    // Use 'this->'

    int Led_Brightness = 100;
    int Led_Speed = 100;

    int Mode = 0; // 0 - Fade ; 1 - x ;

    int Lcd_cursorPos = 0;
    bool Lcd_update = true; // Default: true -- so it refreshes after it starts

    // serios stuff
    // ADMIRE IT THEN GET RID OF IT BECAUSE WE'RE GOING SIMPLER FMM VIATA WHY

    Led_handlerFunction = _Led_handlerFunction;
    Pwm_handlerFunction = _Pwm_handlerFunction;
    Hc_handlerFunction = _Hc_handlerFunction;
}

void State::init()
{

    // void State::on()
    // {
    //     digitalWrite(pin, HIGH);
    // }
}

void State::updateHandlers()
{
    Hc_handlerFunction();
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
