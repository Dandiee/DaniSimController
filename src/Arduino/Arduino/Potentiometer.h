// Potentiometer.h

#ifndef _POTENTIOMETER_h
#define _POTENTIOMETER_h

const uint8_t POTENTIOMETER_MEASURES = 32;

class Potentiometer
{
public:

  Potentiometer(uint8_t analogPin, long rangeMin, long rangeMax, bool isInverse) 
  : _pin(analogPin), _value(0), _cursor(0), _rollingTotal(0), isInverse(isInverse)
  { 
    _rangeMin = rangeMin;
    _rangeMax = rangeMax;
  }

  bool detectChanges() {

    int currentValue = analogRead(_pin);
    _cursor = (_cursor + 1) % POTENTIOMETER_MEASURES;
    uint16_t valueToRemove = _readings[_cursor];
    _rollingTotal = _rollingTotal + currentValue - valueToRemove;
    _readings[_cursor] = currentValue;

    uint16_t newValue = _rollingTotal / POTENTIOMETER_MEASURES;
    if (abs(newValue - _value) > 2) {
      _value = newValue;  
      mappedValue = map(_value, 0, 1024, _rangeMin, _rangeMax);

      if (isInverse) {
        mappedValue = (-1 * mappedValue) - 1;
      }
      return true;
    }
    return false;
  }
  uint8_t _pin = 0;
  uint16_t _value;
  int16_t  mappedValue;
private:
  
  long _rangeMin = 0;
  long _rangeMax = 0;
  bool isInverse = false;
  
  uint16_t _readings[POTENTIOMETER_MEASURES];
 uint16_t _sorted[POTENTIOMETER_MEASURES];
  uint8_t _cursor = 0;
  uint32_t _rollingTotal = 0;
};
#endif
