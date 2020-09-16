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
.withIconMirror(1)
.withIconSequentialOperation(1)
.withIconHardwareAddress(1)
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
	//Gamepad.begin();
}

void checkInterrupts()
{
	if (isExpanderInterrupted)
	{
		detachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN));
		uint16_t gpio = mcpInput.readGpio();

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

		// mcpOutput.writePin(LED_0_GPIO_PIN, LOW);
		// mcpOutput.writePin(LED_2_GPIO_PIN, LOW);
		// mcpOutput.writePin(LED_4_GPIO_PIN, LOW);
	}
	else
	{
		//mcpOutput.writePin(LED_1_GPIO_PIN, LOW);
		//mcpOutput.writePin(LED_3_GPIO_PIN, LOW);
		//mcpOutput.writePin(LED_5_GPIO_PIN, LOW);

		mcpOutput.writePin(LED_0_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_2_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_4_GPIO_PIN, HIGH);
	}

	checkInterrupts();

	sendGamepadReport();

	/*checkInterrupts();

	if (digitalRead(panicButtonPin) == LOW)
	{
	  Serial.println("Debug pressed, please wait...");
	  uint16_t gpioState = expander.readAndReset();
	  writeBinary(gpioState);
	  delay(500);
	  Serial.println("Okay, good to go");
	}*/

	/*uint16_t exp2Io = expander2.readAndReset();
	if (expander2GpioValue != exp2Io)
	{
	  writeBinary(exp2Io);
	  expander2GpioValue = exp2Io;
	}*/

	//simController.readSerial();


}

void sendGamepadReport()
{
	//display1.showNumberDec(pot1.readAndGetValue(), 0, 4, 0);
	display1.showNumberDec(pot1.readAndGetValue(), 0, 4, 0);
	display3.showNumberDec(pot2.readAndGetValue(), 0, 4, 0);
	display5.showNumberDec(pot3.readAndGetValue(), 0, 4, 0);
	display7.showNumberDec(pot4.readAndGetValue(), 0, 4, 0);

	display2.showNumberDec(pot5.readAndGetValue(), 0, 4, 0);
	//Serial.print((analogRead(POT_1_PIN) * 64) - 32768); Serial.print("/");
	//Serial.print((analogRead(POT_2_PIN) * 64) - 32768); Serial.print("/");
	//Serial.print((analogRead(POT_3_PIN) * 64) - 32768); Serial.print("/");
	//Serial.print((analogRead(POT_4_PIN) * 64) - 32768);
	/*
	Gamepad.xAxis((analogRead(POT_0_PIN) * 64) - 32768);
	Gamepad.yAxis((analogRead(POT_1_PIN) * 64) - 32768);
	Gamepad.zAxis((analogRead(POT_2_PIN) * 64) - 32768);
	Gamepad.rxAxis((analogRead(POT_3_PIN) * 64) - 32768);
	Gamepad.ryAxis((analogRead(POT_4_PIN) * 64) - 32768);*/

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

void onSimStateChanged(byte key, int value)
{
	
}

void onExpanderInterrupt()
{
	isExpanderInterrupted = true;
}

void onButtonPressed(Button sender)
{
	
}