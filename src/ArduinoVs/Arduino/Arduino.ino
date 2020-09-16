#include "Settings.h"
#include "Mcp/McpSettings.h"
#include "Mcp/Mcp.h"
#include "Mcp/McpBuilder.h"
#include <HID-Project.h>
#include "Encoder.h"
#include "Display.h"
#include "SimController.h"

void onEncoderChanged(int8_t change, byte id, int value);
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

int potentiometers[] = { 0 };

Display displays[] =
{
  Display(DIS_CLK_PIN, DIS_0_0_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_0_1_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_1_0_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_1_1_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_2_0_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_2_1_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_3_0_DIO_PIN),
  Display(DIS_CLK_PIN, DIS_3_1_DIO_PIN),
};

byte numberOfDisplays = sizeof(displays) / sizeof(displays[0]);
const byte panicButtonPin = 4;

uint16_t expander2GpioValue = 0;
volatile bool isQueueAUnderWrite = true;

volatile byte interruptsCountA = 0;
volatile byte interruptsCountB = 0;



bool isWriting = false;

Encoder encoders[4];


void setup()
{
	Serial.begin(9600);
	while (!Serial);

	SPI.begin();
	mcpInput.begin();
	mcpOutput.begin();

	for (uint8_t i = 0; i < ENC_NUM; i++) {
		encoders[i] = Encoder(ENC_GPIO_PINS[i][0], ENC_GPIO_PINS[i][1], i, onEncoderChanged);
	}

	Serial.println("kickin");
	//Gamepad.begin();
}

void checkInterrupts()
{
	if (isExpanderInterrupted)
	{
		detachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN));
		// Serial.println("INTERRUPTED");

		uint16_t prevGpio = mcpInput.getLastKnownGpio();
		uint16_t gpio = mcpInput.readGpio();
		
		for (uint8_t i = 0; i < 16; i++)
		{
			bool prevPin = prevGpio & (1 << i);
			bool nextPin = gpio & (1 << i);

			if (prevPin != nextPin) {

			}
		}

		for (int j = 0; j < ENC_NUM; j++)
		{
			encoders[j].process(
				bitRead(gpio, encoders[j].pinA), 
				bitRead(gpio, encoders[j].pinB));
		}

		isExpanderInterrupted = false;
		attachInterrupt(digitalPinToInterrupt(MCP_INPUT_INTERRUPT_PIN), onExpanderInterrupt, FALLING);
	}
}

long c = 0;
void loop()
{
	
	if (((millis() / 1000) % 2 == 0))
	{
		mcpOutput.writePin(LED_4_GPIO_PIN, HIGH);
		mcpOutput.writePin(LED_5_GPIO_PIN, LOW);

	}
	else
	{
		mcpOutput.writePin(LED_4_GPIO_PIN, LOW);
		mcpOutput.writePin(LED_5_GPIO_PIN, HIGH);
	}

	checkInterrupts();

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

	//sendGamepadReport();
}

void sendGamepadReport()
{
	//Gamepad.xAxis( (analogRead(A5) * 64) - 32768);
	Gamepad.xAxis(100);
	Gamepad.write();
}

void writeBinary(uint16_t doubleByte) {
	for (byte i = 0; i < 16; i++)
	{
		Serial.print(bitRead(doubleByte, i));
	}
	//Serial.println();
}

void onEncoderChanged(int8_t change, uint8_t id, int value)
{
	displays[id * 2].showNumberDec(encoders[id].value, false, 4, 0);
	displays[id * 2 + 1].showNumberDec(encoders[id].value + 1, false, 4, 0);
	
	Serial.print(id);
	Serial.print(":");
	Serial.print(change);
	Serial.print(" (");
	Serial.print(value);
	Serial.println(")");
}

void onSimStateChanged(byte key, int value)
{
	displays[0].showNumberDec(value, false, 4, 0);
}

void onExpanderInterrupt()
{
	isExpanderInterrupted = true;
}
