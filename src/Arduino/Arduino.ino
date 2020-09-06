bool isProcessingIncomingData = false;
char commandStartCharacter = '|';
char endOfCommandCharacter = '\n';
char inputBuffer[32];
int cursor = 0; 
float state[64];

void setup()
{
  Serial.begin(9600);
  pinMode(7, OUTPUT);
}

void loop()
{
  readSerial();
  writeState();
}

void writeState()
{
  writeGear();
}

void writeGear()
{
  float value = state[0];
  if(value > 0 && value < 1)
  {
    blink(7);
  }
  else if (value == 0)
  {
    digitalWrite(7, LOW);  
  }
  else
  {
    digitalWrite(7, HIGH);  
  }
}

void blink(int pin)
{
  digitalWrite(7, HIGH);
  delay(50);
  digitalWrite(7, LOW);
  delay(50);
}


void readSerial()
{
  while (Serial.available() > 0)
  {
    Serial.println("van kari");
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

void interpretIncomingData()
{
  inputBuffer[cursor + 1] = 0;
  isProcessingIncomingData = false;
  cursor = 0;
  char* separator = strchr(inputBuffer, ':');
  int key = atoi(inputBuffer);
  float value = atof(++separator);
  state[key] = value;
  Serial.println(key);
  Serial.println(value);
}
