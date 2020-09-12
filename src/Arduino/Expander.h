void onExpanderInterrupt();

#ifndef SRC_DANIMCP23S17_H_
#define SRC_DANIMCP23S17_H_
#include <SPI.h>

const byte IODIRA   = 0x00;   // IO direction  (0 = output, 1 = input (Default))
const byte IODIRB   = 0x01;
const byte IOPOLA   = 0x02;   // IO polarity   (0 = normal, 1 = inverse)
const byte IOPOLB   = 0x03;
const byte GPINTENA = 0x04;   // Interrupt on change (0 = disable, 1 = enable)
const byte GPINTENB = 0x05;
const byte DEFVALA  = 0x06;   // Default comparison for interrupt on change (interrupts on opposite)
const byte DEFVALB  = 0x07;
const byte INTCONA  = 0x08;   // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
const byte INTCONB  = 0x09;
const byte IOCON    = 0x0A;   // IO Configuration: bank/mirror/seqop/disslw/haen/odr/intpol/notimp
const byte GPPUA    = 0x0C;   // Pull-up resistor (0 = disabled, 1 = enabled)
const byte GPPUB    = 0x0D;
const byte INFTFA   = 0x0E;   // Interrupt flag (read only) : (0 = no interrupt, 1 = pin caused interrupt)
const byte INFTFB   = 0x0F;
const byte INTCAPA  = 0x10;   // Interrupt capture (read only) : value of GPIO at time of last interrupt
const byte INTCAPB  = 0x11;
const byte GPIOA    = 0x12;   // Port value. Write to change, read to obtain value
const byte GPIOB    = 0x13;
const byte OLLATA   = 0x14;   // Output latch. Write to latch output.
const byte OLLATB   = 0x15;

const byte EXP      = 0x20;
const byte INTPIN   = 0x07;

class Expander
{
  public:
    Expander(
      byte ssPin, 
      uint16_t pinDirections,
      uint16_t pullUps,
      uint16_t ioPolarities,
      uint16_t useInterrupts,
      uint16_t interruptControls,
      uint16_t defaultInterruptValue) : 
                ssPin(ssPin), 
                pinDirections(pinDirections), 
                pullUps(pullUps), 
                ioPolarities(ioPolarities), 
                useInterrupts(useInterrupts), 
                interruptControls(interruptControls), 
                defaultInterruptValue(defaultInterruptValue) { }

    void begin()
    {
      SPI.begin();
      pinMode(ssPin, OUTPUT);
      write(ssPin, HIGH);

      if (useInterrupts)
      {
        pinMode(INTPIN, INPUT_PULLUP);   
        attachInterrupt(digitalPinToInterrupt(INTPIN), ::onExpanderInterrupt, FALLING);
      }
    
      write(IOCON,    0b01101000);

      write(IODIRA,   pinDirections         & 0x00FF);  // Pin direction            : Output    Input
      write(GPPUA,    pullUps               & 0x00FF);  // Pull-up resistor         : Disabled  Enabled
      write(IOPOLA,   ioPolarities          & 0x00FF);  // IO polarity              : Normal    Inversed
      write(GPINTENA, useInterrupts         & 0x00FF);  // Interrupt                : Disabled  Enabled
      write(INTCONA,  interruptControls     & 0x00FF);  // Interrupt control        : OnChange  ChangeFrom:DEFVAL
      write(DEFVALA,  defaultInterruptValue & 0x00FF);  // Default intertupt value  : Low       High

      write(IODIRB,   pinDirections         & 0xFF00);  // Pin direction            : Output    Input
      write(GPPUB,    pullUps               & 0xFF00);  // Pull-up resistor         : Disabled  Enabled
      write(IOPOLB,   ioPolarities          & 0xFF00);  // IO polarity              : Normal    Inversed
      write(GPINTENB, useInterrupts         & 0xFF00);  // Interrupt                : Disabled  Enabled
      write(INTCONB,  interruptControls     & 0xFF00);  // Interrupt control        : OnChange  ChangeFrom:DEFVAL
      write(DEFVALB,  defaultInterruptValue & 0xFF00);  // Default intertupt value  : Low       Hig 

      if (useInterrupts)
      {
        read(INTCAPA);
        read(INTCAPB);
      }
    }

    uint16_t readAndReset()
    {
        // TODO: we should get both bytes mostly
        byte portA = read(GPIOA);
        byte portB = read(GPIOB);
        return ((portB << 8) | portA);
    }

  void letItGo()
  {
    digitalWrite(ssPin, HIGH);
  }
        
  private:

    byte ssPin = 0;
    bool useInterrupt = false;

    uint16_t pinDirections          = 0b0000000000000000;
    uint16_t pullUps                = 0b0000000000000000;
    uint16_t ioPolarities           = 0b0000000000000000;
    uint16_t useInterrupts          = 0b0000000000000000;
    uint16_t interruptControls      = 0b0000000000000000;
    uint16_t defaultInterruptValue  = 0b0000000000000000;
  
    void write(const byte reg, const byte data)
    {
      digitalWrite(ssPin, LOW);
      SPI.transfer(EXP << 1);  // note this is write mode
      SPI.transfer(reg);
      SPI.transfer(data);
      digitalWrite(ssPin, HIGH);
    }
    
    byte read(const byte reg)
    {
      digitalWrite(ssPin, LOW);
      SPI.transfer((EXP << 1) | 1);
      SPI.transfer(reg);
      byte data = SPI.transfer(0);
      digitalWrite(ssPin, HIGH);
      return data;
    }
};

#endif
