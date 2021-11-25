#include <ShiftRegisterPWM.h>

ShiftRegisterPWM sr(1, 16);

void setup()
{
  pinMode(2, OUTPUT); // sr data pin  // DS
  pinMode(3, OUTPUT); // sr clock pin  // STCP
  pinMode(4, OUTPUT); // sr latch pin  // SHCP

  // use timer1 for frequent update
  sr.interrupt(ShiftRegisterPWM::UpdateFrequency::SuperFast);
}
int delay = 250;

void loop()
{
  for (uint8_t i = 0; i < 8; i++)
  {
    uint8_t val = (uint8_t)(((float)
                             sin(millis() / delay + i / 8.0 * 2.0 * PI) + 1) * 128);
    sr.set(i, val);
  }
}
