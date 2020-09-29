#pragma once

#include <Arduino.h>
#include "HID.h"
#include "HID-Settings.h"
#include "GamepadAPI.h"

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
  0x05, 0x01, /* Usage Page (Generic Desktop) */
  0x09, 0x30,             /*     USAGE (X) */
  0x09, 0x31,             /*     USAGE (Y) */
  0x09, 0x32,             /*     USAGE (Z) */
  0x09, 0x33,             /*     USAGE (Rx) */
  0x09, 0x34,             /*     USAGE (Ry) */
  0x09, 0x35,             /*     USAGE (Rz) */
  0x09, 0x36,             /*     USAGE (Slider) */
  0x09, 0x37,             /*     USAGE (Dial) */
  0x09, 0x40,             /*     USAGE (Wheel) */
  0x16, 0x00, 0x80,         /*     LOGICAL_MINIMUM (-32768) */
  0x26, 0xFF, 0x7F,         /*     LOGICAL_MAXIMUM (32767) */
  0x75, 16,             /*     REPORT_SIZE (16) */
  0x95, 9,              /*     REPORT_COUNT (9) */          /* -> REPORT_COUNT (6) */
  0x81, 0x82,             /*     INPUT (Data,Var,Abs) */ // 081 002 volt
  /* 2 8bit Axis */

  // 0x15, 0x80,              /*     LOGICAL_MINIMUM (-128) */
  // 0x25, 0x7F,              /*     LOGICAL_MAXIMUM (127) */
  // 0x75, 0x08,              /*     REPORT_SIZE (8) */
  // 0x95, 0x02,              /*     REPORT_COUNT (2) */
  // 0x81, 0x02,              /*     INPUT (Data,Var,Abs) */
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
  
   //Gamepad_ Gamepad;
};
Gamepad_ Gamepad;
