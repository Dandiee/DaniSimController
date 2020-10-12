#include "Potentiometer.h"
#include "Button.h"
#include "Settings.h"
#include "McpSettings.h"
#include "Mcp.h"
#include "McpBuilder.h"
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

Encoder encoders[] = { 
  Encoder(ENC_0_GPIO_B_PINS, ENC_0_GPIO_A_PINS, 1, onEncoderChanged),
  Encoder(ENC_1_GPIO_B_PINS, ENC_1_GPIO_A_PINS, 2, onEncoderChanged),
  Encoder(ENC_2_GPIO_B_PINS, ENC_2_GPIO_A_PINS, 3, onEncoderChanged),
  Encoder(ENC_3_GPIO_B_PINS, ENC_3_GPIO_A_PINS, 4, onEncoderChanged)
};

Button buttons[] =  { 
  Button(ENC_0_BTN_GPIO_PIN, false, false, onButtonPressed),
  Button(ENC_1_BTN_GPIO_PIN, false, false, onButtonPressed),
  Button(ENC_2_BTN_GPIO_PIN, false, false, onButtonPressed),
  Button(ENC_3_BTN_GPIO_PIN, false, false, onButtonPressed),
  
  Button(BTN_0_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_1_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_2_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_3_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_4_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_5_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_6_GPIO_PIN, true, true, onButtonPressed),
  Button(BTN_7_GPIO_PIN, true, true, onButtonPressed)
};

Potentiometer pots[] = { 
  Potentiometer(POT_0_PIN, -32768, 32768, false),
  Potentiometer(POT_1_PIN, -32768, 32768, true),
  Potentiometer(POT_2_PIN, -32768, 32768, true),
  Potentiometer(POT_3_PIN, -32768, 32768, true),
  Potentiometer(POT_4_PIN, -32768, 32768, false)
};

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
      buttons[i].begin();
  }

  Gamepad.begin();
}

void loop()
{
  uint16_t gpio = mcpInput.readGpio();
  bool isChanged = false;

  for (uint8_t i = 0; i < encodersCount; i++) 
  {
      isChanged |= encoders[i].detectChanges(gpio);
  }

  for (uint8_t i = 0; i < buttonsCount; i++) 
  {
      isChanged |= buttons[i].detectChanges(gpio);
  }

  for (uint8_t i = 0; i < potsCount; i++) 
  {
      isChanged |= pots[i].detectChanges();
  }
  
  if (isChanged)
  {
    sendGamepadReport();
  }

  simController.handle(mcpOutput);
}

void sendGamepadReport()
{
  uint32_t buttonStates = 0; 
  for (uint8_t i = 0; i < buttonsCount; i++) {
    bitWrite(buttonStates, i, !buttons[i].lastKnownState);
  }
  Gamepad.buttons(buttonStates);

  Gamepad.analog1(pots[0].mappedValue);
  Gamepad.analog2(pots[1].mappedValue);
  Gamepad.analog3(pots[2].mappedValue);
  Gamepad.analog4(pots[3].mappedValue);
  Gamepad.analog5(pots[4].mappedValue);
  
  Gamepad.analog6(encoders[0].value);
  Gamepad.analog7(encoders[1].value);
  Gamepad.analog8(encoders[2].value);
  Gamepad.analog9(encoders[3].value);
  
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
