#include <Wire.h>
#include <SPI.h>

// MCP23017 registers (everything except direction defaults to 0)

const byte  IODIRA   = 0x00;   // IO direction  (0 = output, 1 = input (Default))
const byte  IODIRB   = 0x01;
const byte  IOPOLA   = 0x02;   // IO polarity   (0 = normal, 1 = inverse)
const byte  IOPOLB   = 0x03;
const byte  GPINTENA = 0x04;   // Interrupt on change (0 = disable, 1 = enable)
const byte  GPINTENB = 0x05;
const byte  DEFVALA  = 0x06;   // Default comparison for interrupt on change (interrupts on opposite)
const byte  DEFVALB  = 0x07;
const byte  INTCONA  = 0x08;   // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
const byte  INTCONB  = 0x09;
const byte  IOCON    = 0x0A;   // IO Configuration: bank/mirror/seqop/disslw/haen/odr/intpol/notimp
const byte  GPPUA    = 0x0C;   // Pull-up resistor (0 = disabled, 1 = enabled)
const byte  GPPUB    = 0x0D;
const byte  INFTFA   = 0x0E;   // Interrupt flag (read only) : (0 = no interrupt, 1 = pin caused interrupt)
const byte  INFTFB   = 0x0F;
const byte  INTCAPA  = 0x10;   // Interrupt capture (read only) : value of GPIO at time of last interrupt
const byte  INTCAPB  = 0x11;
const byte  GPIOA    = 0x12;   // Port value. Write to change, read to obtain value
const byte  GPIOB    = 0x13;
const byte  OLLATA   = 0x14;   // Output latch. Write to latch output.
const byte  OLLATB   = 0x15;


const byte  DEVICE_ADDRESS = 0x20;  // MCP23017 is on I2C port 0x20

const byte ssPin = 6;   // slave select pin, if non-zero use SPI
const byte expanderPort = 0x20;

// set register "reg" on expander to "data"
// for example, IO direction
void expanderWrite (const byte reg, const byte data )
{
  // start send
  digitalWrite (ssPin, LOW);
  SPI.transfer (expanderPort << 1);  // note this is write mode  
  
  SPI.transfer(reg); // send
  SPI.transfer(data); // send

  // end
  digitalWrite (ssPin, HIGH);
}



void setup ()
{

//Serial.begin();
//while(!Serial);
  
  digitalWrite (ssPin, HIGH);
  SPI.begin ();
  pinMode (ssPin, OUTPUT);
  

  // byte mode (not sequential)
  expanderWrite (IOCON, 0b00100000);
  
  // all pins as outputs
  expanderWrite (IODIRA, 0);
  expanderWrite (IODIRB, 0);
  
}  // end of setup

void loop ()
{
  expanderWrite (GPIOA, 0xAA);
  delay (100);  
  expanderWrite (GPIOA, 0);
  delay (100);  
}
