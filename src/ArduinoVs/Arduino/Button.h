#ifndef _BUTTON_h
#define _BUTTON_h

const uint8_t BUTTON_COOLDOWN = 50;


class Button
{
  typedef void (*buttonPressed)(Button sender);
public:

  Button() { }
  Button(const uint8_t pin, bool isGpio, bool isInverted, buttonPressed callback) 
    : pin(pin), isGpio(isGpio), isInverted(isInverted), onPressedCallback(callback) { 
    
  }

  void setup() {
      pinMode(pin, INPUT_PULLUP);
  }

  bool checkState(uint16_t gpio) 
  {
    uint8_t state = isGpio ? bitRead(gpio, pin) : digitalRead(pin);

    if (isInverted) state = !state;
    
    bool isChanged = false;

    if (state != lastKnownState) // does it differ from the previous measurement?
    {
      unsigned long now = millis();
      Serial.println("báltozott");
      if ((now - lastChangedAt) > BUTTON_COOLDOWN) // okay, but enough time passed since the last change?
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
