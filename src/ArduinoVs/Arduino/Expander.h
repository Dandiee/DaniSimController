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
      write(ssPin, HIGH);

      if (useInterrupts)
      {
        pinMode(INTPIN, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(INTPIN), ::onExpanderInterrupt, FALLING);
      }

      byte BANK = 0;    // Controls how the registers are addressed
                        //        0 = The registers are in the same bank (addresses are sequential).
                        //        1 = The registers associated with each port are separated into different banks.

      byte MIRROR = 1;  // INT Pins Mirror bit
                        //        0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with
                        //        1 = The INT pins are internally connected

      byte SEQOP = 1;   // Sequential Operation mode bit
                        //        0 = Sequential operation enabled, address pointer increments.
                        //        1 = Sequential operation disabled, address pointer does not increment.

      byte DISSLW = 0;  // Slew Rate control bit for SDA output
                        //        0 = Slew rate enabled
                        //        1 = Slew rate disabled

      byte HAEN = 1;    //  Hardware Address Enable bit (MCP23S17 only)
                        //        0 = Disables the MCP23S17 address pins
                        //        1 = Enables the MCP23S17 address pins

      byte ODR = 0;     // Configures the INT pin as an open-drain output
                        //        0 = Active driver output (INTPOL bit sets the polarity.)
                        //        1 = Open-drain output (overrides the INTPOL bit.)

      byte INPOT = 0;   // This bit sets the polarity of the INT output pin
                        //        0 = Active-low
                        //        1 = Active-high

      uint16_t icon = 0;
      bitWrite(icon, 7, BANK);
      bitWrite(icon, 6, MIRROR);
      bitWrite(icon, 5, SEQOP);
      bitWrite(icon, 4, DISSLW);
      bitWrite(icon, 3, HAEN);
      bitWrite(icon, 2, ODR);
      bitWrite(icon, 1, INPOT);
      write(IOCON, icon);

     
      write(IODIRA,   pinDirections         & 0x00FF);  // Pin direction            : Output    Input
      write(GPPUA,    pullUps               & 0x00FF);  // Pull-up resistor         : Disabled  Enabled
      write(IOPOLA,   ioPolarities          & 0x00FF);  // IO polarity              : Normal    Inversed
      write(GPINTENA, useInterrupts         & 0x00FF);  // Interrupt                : Disabled  Enabled
      write(INTCONA,  interruptControls     & 0x00FF);  // Interrupt control        : OnChange  ChangeFrom:DEFVAL
      write(DEFVALA,  defaultInterruptValue & 0x00FF);  // Default intertupt value  : Low       High

      write(IODIRB,   (pinDirections         & 0xFF00) >> 8);  // Pin direction            : Output    Input
      write(GPPUB,    (pullUps               & 0xFF00) >> 8);  // Pull-up resistor         : Disabled  Enabled
      write(IOPOLB,   (ioPolarities          & 0xFF00) >> 8);  // IO polarity              : Normal    Inversed
      write(GPINTENB, (useInterrupts         & 0xFF00) >> 8);  // Interrupt                : Disabled  Enabled
      write(INTCONB,  (interruptControls     & 0xFF00) >> 8);  // Interrupt control        : OnChange  ChangeFrom:DEFVAL
      write(DEFVALB,  (defaultInterruptValue & 0xFF00) >> 8);  // Default intertupt value  : Low       Hig

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

      lastKnownGpioState = ((portB << 8) | portA);

      return lastKnownGpioState;
    }

    void writeGpio(uint16_t value)
    {
      wordWrite(GPIOA, value);
      lastKnownGpioState = value;
    }

    void writePin(byte pin, bool value)
    {
      uint16_t word = value;
      bitWrite(word, pin, value);
      writeGpio(word);
    }

    void letItGo()
    {
      digitalWrite(ssPin, HIGH);
    }

    byte ssPin = 0;

  private:

    uint16_t lastKnownGpioState = 0;
    
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

    void wordWrite(uint8_t reg, unsigned int word)
    {
      digitalWrite(ssPin, LOW);
      SPI.transfer(EXP << 1);
      SPI.transfer(reg);
      SPI.transfer((uint8_t) (word));
      SPI.transfer((uint8_t) (word >> 8));
      digitalWrite(ssPin, HIGH);
    }
};

#endif
