void onExpanderInterrupt();

#ifndef SRC_DANSIMCONTROLLER_H_
#define SRC_DANSIMCONTROLLER_H_

const char commandStartCharacter = '|';
const char endOfCommandCharacter = '\n';

typedef void (*simStateChanged)(byte key, int value);
class SimController
{
  public:
    SimController(simStateChanged callback) : callback(callback) { } 
    int state[16];

    void readSerial()
    {
      while (Serial.available() > 0)
      {
        char currentChar = Serial.read();
        
        if (isProcessingIncomingData)
        {
          if (currentChar == endOfCommandCharacter)
          {
            interpretIncomingData();
          }
          else
          {
            inputBuffer[cursor] = currentChar;
            cursor++;
          }
        }
        else if(currentChar == commandStartCharacter)
        {
          isProcessingIncomingData = true;
        }
      }
    }
        
  private:
    simStateChanged callback = nullptr;
    char inputBuffer[32];
    int cursor = 0; 
    bool isProcessingIncomingData = false;
    
    void interpretIncomingData()
    {
      inputBuffer[cursor + 1] = 0;
      isProcessingIncomingData = false;
      cursor = 0;
      char* separator = strchr(inputBuffer, ':');
      byte key = atoi(inputBuffer);
      int value = atoi(++separator);

      int prevValue = state[key];
      if (prevValue != value)
      {
        state[key] = value;  
        callback(key, value);
      }
    }
    
};

#endif
