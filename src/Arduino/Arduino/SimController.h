#pragma once
void onExpanderInterrupt();

#ifndef SRC_DANSIMCONTROLLER_H_
#define SRC_DANSIMCONTROLLER_H_

struct State {

  uint8_t IsDaniClientConnected = 0;
  uint8_t IsSimConnectConnected = 0;
  
  uint8_t IsAutopilotMasterEnabled = 0;
  uint8_t IsAutopilotHeadingEnabled = 0;
  uint8_t IsAutopilotAltitudeEnabled = 0;
  uint8_t IsAutopilotAirspeedEnabled = 0;
  uint8_t IsAutopilotVerticalSpeedEnabled = 0;
  uint8_t IsAutopilotYawDamperEnabled = 0;

  
  uint8_t IsLeftGearMoving = 0;
  uint8_t IsCenterGearMoving = 0;
  uint8_t IsRightGearMoving = 0;
  
  uint8_t IsLeftGearOut = 0;
  uint8_t IsCenterGearOut = 0;
  uint8_t IsRightGearOut = 0;

  uint8_t IsFlapNonZero = 0;
  uint8_t IsBrakeNonZero = 0;

  uint8_t IsParkingBrakeEnabled = 0;
  uint8_t IsAutoThrottleEnabled = 0;
};

class SimController
{
  public:

    SimController() 
    {
      _state = State();  
    }

    void begin()
    {
      pinMode(8, OUTPUT);
      pinMode(9, OUTPUT);
      pinMode(10, OUTPUT);
    }
    
    void handle(Mcp mcpOutput)
    {
      read();
      write(mcpOutput);
    }
        
  private:
    State _state;
    uint8_t buffer[16];
    
    bool read()
    {
        int availableBytes = Serial.available();
        if (availableBytes > 0)
        {
          if (availableBytes != 4)
          {        
            while(Serial.available()) Serial.read();
          }
          else 
          {
            Serial.readBytes(buffer, 4);
    
            uint8_t byte1 = buffer[0];
            uint8_t byte2 = buffer[1];
            uint8_t byte3 = buffer[2];
            uint8_t byte4 = buffer[3];
    
            _state.IsDaniClientConnected = bitRead(byte1, 0);
            _state.IsSimConnectConnected = bitRead(byte1, 1);
         
            _state.IsAutopilotMasterEnabled = bitRead(byte2, 0);
            _state.IsAutopilotHeadingEnabled = bitRead(byte2, 1);
            _state.IsAutopilotAltitudeEnabled = bitRead(byte2, 2);
            _state.IsAutopilotAirspeedEnabled = bitRead(byte2, 3);
            _state.IsAutopilotVerticalSpeedEnabled = bitRead(byte2, 4);
            _state.IsAutopilotYawDamperEnabled = bitRead(byte2, 5);

            _state.IsLeftGearMoving = bitRead(byte3, 0);
            _state.IsCenterGearMoving = bitRead(byte3, 1);
            _state.IsRightGearMoving = bitRead(byte3, 2);
    
            _state.IsLeftGearOut = bitRead(byte3, 3);
            _state.IsCenterGearOut = bitRead(byte3, 4);
            _state.IsRightGearOut = bitRead(byte3, 5);
            
            _state.IsFlapNonZero = bitRead(byte3, 6);
            _state.IsBrakeNonZero = bitRead(byte3, 7);
    
            _state.IsParkingBrakeEnabled = bitRead(byte4, 0);
            _state.IsAutoThrottleEnabled = bitRead(byte4, 1);

            return true;
          }
        }

        return false;
    }

    void write(Mcp mcpOutput)
    {      
      long m = millis();
      long blinkState = (m / 125) % 2 == 0;
    
      uint16_t gpio = 0;
      bitWrite(gpio, 15, 1); // always on
    
      digitalWrite(8, LOW);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
    
      if (!_state.IsDaniClientConnected)
      {
        bitWrite(gpio, 14, LOW); // no client
      }
      else if (!_state.IsSimConnectConnected)
      {
        bitWrite(gpio, 14, blinkState); // no sim
      }
      else
      {
        bitWrite(gpio, 14, HIGH); // all ok        
        
        bitWrite(gpio, 7, _state.IsBrakeNonZero);
        bitWrite(gpio, 8, _state.IsFlapNonZero);
        bitWrite(gpio, 9, _state.IsAutopilotMasterEnabled);
        bitWrite(gpio, 10, _state.IsAutopilotHeadingEnabled);
        bitWrite(gpio, 11, _state.IsAutopilotAltitudeEnabled);
        bitWrite(gpio, 12, _state.IsAutopilotAirspeedEnabled);
        bitWrite(gpio, 13, _state.IsAutopilotVerticalSpeedEnabled);
        bitWrite(gpio, 0, _state.IsParkingBrakeEnabled);     
        bitWrite(gpio, 5, _state.IsAutoThrottleEnabled);
        bitWrite(gpio, 4, _state.IsAutopilotYawDamperEnabled);
      
        digitalWrite(8, _state.IsLeftGearMoving ? blinkState : _state.IsLeftGearOut);
        digitalWrite(9, _state.IsCenterGearMoving ? blinkState : _state.IsCenterGearOut);
        digitalWrite(10, _state.IsRightGearMoving ? blinkState : _state.IsRightGearOut);
      }
    
      mcpOutput.writeGpio(gpio);    
    }
    
};
#endif
