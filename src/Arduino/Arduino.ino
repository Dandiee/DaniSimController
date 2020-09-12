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

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  expander.begin();
  Serial.println("kickin");
}

void loop()
{ 
  display.showNumberDec(encoders[0].value, false, 4, 0);
  
  uint16_t gpioState = expander.readGpioState();
  if (gpioState)
  {
    for (byte i = 0; i < numberOfEncoders; i++)
    {
      encoders[i].process(gpioState);
    }
  }
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
  expander.isInterrupted = true;
}
