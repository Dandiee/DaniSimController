#include <HID-Project.h>
#include "Expander.h"
#include "Encoder.h"
#include "Display.h"
#include "SimController.h"

void onEncoderChanged(int8_t change, byte id, int value);
void onSimStateChanged(byte key, int value);

SimController simController = SimController(onSimStateChanged);

Encoder encoders[] = 
{ 
  Encoder(0, 1, 1, onEncoderChanged),
  Encoder(2, 3, 2, onEncoderChanged),
  Encoder(4, 5, 3, onEncoderChanged),
  Encoder(6, 7, 4, onEncoderChanged)
};
byte numberOfEncoders = sizeof(encoders)/sizeof(encoders[0]);
Expander expander = Expander(
  6,                  // SS Pin
  0b1111111111111111, // Pin direction
  0b1111111111111111, // Pull-up
  0b1111111111111111, // IO Polarities
  0b1111111111111111, // Use interrupts
  0b0000000000000000, // Interrupt control
  0b0000000000000000  // Interrupt default value
);
volatile bool isExpanderInterrupted = false;

int potentiometers[] = {0};

Expander expander2 = Expander(
  5,                  // SS Pin
  0b0000000000000000, // Pin direction
  0b0000000000000000, // Pull-up
  0b0000000000000000, // IO Polarities
  0b0000000000000000, // Use interrupts
  0b0000000000000000, // Interrupt control
  0b0000000000000000  // Interrupt default value
);

const byte commonDisplayClkPin = 2;
Display displays[] = 
{
  Display(commonDisplayClkPin, 3),
  Display(commonDisplayClkPin, 4),
  Display(commonDisplayClkPin, 8),
  Display(commonDisplayClkPin, 9),
  Display(commonDisplayClkPin, 10),
  Display(commonDisplayClkPin, 11),
  Display(commonDisplayClkPin, 12),
  Display(commonDisplayClkPin, 13),
};
byte numberOfDisplays = sizeof(displays)/sizeof(displays[0]);
const byte panicButtonPin = 4;

uint16_t expander2GpioValue = 0;
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

  expander2.begin();
  Serial.println("kickin 2 - the expander strikes back");
  
  //pinMode(panicButtonPin, INPUT_PULLUP);

  //Gamepad.begin();
}

void checkInterrupts()
{
  if (isExpanderInterrupted)
  {
      detachInterrupt(digitalPinToInterrupt(INTPIN));
    
      uint16_t nextInterrupt = expander.readAndReset();
      for (int j = 0; j < numberOfEncoders; j++)
      {
        encoders[j].process(nextInterrupt);   
      }
    
      isExpanderInterrupted = false;
      attachInterrupt(digitalPinToInterrupt(INTPIN), onExpanderInterrupt, FALLING);
  }
}

void loop()
{ 
  for (byte i = 0; i < 4; i++)
  {
    displays[i].showNumberDec(encoders[i].value, false, 4, 0);
    checkInterrupts();
    displays[i + 4].showNumberDec(encoders[i].value + 1, false, 4, 0);
    checkInterrupts();
  }

  

  checkInterrupts();

  /*checkInterrupts();

  if (digitalRead(panicButtonPin) == LOW)
  {
    Serial.println("Debug pressed, please wait...");
    uint16_t gpioState = expander.readAndReset();
    writeBinary(gpioState);
    delay(500);
    Serial.println("Okay, good to go");
  }*/

  /*uint16_t exp2Io = expander2.readAndReset();
  if (expander2GpioValue != exp2Io)
  {
    writeBinary(exp2Io);  
    expander2GpioValue = exp2Io;
  }*/

  //simController.readSerial();

  //sendGamepadReport();
}

void sendGamepadReport()
{
   //Gamepad.xAxis( (analogRead(A5) * 64) - 32768);
   Gamepad.xAxis( 100);
   Gamepad.write();
}

void writeBinary(uint16_t doubleByte){
  for (byte i = 0; i < 16; i++)
  {
    Serial.print(bitRead(doubleByte, i));
  }
  //Serial.println();
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

void onSimStateChanged(byte key, int value)
{
  displays[0].showNumberDec(value, false, 4, 0);
}

void onExpanderInterrupt()
{
  isExpanderInterrupted = true;
}
