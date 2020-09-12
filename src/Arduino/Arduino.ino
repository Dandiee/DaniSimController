#include "Expander.h"
#include "Encoder.h"
#include "Display.h"

void onEncoderChanged(int8_t change, byte id, int value);
Encoder encoders[] = 
{ 
  Encoder(4, 5, 1, onEncoderChanged),
  Encoder(6, 7, 2, onEncoderChanged)
};
byte numberOfEncoders = sizeof(encoders)/sizeof(encoders[0]);
Expander expander = Expander();
Display display = Display(8, 9, 10);
uint16_t interruptQueue[64];
byte interruptsCount = 0;

bool isWriting = false;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  expander.begin();
  Serial.println("kickin");
}

void loop()
{ 
  //display.showNumberDec(encoders[0].value, false, 4, 0);

  if (interruptsCount)
  {
    
  }

  if (!isWriting)
  {
    if(interruptsCount)
    {
      if(interruptsCount > 1)
      {
        Serial.println(interruptsCount);
      }
      for (int i = 0; i < interruptsCount; i++)
      {
        uint16_t nextEvent = interruptQueue[i];
        
        for (int j = 0; j < numberOfEncoders; j++)
        {
          encoders[j].process(nextEvent);   
        }

        interruptsCount = 0;
      }
    }
  }
  
  /*uint16_t gpioState = expander.readGpioState();
  if (gpioState)
  {
    for (byte i = 0; i < numberOfEncoders; i++)
    {
      encoders[i].process(gpioState);
    }
  }*/
}

void onEncoderChanged(int8_t change, byte id, int value) 
{
  Serial.print(id);
  Serial.print(":");
  Serial.print(change);
  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");
}



void onExpanderInterrupt()
{
  isWriting = true;
  //detachInterrupt(digitalPinToInterrupt(INTPIN));
  interruptQueue[interruptsCount] = expander.readAndReset();
  interruptsCount++;
  //attachInterrupt(digitalPinToInterrupt(INTPIN), onExpanderInterrupt, FALLING);
  isWriting = false;
}
