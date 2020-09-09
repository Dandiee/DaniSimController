#ifndef SRC_ROTARYENCOVERMCP_H_
#define SRC_ROTARYENCOVERMCP_H_


#include "Adafruit_MCP23017.h"
#include "Rotary.h"

typedef void (*rotaryActionFunc)(bool clockwise, int id);
class RotaryEncOverMCP {
  public:
    RotaryEncOverMCP(Adafruit_MCP23017* mcp, byte pinA, byte pinB, rotaryActionFunc actionFunc = nullptr, int id = 0)
    : rot(), mcp(mcp),
      pinA(pinA), pinB(pinB),
      actionFunc(actionFunc), id(id) {
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
        uint8_t event = rot.process(pinValA, pinValB);
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
    Rotary rot;                         /* the rotary object which will be created*/
    Adafruit_MCP23017* mcp = nullptr;   /* pointer the I2C GPIO expander it's connected to */
    uint8_t pinA = 0;
    uint8_t pinB = 0;           /* the pin numbers for output A and output B */
    rotaryActionFunc actionFunc = nullptr;  /* function pointer, will be called when there is an action happening */
    int id = 0;                             /* optional ID for identification */
};


#endif /* SRC_ROTARYENCOVERMCP_H_ */
