#pragma once

#include <Arduino.h>
#include "Libs/HID.h"
#include "Libs/HID-Settings.h"
#include "Libs/GamepadAPI.h"

static const uint8_t _hidMultiReportDescriptorGamepad[] PROGMEM = {
  /* Gamepad with 32 buttons and 6 axis*/
  0x05, 0x01,             /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x04,             /* USAGE (Joystick) */
  0xa1, 0x01,             /* COLLECTION (Application) */
  0x85, HID_REPORTID_GAMEPAD,     /*   REPORT_ID */
  /* 32 Buttons */
  0x05, 0x09,             /*   USAGE_PAGE (Button) */
  0x19, 0x01,             /*   USAGE_MINIMUM (Button 1) */
  0x29, 0x20,             /*   USAGE_MAXIMUM (Button 32) */
  0x15, 0x00,             /*   LOGICAL_MINIMUM (0) */
  0x25, 0x01,             /*   LOGICAL_MAXIMUM (1) */
  0x75, 0x01,             /*   REPORT_SIZE (1) */
  0x95, 0x20,             /*   REPORT_COUNT (32) */
  0x81, 0x02,             /*   INPUT (Data,Var,Abs) */
  /* 4 16bit Axis */
  0x05, 0x01,             /*   USAGE_PAGE (Generic Desktop) */
  0xa1, 0x00,             /*   COLLECTION (Physical) */
  0x05, 0x01,               /* Usage Page (Generic Desktop) */
  
  0x09, 0x30,             /*     USAGE (Analog1: X) */
  0x09, 0x31,             /*     USAGE (Analog2: Y) */
  0x09, 0x32,             /*     USAGE (Analog3: Z) */
  0x09, 0x33,             /*     USAGE (Analog4: Rx) */
  0x09, 0x34,             /*     USAGE (Analog5: Ry) */
  0x09, 0x35,             /*     USAGE (Analog6: Rz) */
  0x09, 0x40,             /*     USAGE (Analog7: Slider) */
  0x09, 0x41,             /*     USAGE (Analog8: Dial) */
  0x09, 0x42,             /*     USAGE (Analog9: Idk) */
  0x09, 0x36,             /*     USAGE (Analog10: Wheel) */
  
  0x16, 0x00, 0x80,         /*     LOGICAL_MINIMUM (-32768) */
  0x26, 0xFF, 0x7F,         /*     LOGICAL_MAXIMUM (32767) */
  0x75, 16,                 /*     REPORT_SIZE (16) */
  0x95, 10,                 /*     REPORT_COUNT (9) */          /* -> REPORT_COUNT (6) */
  0x81, 0x82,               /*     INPUT (Data,Var,Abs) */ // 081 002 volt
  0xc0,               /*   END_COLLECTION */
  
  /* 2 Hat Switches */
  0x05, 0x01,             /*   USAGE_PAGE (Generic Desktop) */
  0x09, 0x39,             /*   USAGE (Hat switch) */
  0x09, 0x39,             /*   USAGE (Hat switch) */
  0x15, 0x01,             /*   LOGICAL_MINIMUM (1) */
  0x25, 0x08,             /*   LOGICAL_MAXIMUM (8) */
  0x95, 0x02,             /*   REPORT_COUNT (2) */
  0x75, 0x04,             /*   REPORT_SIZE (4) */
  0x81, 0x02,             /*   INPUT (Data,Var,Abs) */
  0xc0                /* END_COLLECTION */
};

class Gamepad_ : public GamepadAPI
{
  public:
      Gamepad_::Gamepad_(void)  {
        static HIDSubDescriptor node(_hidMultiReportDescriptorGamepad, sizeof(_hidMultiReportDescriptorGamepad));
        HID().AppendDescriptor(&node);
      }
  
  protected: 
      void Gamepad_::SendReport(void* data, int length) {
        HID().SendReport(HID_REPORTID_GAMEPAD, data, length);
      }
};
Gamepad_ Gamepad;
