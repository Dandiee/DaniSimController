#ifndef HID_h
#define HID_h

#include <stdint.h>
#include <Arduino.h>
#include "PluggableUSB.h"

#if defined(USBCON)



#define _USING_HID

// HID 'Driver'
// ------------
#define HID_GET_REPORT        0x01
#define HID_GET_IDLE          0x02
#define HID_GET_PROTOCOL      0x03
#define HID_SET_REPORT        0x09
#define HID_SET_IDLE          0x0A
#define HID_SET_PROTOCOL      0x0B

#define HID_HID_DESCRIPTOR_TYPE         0x21
#define HID_REPORT_DESCRIPTOR_TYPE      0x22
#define HID_PHYSICAL_DESCRIPTOR_TYPE    0x23

// HID subclass HID1.11 Page 8 4.2 Subclass
#define HID_SUBCLASS_NONE 0
#define HID_SUBCLASS_BOOT_INTERFACE 1

// HID Keyboard/Mouse bios compatible protocols HID1.11 Page 9 4.3 Protocols
#define HID_PROTOCOL_NONE 0
#define HID_PROTOCOL_KEYBOARD 1
#define HID_PROTOCOL_MOUSE 2

// Normal or bios protocol (Keyboard/Mouse) HID1.11 Page 54 7.2.5 Get_Protocol Request
// "protocol" variable is used for this purpose.
#define HID_BOOT_PROTOCOL	0
#define HID_REPORT_PROTOCOL	1

// HID Request Type HID1.11 Page 51 7.2.1 Get_Report Request
#define HID_REPORT_TYPE_INPUT   1
#define HID_REPORT_TYPE_OUTPUT  2
#define HID_REPORT_TYPE_FEATURE 3

#define D_HIDREPORT(length) { 9, 0x21, 0x01, 0x01, 0, 1, 0x22, lowByte(length), highByte(length) }

typedef struct
{
  uint8_t len;      // 9
  uint8_t dtype;    // 0x21
  uint8_t addr;
  uint8_t versionL; // 0x101
  uint8_t versionH; // 0x101
  uint8_t country;
  uint8_t desctype; // 0x22 report
  uint8_t descLenL;
  uint8_t descLenH;
} HIDDescDescriptor;

typedef struct 
{
  InterfaceDescriptor hid;
  HIDDescDescriptor   desc;
  EndpointDescriptor  in;
} HIDDescriptor;

class HIDSubDescriptor {
public:
  HIDSubDescriptor *next = NULL;
  HIDSubDescriptor(const void *d, const uint16_t l) : data(d), length(l) { }

  const void* data;
  const uint16_t length;
};

class HID_ : public PluggableUSBModule
{
public:
  
HID_::HID_(void) : PluggableUSBModule(1, 1, epType),
                   rootNode(NULL), descriptorSize(0),
                   protocol(HID_REPORT_PROTOCOL), idle(1)
{
  epType[0] = EP_TYPE_INTERRUPT_IN;
  PluggableUSB().plug(this);
}

  int HID_::begin(void)
{
  return 0;
}
  
int HID_::SendReport(uint8_t id, const void* data, int len)
{
  auto ret = USB_Send(pluggedEndpoint, &id, 1);
  if (ret < 0) return ret;
  auto ret2 = USB_Send(pluggedEndpoint | TRANSFER_RELEASE, data, len);
  if (ret2 < 0) return ret2;
  return ret + ret2;
}
  

void HID_::AppendDescriptor(HIDSubDescriptor *node)
{
  if (!rootNode) {
    rootNode = node;
  } else {
    HIDSubDescriptor *current = rootNode;
    while (current->next) {
      current = current->next;
    }
    current->next = node;
  }
  descriptorSize += node->length;
}

protected:
  // Implementation of the PluggableUSBModule
  int HID_::getInterface(uint8_t* interfaceCount)
{
  *interfaceCount += 1; // uses 1
  HIDDescriptor hidInterface = {
    D_INTERFACE(pluggedInterface, 1, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
    D_HIDREPORT(descriptorSize),
    D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 0x01)
  };
  return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}
  
int HID_::getDescriptor(USBSetup& setup)
{
  // Check if this is a HID Class Descriptor request
  if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) { return 0; }
  if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) { return 0; }

  // In a HID Class Descriptor wIndex cointains the interface number
  if (setup.wIndex != pluggedInterface) { return 0; }

  int total = 0;
  HIDSubDescriptor* node;
  for (node = rootNode; node; node = node->next) {
    int res = USB_SendControl(TRANSFER_PGM, node->data, node->length);
    if (res == -1)
      return -1;
    total += res;
  }
  
  // Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
  // due to the USB specs, but Windows and Linux just assumes its in report mode.
  protocol = HID_REPORT_PROTOCOL;
  
  return total;
}
  

bool HID_::setup(USBSetup& setup)
{
  if (pluggedInterface != setup.wIndex) {
    return false;
  }

  uint8_t request = setup.bRequest;
  uint8_t requestType = setup.bmRequestType;

  if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
  {
    if (request == HID_GET_REPORT) {
      // TODO: HID_GetReport();
      return true;
    }
    if (request == HID_GET_PROTOCOL) {
      // TODO: Send8(protocol);
      return true;
    }
    if (request == HID_GET_IDLE) {
      // TODO: Send8(idle);
    }
  }

  if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
  {
    if (request == HID_SET_PROTOCOL) {
      // The USB Host tells us if we are in boot or report mode.
      // This only works with a real boot compatible device.
      protocol = setup.wValueL;
      return true;
    }
    if (request == HID_SET_IDLE) {
      idle = setup.wValueL;
      return true;
    }
    if (request == HID_SET_REPORT)
    {
      //uint8_t reportID = setup.wValueL;
      //uint16_t length = setup.wLength;
      //uint8_t data[length];
      // Make sure to not read more data than USB_EP_SIZE.
      // You can read multiple times through a loop.
      // The first byte (may!) contain the reportID on a multreport.
      //USB_RecvControl(data, length);
    }
  }

  return false;
}
  

uint8_t HID_::getShortName(char *name)
{
  name[0] = 'H';
  name[1] = 'I';
  name[2] = 'D';
  name[3] = 'A' + (descriptorSize & 0x0F);
  name[4] = 'A' + ((descriptorSize >> 4) & 0x0F);
  return 5;
}

private:
  uint8_t epType[1];

  HIDSubDescriptor* rootNode;
  uint16_t descriptorSize;

  uint8_t protocol;
  uint8_t idle;
};

// Replacement for global singleton.
// This function prevents static-initialization-order-fiasco
// https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
HID_& HID()
{
  static HID_ obj;
  return obj;
}



#endif // USBCON

#endif // HID_h
