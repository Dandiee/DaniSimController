#ifndef SRC_ROTARYENCOVERMCP_H_
#define SRC_ROTARYENCOVERMCP_H_


#include "Adafruit_MCP23017.h"
#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

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

typedef void (*rotaryActionFunc)(bool clockwise, int id);
class RotaryEncOverMCP {
  public:
    RotaryEncOverMCP(Adafruit_MCP23017* mcp, byte pinA, byte pinB, rotaryActionFunc actionFunc = nullptr, int id = 0)
    : mcp(mcp),
      pinA(pinA), 
      pinB(pinB),
      actionFunc(actionFunc), 
      id(id) {
        state = R_START;
    }

    unsigned char process(unsigned char pin1State, unsigned char pin2State) {
      unsigned char pinstate = (pin1State << 1) | pin2State;
      state = ttable[state & 0b00001111][pinstate]; 
      return (state & 0b00110000);
    }

    /* Initialize object in the MCP */
    void init() {
        if(mcp != nullptr) {
            mcp->pinMode(pinA, INPUT);
            mcp->pullUp(pinA, 1); //disable pullup on this pin
            mcp->setupInterruptPin(pinA,CHANGE);
            
            mcp->pinMode(pinB, INPUT);
            mcp->pullUp(pinB, 1); //disable pullup on this pin
            mcp->setupInterruptPin(pinB,CHANGE);
        }
    }

    /* On an interrupt, can be called with the value of the GPIOAB register (or INTCAP) */
    void feedInput(uint16_t gpioAB) {
        uint8_t pinValA = bitRead(gpioAB, pinA);
        uint8_t pinValB = bitRead(gpioAB, pinB);
        uint8_t event = process(pinValA, pinValB);
        if(event == DIR_CW || event == DIR_CCW) {
            //clock wise or counter-clock wise
            bool clockwise = event == DIR_CW;
            //Call into action function if registered
            if(actionFunc) {
                actionFunc(clockwise, id);
            }
        }
    }

    Adafruit_MCP23017* getMCP() {
        return mcp;
    }

private:
    Adafruit_MCP23017* mcp = nullptr;
    uint8_t pinA = 0;
    uint8_t pinB = 0;           /* the pin numbers for output A and output B */
    rotaryActionFunc actionFunc = nullptr;  /* function pointer, will be called when there is an action happening */
    int id = 0;                             /* optional ID for identification */
    unsigned char state;
};


#endif /* SRC_ROTARYENCOVERMCP_H_ */
