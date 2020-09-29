#ifndef _BUTTON_h
#define _BUTTON_h

const uint8_t BUTTON_COOLDOWN = 50;


class Button
{
  typedef void (*buttonPressed)(Button sender);
public:

  Button() { }
  Button(const uint8_t pin, buttonPressed callback) 
    : pin(pin), onPressedCallback(callback) { 
    
  }

  void setup() {
    Serial.println("BUTTON IS TOTALLY SET TO: " + String(pin));
      pinMode(pin, INPUT_PULLUP);
  }

  bool checkState() 
  {
    uint8_t state = digitalRead(pin);

    bool isChanged = false;

    if (state != lastKnownState) // does it differ from the previous measurement?
    {
      unsigned long now = millis();
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
  unsigned long lastChangedAt = 0;
  buttonPressed onPressedCallback = nullptr;

};
#endif
