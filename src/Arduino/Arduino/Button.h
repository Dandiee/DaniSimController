#ifndef _BUTTON_h
#define _BUTTON_h

const uint8_t BUTTON_COOLDOWN = 50;


class Button
{
  typedef void (*buttonPressed)(Button sender);
public:

  Button(const uint8_t pin, bool isGpio, bool isInverted, buttonPressed callback) 
    : pin(pin), isGpio(isGpio), isInverted(isInverted), onPressedCallback(callback) { 
  }

  void begin() {
    if (!isGpio) {      
      pinMode(pin, INPUT_PULLUP);
    }
  }

  bool detectChanges(uint16_t gpio) 
  {
    uint8_t state = isGpio ? bitRead(gpio, pin) : digitalRead(pin);

    if (isInverted) state = !state;
    
    bool isChanged = false;

    if (state != lastKnownState)
    {
      unsigned long now = millis();
      if ((now - lastChangedAt) > BUTTON_COOLDOWN)
      {
        if (lastKnownState && !state)
        {
          onPressedCallback(*this);
        }

        lastKnownState = state;
        lastChangedAt = now;
        isChanged = true;
      }
    }

    return isChanged;
  }
  uint8_t pin = 0;
  uint8_t lastKnownState = 0;
private:
  uint8_t cooldown = 0;
  bool isGpio = false;
  bool isInverted = false;
  unsigned long lastChangedAt = 0;
  buttonPressed onPressedCallback = nullptr;

};
#endif
