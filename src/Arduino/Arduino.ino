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
Display display = Display(8, 9, 5);

uint16_t interruptQueueA[512];
uint16_t interruptQueueB[512];

volatile bool isQueueAUnderWrite = true;

volatile byte interruptsCountA = 0;
volatile byte interruptsCountB = 0;

bool isWriting = false;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  expander.begin();
  Serial.println("kickin");

  pinMode(13, INPUT_PULLUP);
}

bool consumerInterruptQueue(uint16_t interruptQueue[], byte interruptsCount)
{
  if (interruptsCount)
  {     
    for (byte i = 0; i < interruptsCount; i++)
    {
      uint16_t nextInterrupt = interruptQueue[i];
      for (int j = 0; j < numberOfEncoders; j++)
      {
        encoders[j].process(nextInterrupt);   
      }
    }
  }  
}

void loop()
{ 
  display.showNumberDec(encoders[0].value, false, 4, 0);

  if (isQueueAUnderWrite)
  {
    consumerInterruptQueue(interruptQueueB, interruptsCountB);   
    interruptsCountB = 0;
    isQueueAUnderWrite = false;
  }
  else
  {
    consumerInterruptQueue(interruptQueueA, interruptsCountA);
    interruptsCountA = 0;
    isQueueAUnderWrite = true;
  }

  if (digitalRead(13) == LOW)
  {
    Serial.println("Debug pressed, please wait...");
    uint16_t gpioState = expander.readAndReset();
    Serial.println(gpioState);
    delay(500);
    Serial.println("Okay, good to go");
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
  if (isQueueAUnderWrite)
  {
    interruptQueueA[interruptsCountA++] = expander.readAndReset();
  }
  else
  {
    interruptQueueB[interruptsCountB++] = expander.readAndReset();
  }
}
