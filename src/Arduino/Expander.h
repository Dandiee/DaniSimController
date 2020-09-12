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

const byte SSPIN    = 0x06;   
const byte EXP      = 0x20;
const byte INTPIN   = 0x07;

class Expander
{
  public:
    Expander() { }

    void begin()
    {
      SPI.begin();
      pinMode(SSPIN, OUTPUT);
      write(SSPIN, HIGH);

      pinMode(INTPIN, INPUT_PULLUP);   
      attachInterrupt(digitalPinToInterrupt(INTPIN), ::onExpanderInterrupt, FALLING);
    
      write(IOCON,    0b01101000);
                                      //                          : 0         1
      write(IODIRA,   0b11111111); // Pin direction            : Output    Input
      write(GPPUA,    0b11111111); // Pull-up resistor         : Disabled  Enabled
      write(IOPOLA,   0b11111111); // IO polarity              : Normal    Inversed
      write(GPINTENA, 0b11111111); // Interrupt                : Disabled  Enabled
      write(INTCONA,  0b00000000); // Interrupt control        : OnChange  ChangeFrom:DEFVAL
      write(DEFVALA,  0b00000000); // Default intertupt value  : Low       High
      
      write(IODIRB,   0b11111111); 
      write(GPPUB,    0b00000000); 
      write(GPINTENB, 0b00000000);
      write(IOPOLB,   0b11111111);
      write(INTCONB,  0b00000000);
      
      read(INTCAPA);
      read(INTCAPB);
    }

    uint16_t readGpioState()
    {
      if (isInterrupted)
      {
        detachInterrupt(digitalPinToInterrupt(INTPIN));
        byte portA = read(GPIOA);
        byte portB = read(GPIOB);

        uint16_t result = ((portB << 8) | portA);
           
        attachInterrupt(digitalPinToInterrupt(INTPIN), ::onExpanderInterrupt, FALLING);
        isInterrupted = false;

        return result;
      }

      return 0;
    }

    uint16_t readAndReset()
    {
        byte portA = read(GPIOA);
        byte portB = read(GPIOB);

        return ((portB << 8) | portA);
    }
    

    bool isInterrupted = false;
    
  private:
    void write(const byte reg, const byte data)
    {
      digitalWrite(SSPIN, LOW);
      SPI.transfer(EXP << 1);  // note this is write mode
      SPI.transfer(reg);
      SPI.transfer(data);
      digitalWrite(SSPIN, HIGH);
    }
    
    byte read(const byte reg)
    {
      digitalWrite(SSPIN, LOW);
      SPI.transfer((EXP << 1) | 1);
      SPI.transfer(reg);
      byte data = SPI.transfer(0);
      digitalWrite(SSPIN, HIGH);
      return data;
    }
};

#endif
