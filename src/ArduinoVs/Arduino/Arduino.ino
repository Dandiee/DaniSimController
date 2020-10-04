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

/*
Display display1 = Display(DIS_CLK_PIN, DIS_0_0_DIO_PIN);
Display display2 = Display(DIS_CLK_PIN, DIS_0_1_DIO_PIN);
Display display3 = Display(DIS_CLK_PIN, DIS_1_0_DIO_PIN);
Display display4 = Display(DIS_CLK_PIN, DIS_1_1_DIO_PIN);
Display display5 = Display(DIS_CLK_PIN, DIS_2_0_DIO_PIN);
Display display6 = Display(DIS_CLK_PIN, DIS_2_1_DIO_PIN);
Display display7 = Display(DIS_CLK_PIN, DIS_3_0_DIO_PIN);
Display display8 = Display(DIS_CLK_PIN, DIS_3_1_DIO_PIN);*/

Button encoderButton1 = Button(ENC_0_BTN_GPIO_PIN, onButtonPressed);
Button encoderButton2 = Button(ENC_1_BTN_GPIO_PIN, onButtonPressed);
Button encoderButton3 = Button(ENC_2_BTN_GPIO_PIN, onButtonPressed);
Button encoderButton4 = Button(ENC_3_BTN_GPIO_PIN, onButtonPressed);
Button button1 = Button(BTN_0_GPIO_PIN, onButtonPressed);
Button button2 = Button(BTN_1_GPIO_PIN, onButtonPressed);

Encoder encoder1 = Encoder(ENC_0_GPIO_A_PINS, ENC_0_GPIO_B_PINS, 1, onEncoderChanged);
Encoder encoder2 = Encoder(ENC_1_GPIO_A_PINS, ENC_1_GPIO_B_PINS, 2, onEncoderChanged);
Encoder encoder3 = Encoder(ENC_2_GPIO_A_PINS, ENC_2_GPIO_B_PINS, 3, onEncoderChanged);
Encoder encoder4 = Encoder(ENC_3_GPIO_A_PINS, ENC_3_GPIO_B_PINS, 4, onEncoderChanged);

Encoder* encoders[] = { &encoder1, &encoder2, &encoder3, &encoder4 };
Button* buttons[] = { &encoderButton1, &encoderButton2, &encoderButton3, &encoderButton4, &button1, &button2 };

Potentiometer pot1 = Potentiometer(POT_0_PIN, -32768, 32768);
Potentiometer pot2 = Potentiometer(POT_1_PIN, -32768, 32768);
Potentiometer pot3 = Potentiometer(POT_2_PIN, -32768, 32768);
Potentiometer pot4 = Potentiometer(POT_3_PIN, -32768, 32768);
Potentiometer pot5 = Potentiometer(POT_4_PIN, -32768, 32768);
Potentiometer pot6 = Potentiometer(POT_5_PIN, -32768, 32768);

void setup()
{
  Serial.begin(115200);
  
  SPI.begin();
  mcpInput.begin();
  mcpOutput.begin();

  for (uint8_t i = 0; i < 6; i++) {
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

void loop()
{
  uint16_t gpio = mcpInput.readGpio();
  
  bool isChanged = false;
  
  if (encoder1.process(gpio))
  {
    isChanged = true;
  }
 
  if (encoder2.process(gpio))
  {
    isChanged = true;
  }
  
  if (encoder3.process(gpio))
  {
    isChanged = true;
  }
  
  if (encoder4.process(gpio))
  {
    isChanged = true;
  }
  
  if (encoderButton1.checkState())
  {
    isChanged = true;
  }
  
  if (encoderButton2.checkState())
  {
    isChanged = true;
  }
  
  if (encoderButton3.checkState())
  {
    isChanged = true;
  }
  
  if (encoderButton4.checkState())
  {
    isChanged = true;
  }

  if (button1.checkState())
  {
    isChanged = true;
  }
  
  if (button2.checkState())
  {
    isChanged = true;
  }


  if (pot1.readAndGetValue())
  {
    isChanged = true;
  }
  
  if (pot2.readAndGetValue())
  {
    isChanged = true;
  }

  if (pot3.readAndGetValue())
  {
    isChanged = true;
  }

  if (pot4.readAndGetValue())
  {
    isChanged = true;
  }
  
  if (pot5.readAndGetValue())
  {
    isChanged = true;
  }

  if (isChanged)
  {
    sendGamepadReport();
  }

  if ((millis() / 1000) % 2 == 0)
  {
    mcpOutput.writePin(15, 1);  
  }
  else
  {
    mcpOutput.writePin(15, 0);    
  }
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
    | (encoder1.delta == 1) << 6
    | (encoder1.delta == -1) << 7);  

  Gamepad.analog1(pot1.mappedValue);
  Gamepad.analog2( (-1 * pot2.mappedValue) - 1);
  Gamepad.analog3(pot3.mappedValue);
  Gamepad.analog4(pot5.mappedValue);
  Gamepad.analog5(pot4.mappedValue);
  Gamepad.analog6(pot6.mappedValue);


  
  Gamepad.analog7(encoder1.value);
  Gamepad.analog8(encoder2.value);
  Gamepad.analog9(encoder3.value);
  Gamepad.analog10(encoder4.value);
  
  Gamepad.write();
}

void onEncoderChanged(int8_t change, uint8_t id, int value)
{ 
  /*Serial.print(id);
  Serial.print(":");
  Serial.print(change);
  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");*/
}

void onButtonPressed(Button sender)
{
  //Serial.println("Pressed: " + String(sender.pin));
}

void onExpanderInterrupt() {
  isExpanderInterrupted = true;  
}
