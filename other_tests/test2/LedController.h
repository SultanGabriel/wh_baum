#ifndef LedController_H
#define LedController_H
#include <Arduino.h>
#include <ShiftRegisterPWM.h>

class LedController
{

  private:
    ShiftRegisterPWM *shiftReg;
    // 74HC595 Pins
    int PIN_SHCP = 4;
    int PIN_STCP = 3;
    int PIN_DS = 2;

    int PINS_PWM[5] = {5, 6, 9, 10, 11};

    int modes[4] = {"Fade", "Blink", "Random", "Off"};
    int modeIndex = 0;

    int fadeDelay = 250;

  public:
    LedController(ShiftRegisterPWM &_shiftReg);


    void init();
    void on();
    void off();
    void changeMode(int mode);
    void update();
};

#endif
