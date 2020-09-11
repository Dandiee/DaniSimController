#include <SPI.h>

// MCP23S17 registers

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

const byte ssPin = 6;   
const byte ports =  0x20;
const byte intPins = 7;

bool interrupted = false;
static byte counters[8] = {0,0,0,0,0,0,0,0 };
byte portALast = 0b00000000;

//ISR functions
void handleInterrupt() { interrupted = true; }


//Write to expander
void expanderWrite (const byte reg, const byte data, const byte port)
  {
     digitalWrite (ssPin, LOW);
     SPI.transfer (port << 1);  // note this is write mode
     SPI.transfer (reg);
     SPI.transfer (data);
     digitalWrite (ssPin, HIGH);
}

//Read from expander
byte expanderRead(const byte reg)
{
     byte data = 0;
     digitalWrite (ssPin, LOW);
     SPI.transfer ((ports << 1) | 1);  // note this is read mode
     SPI.transfer (reg);
     data = SPI.transfer (0);
     digitalWrite (ssPin, HIGH);

     return data;
}

void setup()
{

  Serial.begin(9600);
  while(!Serial);

  SPI.begin();

  pinMode (ssPin, OUTPUT);
  digitalWrite (ssPin, HIGH);

  pinMode(intPins, INPUT_PULLUP);   
  attachInterrupt(digitalPinToInterrupt(intPins), handleInterrupt, FALLING);
 
  
      //Config MCP
      expanderWrite (IOCON, 0b01101000, ports); //Mirror interrupts, disable sequential mode, enable hardware adressing
   
      //Set PORT A registers
      expanderWrite (IODIRA, 0b11111111, ports); //1 is input
      expanderWrite (GPPUA, 0b00000000, ports); 
      expanderWrite (IOPOLA, 0b11111111, ports);
      expanderWrite (INTCONA, 0b00000000, ports);
      expanderWrite (GPINTENA, 0b11111111, ports);
   
      //Set PORT B registers
      expanderWrite (IODIRB, 0b11111111, ports); //1 is input
      expanderWrite (GPPUB, 0b00000000, ports); 
      expanderWrite (IOPOLB, 0b11111111, ports);
      expanderWrite (INTCONB, 0b00000000, ports);
      expanderWrite (GPINTENB, 0b00000000, ports);

  expanderRead(INTCAPA);
  expanderRead(INTCAPB);

      Serial.println("started");
  
}

void loop()
{
  
      if(interrupted)
      {
       
        interrupted = false;
        byte portA = expanderRead(GPIOA);
        byte portB = expanderRead(GPIOB);
   
        //Check which pin of the encoder caused interrupt and and check the value of pin A
       
        byte mask = 0;
        byte i = 0;
        byte aLast;
        byte aCurrent;
               
        for(i = 0; i < 8; i++)
        {
   
            mask = 1 << i;
            aLast = portALast & mask;
            aCurrent = portA & mask;
   
            if(aLast ^ aCurrent)
            {
              break;
            }
         
        }

        //Then determine the direction of rotation if its the raising edge of the square wave
        if(aCurrent)
        {
          if(!aLast)
          {
            portALast = portA;
            byte bState = mask & portB;
           
            if(bState) //Clockwise
            {
             
             counters[i]++;
            }else{ //Counter-Clockwise
              counters[i]--;;
            } 

            Serial.print(i);
            Serial.print(") : ");
            Serial.println(counters[i]);           
           
          }
        }else{
          if(aLast)
          {
            portALast = portA;
          }
        }
      }
  } 
