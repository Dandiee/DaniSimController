#include "Potentiometer.h"
#include "Button.h"
#include "Settings.h"
#include "Mcp/McpSettings.h"
#include "Mcp/Mcp.h"
#include "Mcp/McpBuilder.h"
#include "Encoder.h"
// #include "Display.h"
#include "SimController.h"
#include "Gamepad.h"

void onEncoderChanged(int8_t change, byte id, int value);
void onButtonPressed(Button sender);
void onSimStateChanged(byte key, int value);


//SimController simController = SimController(onSimStateChanged);


Mcp mcpInput = McpBuilder(MCP_INPUT_SS_PIN)
  .withPinDirections(0xFFFF)
  .withPullUps(0xFFFF)
  .withIoPolarity(0xFFFF)
  //.withInterrupts(0xFFFF)-- I'll just poll read the gpio state
  .withInterrupts(0x0000)
  .withInterruptPin(MCP_INPUT_INTERRUPT_PIN)
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


volatile bool isExpanderInterrupted = false;

Button encoderButton1 = Button(ENC_0_BTN_GPIO_PIN, true, true, onButtonPressed);
Button encoderButton2 = Button(ENC_1_BTN_GPIO_PIN, true, true, onButtonPressed);
Button encoderButton3 = Button(ENC_2_BTN_GPIO_PIN, true, true, onButtonPressed);
Button encoderButton4 = Button(ENC_3_BTN_GPIO_PIN, true, true, onButtonPressed);

Button button1 = Button(BTN_0_GPIO_PIN, true, true, onButtonPressed);

Encoder encoder1 = Encoder(ENC_0_GPIO_B_PINS, ENC_0_GPIO_A_PINS, 1, onEncoderChanged);
Encoder encoder2 = Encoder(ENC_1_GPIO_B_PINS, ENC_1_GPIO_A_PINS, 2, onEncoderChanged);
Encoder encoder3 = Encoder(ENC_2_GPIO_B_PINS, ENC_2_GPIO_A_PINS, 3, onEncoderChanged);
Encoder encoder4 = Encoder(ENC_3_GPIO_B_PINS, ENC_3_GPIO_A_PINS, 4, onEncoderChanged);
Encoder encoder5 = Encoder(ENC_4_GPIO_B_PINS, ENC_4_GPIO_A_PINS, 5, onEncoderChanged);

Potentiometer pot1 = Potentiometer(POT_0_PIN, -32768, 32768, true);
Potentiometer pot2 = Potentiometer(POT_1_PIN, -32768, 32768, false);
Potentiometer pot3 = Potentiometer(POT_2_PIN, -32768, 32768, false);
Potentiometer pot4 = Potentiometer(POT_3_PIN, -32768, 32768, true);

Encoder* encoders[] = { &encoder1, &encoder2, &encoder3, &encoder4, &encoder5 };
Button* buttons[] = { &encoderButton1, &encoderButton2, &encoderButton3, &encoderButton4, &button1 };
Potentiometer* pots[] = { &pot1, &pot2, &pot3, &pot4 };

uint8_t encodersCount = (sizeof(encoders)/sizeof(*encoders));
uint8_t buttonsCount = (sizeof(buttons)/sizeof(*buttons));
uint8_t potsCount = (sizeof(pots)/sizeof(*pots));

void setup()
{
  Serial.begin(115200);
  
  SPI.begin();
  mcpInput.begin();
  mcpOutput.begin();

  for (uint8_t i = 0; i < buttonsCount; i++) {
      buttons[i]->setup();
  }

  Gamepad.begin();
}

uint16_t readInterrupt() {
  /*if (isExpanderInterrupted) {

    detachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN));
    uint16_t gpio = mcpInput.readGpio();
    Serial.println(gpio);
    isExpanderInterrupted  = false;

    attachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN), onExpanderInterrupt, FALLING);
  }*/
}

int e1 = 0;
int e2 = 0;
int e3 = 0;
int e4 = 0;

int btnState = 0;

int16_t p1 = 0;
int16_t p2 = 0;
int16_t p3 = 0;
int16_t p4 = 0;
int16_t p5 = 0;

uint8_t lastThingy = -1;

void loop()
{
  uint16_t gpio = mcpInput.readGpio();
  
  bool isChanged = false;

  for (uint8_t i = 0; i < encodersCount; i++) {
      isChanged |= encoders[i]->process(gpio);
  }

  for (uint8_t i = 0; i < buttonsCount; i++) {
      isChanged |= buttons[i]->checkState(gpio);
  }

  for (uint8_t i = 0; i < potsCount; i++) {
      isChanged |= pots[i]->readAndGetValue();
  }
  
  if (isChanged)
  {
    sendGamepadReport();
  }
  
  readFromPc();
}

void ledTest() {
  long nextActive = (millis() / 50) % 7;
  for (uint8_t i = 0; i < 7; i++) {
    mcpOutput.writePin(i, nextActive == i);  
  }
}

uint8_t bytes[64];

void readFromPc(){

  int availableBytes = Serial.available();
  if (availableBytes > 0) {
    
      Serial.readBytes(bytes, availableBytes);

      byte firstByte = bytes[0];

      for (uint8_t i = 0; i < 8; i++) {
        mcpOutput.writePin(i, bitRead(firstByte, i));    
      }      
  }
  
}

void sendGamepadReport()
{
 
  Gamepad.buttons(
    !encoderButton1.lastKnownState
    | !encoderButton2.lastKnownState << 1
    | !encoderButton3.lastKnownState << 2
    | !encoderButton4.lastKnownState << 3
    | !button1.lastKnownState << 4);  

  Gamepad.analog1(pot1.mappedValue);
  Gamepad.analog2(pot2.mappedValue);
  Gamepad.analog3(pot3.mappedValue);
  Gamepad.analog4(pot4.mappedValue);
  
  Gamepad.analog5(encoder1.value);
  Gamepad.analog6(encoder2.value);
  Gamepad.analog7(encoder3.value);
  Gamepad.analog8(encoder4.value);
  Gamepad.analog9(encoder5.value);
  
  Gamepad.write();
}

void onEncoderChanged(int8_t change, uint8_t id, int value)
{ 
  Serial.print(id);
  Serial.print(":");
  Serial.print(change);
  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");
}

void onButtonPressed(Button sender)
{
  //Serial.println("Pressed: " + String(sender.pin));
}

void onExpanderInterrupt() {
  isExpanderInterrupted = true;  
}
