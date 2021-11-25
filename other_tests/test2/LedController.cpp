#include "LedController.h"
#include <Arduino.h>
#include <ShiftRegisterPWM.h>

LedController::LedController(ShiftRegisterPWM &_shiftRegister)
{
    // Use 'this->' to make the difference between the
    // 'pin' attribute of the class and the
    // local variable 'pin' created from the parameter.
    // this->pin = pin;

    this->shiftReg = &_shiftRegister;
    //init();
}
void LedController::init()
{
    pinMode(PIN_SHCP, OUTPUT);
    pinMode(PIN_STCP, OUTPUT);
    pinMode(PIN_DS, OUTPUT);

    for (int i = 0; i < sizeof(PINS_PWM); i++)
    {
        pinMode(PINS_PWM, OUTPUT);
    }

}
void LedController::on()
{
}

void LedController::off()
{
}

void LedController::update()
{
    switch (modeIndex)
    {
    case 0:
        for (uint8_t i = 0; i < 8; i++)
        {
            uint8_t val = (uint8_t)(((float)
                                         sin(millis() / fadeDelay + i / 8.0 * 2.0 * PI) +
                                     1) *
                                    128);
            shiftReg->set(i, val);
        }
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        for (uint8_t i = 0; i < 8; i++)
        {
            shiftReg->set(i, 0);
        }
    }
}
