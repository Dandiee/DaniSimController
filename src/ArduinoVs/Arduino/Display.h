#ifndef SRC_DANIDISPLAY_H_
#define SRC_DANIDISPLAY_H_

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const byte digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

static const byte minusSegments = 0b01000000;

const byte clearData[] = { 0, 0, 0, 0 };
const uint32_t delayInMicrosec = 10;

class Display
{
  public:
    Display(byte clkPin, byte dioPin)
      : clkPin(clkPin), dioPin(dioPin), displayedValue(displayedValue)
    {
      brightness = 8;
      
      pinMode(clkPin, INPUT);
      pinMode(dioPin, INPUT);
      
      digitalWrite(clkPin, LOW);
      digitalWrite(dioPin, LOW);

      clear();
    }

    void clear()
    {
      setSegments(clearData, 4, 0);
    }

    void showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
    {
      if (num != displayedValue)
      {
        showNumberDecEx(num, 0, leading_zero, length, pos);
        displayedValue = num;
      }
    }
    
    void showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                        uint8_t length, uint8_t pos)
    {
      showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
    }
    
    void showNumberHexEx(uint16_t num, uint8_t dots, bool leading_zero,
                                        uint8_t length, uint8_t pos)
    {
      showNumberBaseEx(16, num, dots, leading_zero, length, pos);
    }
    
    void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
                                        uint8_t length, uint8_t pos)
    {     
        bool negative = false;
      if (base < 0) {
          base = -base;
        negative = true;
      }
    
    
        uint8_t digits[4];
    
      if (num == 0 && !leading_zero) {

        // Singular case - take care separately
        for(uint8_t i = 0; i < (length-1); i++)
          digits[i] = 0;
        digits[length-1] = encodeDigit(0);
      }
      else {       
        for(int i = length-1; i >= 0; --i)
        {
            uint8_t digit = num % base;
          
          if (digit == 0 && num == 0 && leading_zero == false)
              // Leading zero is blank
            digits[i] = 0;
          else
              digits[i] = encodeDigit(digit);
            
          if (digit == 0 && num == 0 && negative) {
              digits[i] = minusSegments;
            negative = false;
          }
    
          num /= base;
        }
        }
      
      if(dots != 0)
      {
        showDots(dots, digits);
      }

        
      setSegments(digits, length, pos);
    }

    void showDots(uint8_t dots, uint8_t* digits)
    {
        for(int i = 0; i < 4; ++i)
        {
            digits[i] |= (dots & 0x80);
            dots <<= 1;
        }
    }
 
  private:
    byte clkPin = 0;
    byte dioPin = 0;
    byte brightness = 8;
    int displayedValue = 0;
    uint8_t displayedDigits[4] = { 0,0,0,0 };

    void stop()
    {
      pinMode(dioPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
      pinMode(clkPin, INPUT);
      delayMicroseconds(delayInMicrosec);
      pinMode(dioPin, INPUT);
      delayMicroseconds(delayInMicrosec);
    }

    bool writeByte(byte b)
    {
      byte data = b;
      for (byte i = 0; i < 8; i++)
      {
        pinMode(clkPin, OUTPUT);
        //delayMicroseconds(delayInMicrosec);
        pinMode(dioPin, (data & 0x01 ? INPUT : OUTPUT));       
        //delayMicroseconds(delayInMicrosec);
        pinMode(clkPin, INPUT);
        delayMicroseconds(delayInMicrosec);
        data = data >> 1;
      }
    
      // Wait for acknowledge
      pinMode(clkPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
      pinMode(dioPin, INPUT);
      //delayMicroseconds(delayInMicrosec);
      pinMode(clkPin, INPUT);
      //delayMicroseconds(delayInMicrosec);
      byte ack = digitalRead(dioPin);
      pinMode(dioPin, OUTPUT);
      delayMicroseconds(delayInMicrosec);
      pinMode(clkPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
   
      return ack;
    }

    void setSegments(const byte segments[], byte length, byte pos)
    {
      pinMode(dioPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
      writeByte(TM1637_I2C_COMM1); // Write COMM1
      //delayMicroseconds(delayInMicrosec);
      stop();
    
      pinMode(dioPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
      writeByte(TM1637_I2C_COMM2 + (pos & 0x03)); // Write COMM2 + first digit address
      for (byte k = 0; k < length; k++) // Write the data bytes
      { 
        writeByte(segments[k]);
      }
      //delayMicroseconds(delayInMicrosec);
      stop();
    
      pinMode(dioPin, OUTPUT);
      //delayMicroseconds(delayInMicrosec);
      writeByte(TM1637_I2C_COMM3 + (brightness & 0x0f)); // Write COMM3 + brightness
      //delayMicroseconds(delayInMicrosec);
      stop();
    }

    byte encodeDigit(byte digit)
    {
      return digitToSegment[digit & 0x0f];
    }
    
};

#endif
