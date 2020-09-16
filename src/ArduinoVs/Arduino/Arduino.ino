#include "Potentiometer.h"
#include "Button.h"
#include "Settings.h"
#include "Mcp/McpSettings.h"
#include "Mcp/Mcp.h"
#include "Mcp/McpBuilder.h"
#include <HID-Project.h>
#include "Encoder.h"
#include "Display.h"
#include "SimController.h"

void onEncoderChanged(int8_t change, byte id, int value);
void onButtonPressed(Button sender);
void onSimStateChanged(byte key, int value);

SimController simController = SimController(onSimStateChanged);

Mcp mcpInput = McpBuilder(MCP_INPUT_SS_PIN)
.withPinDirections(0xFFFF)
.withPullUps(0xFFFF)
.withIoPolarity(0xFFFF)
.withInterrupts(0xFFFF)
.withInterruptPin(MCP_INPUT_INTERRUPT_PIN)
.withIconMirror(1)
.withIconSequentialOperation(1)
.withIconHardwareAddress(1).build();

Mcp mcpOutput = McpBuilder(MCP_OUTPUT_SS_PIN)
.withPinDirections(0x0000)
.withPullUps(0x0000)
.withIconMirror(0)
.withIconSequentialOperation(0)
.withIconHardwareAddress(0)
.build();

volatile bool isExpanderInterrupted = false;


Display display1 = Display(DIS_CLK_PIN, DIS_0_0_DIO_PIN);
Display display2 = Display(DIS_CLK_PIN, DIS_0_1_DIO_PIN);
Display display3 = Display(DIS_CLK_PIN, DIS_1_0_DIO_PIN);
Display display4 = Display(DIS_CLK_PIN, DIS_1_1_DIO_PIN);
Display display5 = Display(DIS_CLK_PIN, DIS_2_0_DIO_PIN);
Display display6 = Display(DIS_CLK_PIN, DIS_2_1_DIO_PIN);
Display display7 = Display(DIS_CLK_PIN, DIS_3_0_DIO_PIN);
Display display8 = Display(DIS_CLK_PIN, DIS_3_1_DIO_PIN);

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

Potentiometer pot1 = Potentiometer(POT_0_PIN, -128, 128);
Potentiometer pot2 = Potentiometer(POT_1_PIN, -32768, 32768);
Potentiometer pot3 = Potentiometer(POT_2_PIN, -32768, 32768);
Potentiometer pot4 = Potentiometer(POT_3_PIN, -32768, 32768);
Potentiometer pot5 = Potentiometer(POT_4_PIN, -32768, 32768);

void setup()
{
	Serial.begin(9600);
	while (!Serial);

	SPI.begin();
	mcpInput.begin();
	mcpOutput.begin();

	Serial.println("kickin");
	Gamepad.begin();
}

void checkInterrupts()
{
	if (isExpanderInterrupted)
	{
		detachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN));
		uint16_t gpio = mcpInput.readGpio();

		// Serial.println("INTERRUPT");

		encoder1.process(gpio);
		encoder2.process(gpio);
		encoder3.process(gpio);
		encoder4.process(gpio);

		encoderButton1.checkState(gpio);
		encoderButton2.checkState(gpio);
		encoderButton3.checkState(gpio);
		encoderButton4.checkState(gpio);

		button1.checkState(gpio);
		button2.checkState(gpio);

		isExpanderInterrupted = false;
		attachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN), onExpanderInterrupt, FALLING);
	}
}

void loop()
{
	if (((millis() / 1000) % 2 == 0))
	{
		mcpOutput.writePin(LED_1_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_3_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_5_GPIO_PIN, HIGH);

		mcpOutput.writePin(LED_0_GPIO_PIN, LOW);
		mcpOutput.writePin(LED_2_GPIO_PIN, LOW);
		mcpOutput.writePin(LED_4_GPIO_PIN, LOW);
	}
	else
	{
		mcpOutput.writePin(LED_1_GPIO_PIN, LOW);
		mcpOutput.writePin(LED_3_GPIO_PIN, LOW);
		mcpOutput.writePin(LED_5_GPIO_PIN, LOW);

		mcpOutput.writePin(LED_0_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_2_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_4_GPIO_PIN, HIGH);
	}

	
	checkInterrupts();
	sendGamepadReport();
	// simController.readSerial();
}

void sendGamepadReport()
{
	Gamepad.buttons(
		encoderButton1.lastKnownState
		| encoderButton2.lastKnownState << 1
		| encoderButton3.lastKnownState << 2
		| encoderButton4.lastKnownState << 3
		| button1.lastKnownState << 4
		| button2.lastKnownState << 5);

	Gamepad.xAxis(pot5.readAndGetValue()); // linear slider

	Gamepad.zAxis(pot1.readAndGetValue()); // -128 -> 128

	Gamepad.yAxis(pot2.readAndGetValue());
	Gamepad.rxAxis(pot3.readAndGetValue());
	Gamepad.ryAxis(pot4.readAndGetValue());

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

void onSimStateChanged(byte key, int value)
{
	
}

void onExpanderInterrupt()
{
	isExpanderInterrupted = true;
}

void onButtonPressed(Button sender)
{
	//Serial.println("Pressed: " + String(sender.pin));
}