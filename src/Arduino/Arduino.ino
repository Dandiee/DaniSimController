#include <Arduino.h>
#include "Adafruit_MCP23017.h"
#include "Rotary.h"
#include "RotaryEncOverMCP.h"

/* Our I2C MCP23017 GPIO expanders */
Adafruit_MCP23017 mcp;

//Array of pointers of all MCPs if there is more than one
Adafruit_MCP23017* allMCPs[] = { &mcp };
constexpr int numMCPs = (int)(sizeof(allMCPs) / sizeof(*allMCPs));

/* the INT pin of the MCP can only be connected to
 * an interrupt capable pin on the Arduino, either
 * D3 or D2.
 * */
byte arduinoIntPin = 7;

/* variable to indicate that an interrupt has occured */
volatile boolean awakenByInterrupt = false;

/* function prototypes */
void intCallBack();
void cleanInterrupts();
void handleInterrupt();
void RotaryEncoderChanged(bool clockwise, int id);

/* Array of all rotary encoders and their pins */
RotaryEncOverMCP rotaryEncoders[] = {
        // outputA,B on GPA7,GPA6, register with callback and ID=1
        RotaryEncOverMCP(&mcp, 7, 6, &RotaryEncoderChanged, 1)
};
constexpr int numEncoders = (int)(sizeof(rotaryEncoders) / sizeof(*rotaryEncoders));

void RotaryEncoderChanged(bool clockwise, int id) {
    Serial.println("Encoder " + String(id) + ": "
            + (clockwise ? String("clockwise") : String("counter-clock-wise")));
}
RotaryEncOverMCP referenceRotary(&mcp, 7, 6, &RotaryEncoderChanged, 1);
void setup(){

    Serial.begin(115200);
    while(!Serial); Serial.println("asd");
    pinMode(arduinoIntPin,INPUT);Serial.println("asd");
    mcp.begin();      Serial.println("asd");
    mcp.readINTCAPAB(); Serial.println("asd");
    mcp.setupInterrupts(true,false,LOW); Serial.println("asd");
    
    for(int i=0; i < numEncoders; i++) {
        rotaryEncoders[i].init();
    }
    Serial.println("asd");
    attachInterrupt(digitalPinToInterrupt(arduinoIntPin), intCallBack, FALLING);
    Serial.println("asd");
    /*pinMode(2,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), myPrettyCallback, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), myPrettyCallback, CHANGE);
*/
    Serial.println("ready");
}

void myPrettyCallback()
{
  char c = referenceRotary.process(digitalRead(2), digitalRead(3));
  if(c){
    Serial.println(c == DIR_CCW ? "CCW" : "CW");
  }
}

// The int handler will just signal that the int has happened
// we will do the work from the main loop.
void intCallBack() {
    awakenByInterrupt=true;
}

void checkInterrupt() {
    if(awakenByInterrupt) {
        // disable interrupts while handling them.
        detachInterrupt(digitalPinToInterrupt(arduinoIntPin));
        handleInterrupt();
        attachInterrupt(digitalPinToInterrupt(arduinoIntPin), intCallBack, FALLING);
    }
}

void handleInterrupt(){
   for(int j = 0; j < numMCPs; j++) {
        uint16_t gpioAB = allMCPs[j]->readINTCAPAB();
        for (int i=0; i < numEncoders; i++) {
            //only feed this in the encoder if this
            //is coming from the correct MCP
            if(rotaryEncoders[i].getMCP() == allMCPs[j])
                rotaryEncoders[i].feedInput(gpioAB);
        }
    }

    cleanInterrupts();
}

void cleanInterrupts(){
    EIFR=0x01;
    awakenByInterrupt=false;
}

void loop() {
  checkInterrupt();
}
