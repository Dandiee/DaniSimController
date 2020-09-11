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

bool isInterrupted = false;
void onInterrupt() { isInterrupted = true; }
void expWrite(const byte reg, const byte data);
byte expRead(const byte reg);

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();

  pinMode(SSPIN, OUTPUT);
  digitalWrite(SSPIN, HIGH);

  pinMode(INTPIN, INPUT_PULLUP);   
  attachInterrupt(digitalPinToInterrupt(INTPIN), onInterrupt, FALLING);

  expWrite(IOCON,    0b01101000);
                                  //                          : 0         1
  expWrite(IODIRA,   0b11111111); // Pin direction            : Output    Input
  expWrite(GPPUA,    0b00000000); // Pull-up resistor         : Disabled  Enabled
  expWrite(IOPOLA,   0b11111111); // IO polarity              : Normal    Inversed
  expWrite(INTCONA,  0b00000000); // Interrupt control        : OnChange  ChangeFrom:DEFVAL
  expWrite(DEFVALA,  0b00000000); // Default intertupt value  : Low       High
  expWrite(GPINTENA, 0b11111111); // Interrupt                : Disabled  Enabled
   
  expWrite(IODIRB,   0b11111111); 
  expWrite(GPPUB,    0b00000000); 
  expWrite(IOPOLB,   0b11111111);
  expWrite(INTCONB,  0b00000000);
  expWrite(GPINTENB, 0b00000000);

  expRead(INTCAPA);
  expRead(INTCAPB);

  Serial.println("kickin");
}

void loop()
{ 
  if (isInterrupted)
  {
    detachInterrupt(digitalPinToInterrupt(INTPIN));
    byte portA = expRead(GPIOA);
    byte portB = expRead(GPIOB);

    writeBinary(portA);
    Serial.print("/");
    writeBinary(portB);
    Serial.println();

    attachInterrupt(digitalPinToInterrupt(INTPIN), onInterrupt, FALLING);
    isInterrupted = false;
  }
}

void writeBinary(byte b)
{
  for (byte i = 0; i < 8; i++)  
  {
    Serial.print(bitRead(b, i));
  }
}

void expWrite(const byte reg, const byte data)
{
  digitalWrite(SSPIN, LOW);
  SPI.transfer(EXP << 1);  // note this is write mode
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(SSPIN, HIGH);
}

byte expRead(const byte reg)
{
  digitalWrite(SSPIN, LOW);
  SPI.transfer((EXP << 1) | 1);
  SPI.transfer(reg);
  byte data = SPI.transfer(0);
  digitalWrite(SSPIN, HIGH);
  return data;
}
