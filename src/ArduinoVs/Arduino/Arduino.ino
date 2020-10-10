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

struct State {
  uint8_t IsAutopilotMasterEnabled = 0;
  uint8_t IsAutopilotHeadingEnabled = 0;
  uint8_t IsAutopilotAltitudeEnabled = 0;
  uint8_t IsAutopilotAirspeedEnabled = 0;
  uint8_t IsAutopilotVerticalSpeedEnabled = 0;
  
  uint8_t IsLeftGearMoving = 0;
  uint8_t IsCenterGearMoving = 0;
  uint8_t IsRightGearMoving = 0;
  
  uint8_t IsLeftGearOut = 0;
  uint8_t IsCenterGearOut = 0;
  uint8_t IsRightGearOut = 0;

  uint8_t IsFlapNonZero = 0;
  uint8_t IsBrakeNonZero = 0;

  uint8_t IsParkingBrakeEnabled = 0;
};

//SimController simController = SimController(onSimStateChanged);

State _state = State();
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


volatile bool isExpanderInterrupted = false;

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
Button* buttons[] = { &encoderButton1, &encoderButton2, &encoderButton3, &encoderButton4, 
&button1, &button2, &button3, &button4, &button5, &button6, &button7, &button8};

Potentiometer* pots[] = { &pot1, &pot2, &pot3, &pot4, &pot5 };

uint8_t encodersCount = (sizeof(encoders)/sizeof(*encoders));
uint8_t buttonsCount = (sizeof(buttons)/sizeof(*buttons));
uint8_t potsCount = (sizeof(pots)/sizeof(*pots));

void setup()
{
  
  
  Serial.begin(115200);


  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  

  SPI.begin();

  mcpInput.begin();
  mcpOutput.begin();

  for (uint8_t i = 0; i < buttonsCount; i++) {
      buttons[i]->setup();
  }

  Gamepad.begin();
}

long lastLight = 0;
uint8_t isLighting = false;

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
  writeLeds();
}

void ledTest() {
  long m = (millis() / 50) % 16;  uint16_t lofasz = 0x0000;
  bitWrite(lofasz, m, 1);
  mcpOutput.writeGpio(lofasz);
}

uint8_t buffer[16];
uint64_t _prevRawState = 0;
uint8_t myBool = false;
uint8_t cursor = 0;


void lightUp() {
  isLighting = true;
  lastLight = millis();
}



uint8_t prevByte1;
uint8_t prevByte2;
uint8_t prevByte3;
uint8_t prevByte4;

void readFromPc()
{
    
    int availableBytes = Serial.available();
    if (availableBytes > 0)
    {
      if (availableBytes != 4)
      {        
        while(Serial.available()) Serial.read();
      }
      else {
        Serial.readBytes(buffer, 4);

        uint8_t byte1 = buffer[0];
        uint8_t byte2 = buffer[1];
        uint8_t byte3 = buffer[2];
        uint8_t byte4 = buffer[3];
     
        _state.IsAutopilotMasterEnabled = bitRead(byte2, 0);
        _state.IsAutopilotHeadingEnabled = bitRead(byte2, 1);
        _state.IsAutopilotAltitudeEnabled = bitRead(byte2, 2);
        _state.IsAutopilotAirspeedEnabled = bitRead(byte2, 3);
        _state.IsAutopilotVerticalSpeedEnabled = bitRead(byte2, 4);

        _state.IsLeftGearMoving = bitRead(byte3, 0);
        _state.IsCenterGearMoving = bitRead(byte3, 1);
        _state.IsRightGearMoving = bitRead(byte3, 2);

        _state.IsLeftGearOut = bitRead(byte3, 3);
        _state.IsCenterGearOut = bitRead(byte3, 4);
        _state.IsRightGearOut = bitRead(byte3, 5);
        
        _state.IsFlapNonZero = bitRead(byte3, 6);
        _state.IsBrakeNonZero = bitRead(byte3, 7);

        _state.IsParkingBrakeEnabled = bitRead(byte4, 0);
      }
    }
}

void writeLeds() {
  uint16_t gpio = 0;

  long m = millis();
  long blinkState = (m / 125) % 2 == 0;

  bitWrite(gpio, 7, _state.IsBrakeNonZero);
  bitWrite(gpio, 8, _state.IsFlapNonZero);
  
  bitWrite(gpio, 9, _state.IsAutopilotMasterEnabled);
  bitWrite(gpio, 10, _state.IsAutopilotHeadingEnabled);
  bitWrite(gpio, 11, _state.IsAutopilotAltitudeEnabled);
  bitWrite(gpio, 12, _state.IsAutopilotAirspeedEnabled);
  bitWrite(gpio, 13, _state.IsAutopilotVerticalSpeedEnabled);


  bitWrite(gpio, 0, _state.IsParkingBrakeEnabled);


  digitalWrite(8, _state.IsLeftGearMoving ? blinkState : _state.IsLeftGearOut);
  digitalWrite(9, _state.IsCenterGearMoving ? blinkState : _state.IsCenterGearOut);
  digitalWrite(10, _state.IsRightGearMoving ? blinkState : _state.IsRightGearOut);

/*  digitalWrite(8, _state.IsLeftGearOut);
  digitalWrite(9, _state.IsCenterGearOut);
  digitalWrite(10, _state.IsRightGearOut);*/

  mcpOutput.writeGpio(gpio);    
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

void onEncoderChanged(int8_t change, uint8_t id, int value)
{ 
  return;
  Serial.print(id);
  Serial.print(":");
  Serial.print(change);
  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");
}

void onButtonPressed(Button sender)
{
  Serial.println("Pressed: " + String(sender.pin));
}

void onExpanderInterrupt() {
  isExpanderInterrupted = true;  
}
