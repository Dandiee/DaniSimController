#include <Wire.h>
#define IODIRA   0x00   // IO direction  (0 = output, 1 = input (Default))
#define IODIRB   0x01
#define IOPOLA   0x02   // IO polarity   (0 = normal, 1 = inverse)
#define IOPOLB   0x03
#define GPINTENA 0x04   // Interrupt on change (0 = disable, 1 = enable)
#define GPINTENB 0x05
#define DEFVALA  0x06   // Default comparison for interrupt on change (interrupts on opposite)
#define DEFVALB  0x07
#define INTCONA  0x08   // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
#define INTCONB  0x09
#define IOCONA   0x0A   // IO Configuration: bank/mirror/seqop/disslw/haen/odr/intpol/notimp
#define IOCONB   0x0B  // same as 0x0A
#define GPPUA    0x0C   // Pull-up resistor (0 = disabled, 1 = enabled)
#define GPPUB    0x0D
#define INFTFA   0x0E   // Interrupt flag (read only) : (0 = no interrupt, 1 = pin caused interrupt)
#define INFTFB   0x0F
#define INTCAPA  0x10   // Interrupt capture (read only) : value of GPIO at time of last interrupt
#define INTCAPB  0x11
#define GPIOA    0x12   // Port value. Write to change, read to obtain value
#define GPIOB    0x13
#define OLLATA   0x14   // Output latch. Write to latch output.
#define OLLATB   0x15
#define port     0x20  // MCP23017 is on I2C port 0x20


#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

#define R_START      0b0000
#define R_CW_FINAL   0b0001
#define R_CW_BEGIN   0b0010
#define R_CW_NEXT    0b0011
#define R_CCW_BEGIN  0b0100
#define R_CCW_FINAL  0b0101
#define R_CCW_NEXT   0b0110

unsigned char state;
const unsigned char ttable[][4] = 
{
  // 00         01           10           11
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},           // R_START 
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},  // R_CW_FINAL
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},           // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},           // R_CW_NEXT
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},           // R_CCW_BEGIN
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW}, // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START}            // R_CCW_NEXT
};

volatile bool keyPressed = false;
unsigned int keyValue = 0;

void expanderWriteBoth (const byte reg, const byte data )
{
  Wire.beginTransmission (port);
  Wire.write (reg);
  Wire.write (data);  // port A
  Wire.write (data);  // port B
  Wire.endTransmission ();
} 

unsigned int expanderRead (const byte reg)
{
  Wire.beginTransmission (port);
  Wire.write (reg);
  Wire.endTransmission ();
  Wire.requestFrom (port, 1);
  return Wire.read();
}

void keypress ()
{
  keyPressed = true;   // set flag so main loop knows
}

void setup ()
{
  Wire.begin ();  
  Serial.begin (115200);
  while(!Serial);
 

  // setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity)
  uint8_t ioconfValue = expanderRead(IOCONA);
  bitWrite(ioconfValue, 6, true);
  bitWrite(ioconfValue, 2, true);
  bitWrite(ioconfValue, 1, LOW);
  expanderWriteBoth(IOCONA, ioconfValue);
  
  ioconfValue = expanderRead(IOCONB);
  bitWrite(ioconfValue, 6, true); // MIRRORING: OR both INTA and INTB pins.
  bitWrite(ioconfValue, 2, true); // OPENDRAIN: set the INT pin to value or open drain
  bitWrite(ioconfValue, 1, LOW);  // POLARITY: LOW or HIGH on interrupt
  expanderWriteBoth(IOCONB, ioconfValue);


  // pinMode(12, INPUT);
  updateRegisterBit(7, 1, IODIRA, IODIRB); // 1: INPUT, 0: OUTPUT

  // pullUp
  updateRegisterBit(7, 1, GPPUA, GPPUB); // 1: PULLUP

  // setupInterruptOnPin
  updateRegisterBit(7, 0, INTCONA, INTCONB); // 0 = change; 1 = compare against value
  updateRegisterBit(7, 1, DEFVALA, DEFVALB); // 0 = rising; 1 = faiilng
  updateRegisterBit(7, 1, GPINTENA, GPINTENB); // enable interrupt

  updateRegisterBit(6, 0, INTCONA, INTCONB); // 0 = change; 1 = compare against value
  updateRegisterBit(6, 1, DEFVALA, DEFVALB); // 0 = rising; 1 = faiilng
  updateRegisterBit(6, 1, GPINTENA, GPINTENB); // enable interrupt
  
  // read from interrupt capture ports to clear them
  
  Serial.println(expanderRead(INTCAPA));
  Serial.println(expanderRead(INTCAPB));
  //expanderRead(INTCAPB);
  
  pinMode(7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), keypress, FALLING);
  Serial.println ("Started");
}  

 unsigned char process(unsigned char pin1State, unsigned char pin2State) {
      unsigned char pinstate = (pin1State << 1) | pin2State;
      state = ttable[state & 0b00001111][pinstate]; 
      return (state & 0b00110000);
    }

void updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
  uint8_t regValue;
  uint8_t regAddr = getRegisterAddress(pin, portAaddr, portBaddr);
  uint8_t result = pin%8;
  regValue = expanderRead(regAddr);
  bitWrite(regValue, result, pValue);
  expanderWriteBoth(regAddr, regValue);
}

uint8_t getRegisterAddress(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr){
  return(pin<8) ? portAaddr : portBaddr;
}

void handleKeypress ()
{
  keyPressed = false;

  if (expanderRead(INFTFA))
  {
    keyValue &= 0xFF00;
    keyValue |= expanderRead (INTCAPA);    // read value at time of interrupt
    
    byte a = bitRead(keyValue, 6);
    byte b = bitRead(keyValue, 7);
    Serial.print(a); Serial.println(b);    
  }
}  

void loop ()
{
  if (keyPressed)
    handleKeypress ();
}
