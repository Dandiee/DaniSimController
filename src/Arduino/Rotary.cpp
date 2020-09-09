#include "Arduino.h"
#include "Rotary.h"

#define R_START      0b0000
#define R_CW_FINAL   0b0001
#define R_CW_BEGIN   0b0010
#define R_CW_NEXT    0b0011
#define R_CCW_BEGIN  0b0100
#define R_CCW_FINAL  0b0101
#define R_CCW_NEXT   0b0110

const unsigned char ttable[][4] = 
{
  // 00         01           10           11
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},           // R_START 
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},  // R_CW_FINAL
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},           // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},           // R_CW_NEXT
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},           // R_CCW_BEGIN
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW}, // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START}            // R_CCW_NEXT
};

Rotary::Rotary()
{
  state = R_START;
}

unsigned char Rotary::process(unsigned char pin1State, unsigned char pin2State) {
	unsigned char pinstate = (pin1State << 1) | pin2State;
  state = ttable[state & 0b00001111][pinstate]; 
  return (state & 0b00110000);
}
