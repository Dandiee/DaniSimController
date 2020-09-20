// Potentiometer.h

#ifndef _POTENTIOMETER_h
#define _POTENTIOMETER_h

const uint8_t POTENTIOMETER_MEASURES = 8;

struct potValues {
	uint16_t value;
	uint8_t count;
};

class Potentiometer
{
public:

	Potentiometer(uint8_t analogPin, long rangeMin, long rangeMax) {
		pin = analogPin;
		_rangeMin = rangeMin;
		_rangeMax = rangeMax;

		int currentValue = analogRead(analogPin);

		for (uint8_t i = 0; i < POTENTIOMETER_MEASURES; i++) {
			readings[i] = currentValue;
		}

		_rollingTotal = currentValue * POTENTIOMETER_MEASURES;
		cursor = POTENTIOMETER_MEASURES - 1;
	}

	long readAndGetValue() {

		int currentValue = analogRead(pin);
		cursor = (cursor + 1) % POTENTIOMETER_MEASURES;
		_rollingTotal = _rollingTotal + currentValue - readings[cursor];
		rawValue = _rollingTotal / POTENTIOMETER_MEASURES;
		readings[cursor] = currentValue;

		return rawValue;
	}


	long value;
	uint16_t rawValue;

private:
	uint16_t readings[POTENTIOMETER_MEASURES];
	
	uint8_t cursor = 0;
	uint8_t pin = 0;
	
	uint32_t _rollingTotal = 0;
	
	long _rangeMin = 0;
	long _rangeMax = 0;
};



#endif

