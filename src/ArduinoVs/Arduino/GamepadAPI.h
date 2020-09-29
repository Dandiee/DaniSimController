#pragma once

#include <Arduino.h>
#include "HID-Settings.h"

// Dpad directions
#define GAMEPAD_DPAD_CENTERED 0
#define GAMEPAD_DPAD_UP 1
#define GAMEPAD_DPAD_UP_RIGHT 2
#define GAMEPAD_DPAD_RIGHT 3
#define GAMEPAD_DPAD_DOWN_RIGHT 4
#define GAMEPAD_DPAD_DOWN 5
#define GAMEPAD_DPAD_DOWN_LEFT 6
#define GAMEPAD_DPAD_LEFT 7
#define GAMEPAD_DPAD_UP_LEFT 8


typedef union {
	// 32 Buttons, 6 Axis, 2 D-Pads
	uint8_t whole8[0];
	uint16_t whole16[0];
	uint32_t whole32[0];
	uint32_t buttons;

	struct{
		uint8_t button1 : 1;
		uint8_t button2 : 1;
		uint8_t button3 : 1;
		uint8_t button4 : 1;
		uint8_t button5 : 1;
		uint8_t button6 : 1;
		uint8_t button7 : 1;
		uint8_t button8 : 1;

		uint8_t button9 : 1;
		uint8_t button10 : 1;
		uint8_t button11 : 1;
		uint8_t button12 : 1;
		uint8_t button13 : 1;
		uint8_t button14 : 1;
		uint8_t button15 : 1;
		uint8_t button16 : 1;

		uint8_t button17 : 1;
		uint8_t button18 : 1;
		uint8_t button19 : 1;
		uint8_t button20 : 1;
		uint8_t button21 : 1;
		uint8_t button22 : 1;
		uint8_t button23 : 1;
		uint8_t button24 : 1;

		uint8_t button25 : 1;
		uint8_t button26 : 1;
		uint8_t button27 : 1;
		uint8_t button28 : 1;
		uint8_t button29 : 1;
		uint8_t button30 : 1;
		uint8_t button31 : 1;
		uint8_t button32 : 1;

		int16_t	xAxis;
		int16_t	yAxis;
		int16_t	zAxis;

		int16_t	rxAxis;
		int16_t	ryAxis;	
		int16_t	rzAxis;
		
		int16_t	slider;
		int16_t	dial;
		int16_t	wheel;

		uint8_t	dPad1 : 4;
		uint8_t	dPad2 : 4;
		
	};
} HID_GamepadReport_Data_t;

class GamepadAPI {
public:
  	GamepadAPI::GamepadAPI(void) { }

  	void GamepadAPI::begin(void) {
      // release all buttons
      end();
    }
    
    void GamepadAPI::end(void) {
      memset(&_report, 0x00, sizeof(_report));
      SendReport(&_report, sizeof(_report));
    }
    
    void GamepadAPI::write(void) { 
      SendReport(&_report, sizeof(_report)); 
    }
    
    
    void GamepadAPI::press(uint8_t b){ 
      _report.buttons |= (uint32_t)1 << (b - 1); 
    }
    
    
    void GamepadAPI::release(uint8_t b){ 
      _report.buttons &= ~((uint32_t)1 << (b - 1)); 
    }
    
    
    void GamepadAPI::releaseAll(void){ 
      memset(&_report, 0x00, sizeof(_report)); 
    }
    
    void GamepadAPI::buttons(uint32_t b){ 
      _report.buttons = b; 
    }
    
    
    void GamepadAPI::xAxis(int16_t a){ 
      _report.xAxis = a; 
    }
    
    
    void GamepadAPI::yAxis(int16_t a){ 
      _report.yAxis = a; 
    }
    
    
    void GamepadAPI::zAxis(int16_t a){ 
      _report.zAxis = a; 
    }
    
    
    void GamepadAPI::rxAxis(int16_t a){ 
      _report.rxAxis = a; 
    }
    
    
    void GamepadAPI::ryAxis(int16_t a){ 
      _report.ryAxis = a; 
    }
    
    
    void GamepadAPI::rzAxis(int16_t a){ 
      _report.rzAxis = a; 
    }
    
    void GamepadAPI::slider(int16_t a){ 
      _report.slider = a; 
    }
    
    void GamepadAPI::dial(int16_t a){ 
      _report.dial = a; 
    }
    
    void GamepadAPI::wheel(int16_t a){ 
      _report.wheel = a;
    }
    
    
    void GamepadAPI::dPad1(int8_t d){ 
      _report.dPad1 = d; 
    }
    
    
    void GamepadAPI::dPad2(int8_t d){ 
      _report.dPad2 = d; 
    }


	// Sending is public in the base class for advanced users.
	virtual void SendReport(void* data, int length) = 0;

protected:
	HID_GamepadReport_Data_t _report;
};

// Implementation is inline
#include "GamepadAPI.h"
