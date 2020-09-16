// Potentiometer.h

#ifndef _POTENTIOMETER_h
#define _POTENTIOMETER_h

const uint8_t POTENTIOMETER_MEASURES = 8;

class Potentiometer
{
public:

	Potentiometer(uint8_t analogPin, long rangeMin, long rangeMax) {
		pin = analogPin;
		_rangeMin = rangeMin;
		_rangeMax = rangeMax;

		for (uint8_t i = 0; i < POTENTIOMETER_MEASURES; i++)
		{
			lastReadings[i] = 0;
		}
	}

	long readAndGetValue() {

		int currentValue = analogRead(pin);
		int originalValue = 0;

		lastReadings[cursor] = currentValue;
		uint16_t total = lastReadings[0];
		for (uint8_t i = 1; i < POTENTIOMETER_MEASURES; i++) {
			total += lastReadings[i];
		}

		uint16_t avg = total / POTENTIOMETER_MEASURES;
		if (abs(avg - rawValue) > 1)
		{
			rawValue = avg;
			value = map(rawValue, 0, 1024, _rangeMin, _rangeMax);
		}
		cursor = cursor == POTENTIOMETER_MEASURES - 1 ? 0 : cursor + 1;

		return value;
	}


	long value;
	uint16_t rawValue;

private:
	uint16_t lastReadings[POTENTIOMETER_MEASURES];
	uint8_t cursor = 0;
	uint8_t pin = 0;
	long _rangeMin = 0;
	long _rangeMax = 0;
};


#endif

