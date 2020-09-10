#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "Encoder.h"

Adafruit_MCP23017 mcp;
Adafruit_MCP23017* allMCPs[] = { &mcp };
constexpr int numMCPs = (int)(sizeof(allMCPs) / sizeof(*allMCPs));
byte arduinoIntPin = 7;
byte arduinoInterrupt=1;

volatile boolean awakenByInterrupt = false;
void RotaryEncoderChanged(bool clockwise, int id);
RotaryEncOverMCP rotaryEncoders[] = {
        RotaryEncOverMCP(&mcp, 7, 6, &RotaryEncoderChanged, 1)
};

constexpr int numEncoders = (int)(sizeof(rotaryEncoders) / sizeof(*rotaryEncoders));
void setup(){

    Serial.begin(115200);
    while(!Serial);
    pinMode(arduinoIntPin, INPUT);
    
    mcp.begin();
    mcp.setupInterrupts(true, false, LOW);
    
    for(int i=0; i < numEncoders; i++) {
        rotaryEncoders[i].initialize();
    }
    Serial.println("ready");
}

void RotaryEncoderChanged(bool clockwise, int id) {
    Serial.println("Encoder " + String(id) + ": "
            + (clockwise ? String("clockwise") : String("counter-clock-wise")));
}

void intCallBack() {
    awakenByInterrupt=true;
}

void handleInterrupt(){

// Get more information from the MCP from the INT
  //uint8_t pin = mcp.getLastInterruptPin();
  //uint8_t val = mcp.getLastInterruptPinValue();
uint8_t k = 0;
while(true)
{
  uint8_t pin = mcp.getLastInterruptPin();
  if(pin == 255) break;
  uint8_t value = mcp.getLastInterruptPinValue();
  Serial.println(String(pin) + "/" + String(value));
}
  
  //uint8_t a = mcp.digitalRead(6);
  //uint8_t b = mcp.digitalRead(7);

  //uint8_t c = rotaryEncoders[0].process(a, b);
  //Serial.println("Handle: " + String(a) + " " + String(b) + " C:" + String(c));  
  //pin = mcp.getLastInterruptPin();
  //val = mcp.getLastInterruptPinValue();
  //uint16_t res = mcp.readGPIOAB();
  //uint8_t a = res & 0b01000000;
  //uint8_t b = res & 0b10000000;
  //if(pin == 7)
  {
  
  }
//Serial.println(res);
//Serial.println(String(a > 0) + " " + String(b > 0));
  
  while( ! (mcp.digitalRead(7) && mcp.digitalRead(6) ));
  EIFR=0x01;
    awakenByInterrupt=false;
   /*for(int j = 0; j < numMCPs; j++) {
        uint16_t gpioAB = allMCPs[j]->readINTCAPAB();
        for (int i=0; i < numEncoders; i++) {
            //only feed this in the encoder if this
            //is coming from the correct MCP
            if(rotaryEncoders[i].getMCP() == allMCPs[j])
                rotaryEncoders[i].feedInput(gpioAB);
        }
    }

    */
}

void loop() {
  attachInterrupt(arduinoInterrupt, intCallBack, FALLING);
  while(!awakenByInterrupt);
  detachInterrupt(arduinoInterrupt);
  if(awakenByInterrupt) handleInterrupt();
}
