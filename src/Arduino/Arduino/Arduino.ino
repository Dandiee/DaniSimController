#include "Potentiometer.h"
#include "Button.h"
#include "Settings.h"
#include "Mcp/McpSettings.h"
#include "Mcp/Mcp.h"
#include "Mcp/McpBuilder.h"
#include "Encoder.h"
#include "SimController.h"
#include "Gamepad.h"

void onEncoderChanged(int8_t change, byte id, int value);
void onButtonPressed(Button sender);
void onSimStateChanged(byte key, int value);

volatile bool isExpanderInterrupted = false;

Mcp mcpInput = McpBuilder(MCP_INPUT_SS_PIN)
  .withPinDirections(0xFFFF)
  .withPullUps(0xFFFF)
  .withIoPolarity(0xFFFF)
  //.withInterrupts(0xFFFF)-- I'll just poll read the gpio state
  .withInterrupts(0x0000)
  //.withInterruptPin(MCP_INPUT_INTERRUPT_PIN)
  .withIconMirror(1)
  .withIconSequentialOperation(1)
  .withIconHardwareAddress(1)
  .build();

Mcp mcpOutput = McpBuilder(MCP_OUTPUT_SS_PIN)
  .withPinDirections(0x0000)
  .withPullUps(0x0000)
  .withIconMirror(0)
  .withIconSequentialOperation(0)
  .withIconHardwareAddress(0)
  .build();

SimController simController = SimController();

Button encoderButton1 = Button(ENC_0_BTN_GPIO_PIN, false, false, onButtonPressed);
Button encoderButton2 = Button(ENC_1_BTN_GPIO_PIN, false, false, onButtonPressed);
Button encoderButton3 = Button(ENC_2_BTN_GPIO_PIN, false, false, onButtonPressed);
Button encoderButton4 = Button(ENC_3_BTN_GPIO_PIN, false, false, onButtonPressed);

Button button1 = Button(BTN_0_GPIO_PIN, true, true, onButtonPressed);
Button button2 = Button(BTN_1_GPIO_PIN, true, true, onButtonPressed);
Button button3 = Button(BTN_2_GPIO_PIN, true, true, onButtonPressed);
Button button4 = Button(BTN_3_GPIO_PIN, true, true, onButtonPressed);
Button button5 = Button(BTN_4_GPIO_PIN, true, true, onButtonPressed);
Button button6 = Button(BTN_5_GPIO_PIN, true, true, onButtonPressed);
Button button7 = Button(BTN_6_GPIO_PIN, true, true, onButtonPressed);
Button button8 = Button(BTN_7_GPIO_PIN, true, true, onButtonPressed);

Encoder encoder1 = Encoder(ENC_0_GPIO_B_PINS, ENC_0_GPIO_A_PINS, 1, onEncoderChanged);
Encoder encoder2 = Encoder(ENC_1_GPIO_B_PINS, ENC_1_GPIO_A_PINS, 2, onEncoderChanged);
Encoder encoder3 = Encoder(ENC_2_GPIO_B_PINS, ENC_2_GPIO_A_PINS, 3, onEncoderChanged);
Encoder encoder4 = Encoder(ENC_3_GPIO_B_PINS, ENC_3_GPIO_A_PINS, 4, onEncoderChanged);

Potentiometer pot1 = Potentiometer(POT_0_PIN, -32768, 32768, false);
Potentiometer pot2 = Potentiometer(POT_1_PIN, -32768, 32768, true);
Potentiometer pot3 = Potentiometer(POT_2_PIN, -32768, 32768, true);
Potentiometer pot4 = Potentiometer(POT_3_PIN, -32768, 32768, true);
Potentiometer pot5 = Potentiometer(POT_4_PIN, -32768, 32768, false);

Encoder* encoders[] = { &encoder1, &encoder2, &encoder3, &encoder4};
Button* buttons[] = 
{ 
    &encoderButton1, &encoderButton2, &encoderButton3, &encoderButton4, 
    &button1, &button2, &button3, &button4, &button5, &button6, &button7, &button8
};
Potentiometer* pots[] = { &pot1, &pot2, &pot3, &pot4, &pot5 };

uint8_t encodersCount = (sizeof(encoders)/sizeof(*encoders));
uint8_t buttonsCount = (sizeof(buttons)/sizeof(*buttons));
uint8_t potsCount = (sizeof(pots)/sizeof(*pots));

void setup()
{
  Serial.begin(115200);
  SPI.begin();

  mcpInput.begin();
  mcpOutput.begin();
  simController.begin();
  
  for (uint8_t i = 0; i < buttonsCount; i++) {
      buttons[i]->begin();
  }

  Gamepad.begin();
}

void loop()
{
  uint16_t gpio = mcpInput.readGpio();
  bool isChanged = false;

  for (uint8_t i = 0; i < encodersCount; i++) 
  {
      isChanged |= encoders[i]->process(gpio);
  }

  for (uint8_t i = 0; i < buttonsCount; i++) 
  {
      isChanged |= buttons[i]->checkState(gpio);
  }

  for (uint8_t i = 0; i < potsCount; i++) 
  {
      isChanged |= pots[i]->readAndGetValue();
  }
  
  if (isChanged)
  {
    sendGamepadReport();
  }

  simController.handle(mcpOutput);
}

void sendGamepadReport()
{
  Gamepad.buttons(
    !encoderButton1.lastKnownState
    | !encoderButton2.lastKnownState << 1
    | !encoderButton3.lastKnownState << 2
    | !encoderButton4.lastKnownState << 3
    | !button1.lastKnownState << 4
    | !button2.lastKnownState << 5
    | !button3.lastKnownState << 6
    | !button4.lastKnownState << 7
    | !button5.lastKnownState << 8
    | !button6.lastKnownState << 9
    | !button7.lastKnownState << 10
    | !button8.lastKnownState << 11);  

  Gamepad.analog1(pot1.mappedValue);
  Gamepad.analog2(pot2.mappedValue);
  Gamepad.analog3(pot3.mappedValue);
  Gamepad.analog4(pot4.mappedValue);
  Gamepad.analog5(pot5.mappedValue);
  
  Gamepad.analog6(encoder1.value);
  Gamepad.analog7(encoder2.value);
  Gamepad.analog8(encoder3.value);
  Gamepad.analog9(encoder4.value);
  
  Gamepad.write();
}

// For debug:
uint16_t readInterrupt() 
{
  
}

void onEncoderChanged(int8_t change, uint8_t id, int value)
{ 

}

void onButtonPressed(Button sender)
{

}

void onExpanderInterrupt() 
{
  isExpanderInterrupted = true;  
}
