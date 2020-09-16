#ifndef _BUTTON_h
#define _BUTTON_h

const uint8_t BUTTON_COOLDOWN = 50;


class Button
{
	typedef void (*buttonPressed)(Button sender);
public:

	Button::Button() { }
	Button::Button(const uint8_t pin, buttonPressed callback) 
		: pin(pin), onPressedCallback(callback) { }

	void checkState(uint16_t gpio) 
	{
		uint8_t state = bitRead(gpio, pin);

		if (state != lastKnownState)
		{
			unsigned long now = millis();
			if ((now - lastKnownState) > BUTTON_COOLDOWN)
			{
				if (lastKnownState && !state)
				{
					onPressedCallback(*this);
				}

				lastKnownState = state;
				lastChangedAt = now;
			}
		}
	}
	uint8_t pin = 0;
	uint8_t lastKnownState = 0;
private:
	uint8_t cooldown = 0;
	unsigned long lastChangedAt = 0;
	buttonPressed onPressedCallback = nullptr;

};
#endif