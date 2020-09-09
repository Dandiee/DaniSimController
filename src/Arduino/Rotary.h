#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

class Rotary
{
  public:
    Rotary();
    unsigned char process(unsigned char pin1State, unsigned char pin2State);

  private:
    unsigned char state;
};

#endif
