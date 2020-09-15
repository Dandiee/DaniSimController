#include <SPI.h>
#ifndef _MCP_h
#define _MCP_h

const byte IODIRA = 0x00;   // IO direction  (0 = output, 1 = input (Default))
const byte IODIRB = 0x01;
const byte IOPOLA = 0x02;   // IO polarity   (0 = normal, 1 = inverse)
const byte IOPOLB = 0x03;
const byte GPINTENA = 0x04;   // Interrupt on change (0 = disable, 1 = enable)
const byte GPINTENB = 0x05;
const byte DEFVALA = 0x06;   // Default comparison for interrupt on change (interrupts on opposite)
const byte DEFVALB = 0x07;
const byte INTCONA = 0x08;   // Interrupt control (0 = interrupt on change from previous, 1 = interrupt on change from DEFVAL)
const byte INTCONB = 0x09;
const byte IOCON = 0x0A;   // IO Configuration: bank/mirror/seqop/disslw/haen/odr/intpol/notimp
const byte GPPUA = 0x0C;   // Pull-up resistor (0 = disabled, 1 = enabled)
const byte GPPUB = 0x0D;
const byte INFTFA = 0x0E;   // Interrupt flag (read only) : (0 = no interrupt, 1 = pin caused interrupt)
const byte INFTFB = 0x0F;
const byte INTCAPA = 0x10;   // Interrupt capture (read only) : value of GPIO at time of last interrupt
const byte INTCAPB = 0x11;
const byte GPIOA = 0x12;   // Port value. Write to change, read to obtain value
const byte GPIOB = 0x13;
const byte OLLATA = 0x14;   // Output latch. Write to latch output.
const byte OLLATB = 0x15;

const byte EXP = 0x20;

class Mcp
{
public:

	Mcp(McpSettings settings)
		: _settings(settings)
	{
		_slaveSelectorPin = settings.SlaveSelectorPin;

		pinMode(settings.SlaveSelectorPin, OUTPUT);
		digitalWrite(settings.SlaveSelectorPin, HIGH);
		SPI.begin();

		writeByte(IOCON, settings.Iocon);

		writeWord(IODIRA, settings.InterruptPin);
		writeWord(GPPUA, settings.PullUps);
		writeWord(IOPOLA, settings.IoPolarities);
		writeWord(GPINTENA, settings.InterruptPin);
		writeWord(INTCONA, settings.InterruptControlModes);
		writeWord(DEFVALA, settings.InterruptDefaultValue);

		if (settings.UseInterrupts && settings.InterruptPin)
		{
			pinMode(settings.InterruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(settings.InterruptPin), ::onExpanderInterrupt, FALLING);

			readByte(INTCAPA);
			readByte(INTCAPB);
		}
	}

	uint16_t readGpio() {
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer((EXP << 1) | 1);
		SPI.transfer(GPIOA);
		uint16_t value = SPI.transfer(0x00);
		value |= (SPI.transfer(0x00) << 8);
		digitalWrite(_slaveSelectorPin, HIGH);
		return value;
	}

private:
	McpSettings _settings;
	uint8_t _slaveSelectorPin;

	void writeByte(uint8_t registerAddress, uint8_t value) {
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer(EXP << 1);
		SPI.transfer(registerAddress);
		SPI.transfer(value);
		digitalWrite(_slaveSelectorPin, HIGH);
	}

	void writeWord(uint8_t registerAddress, uint16_t word) {
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer(EXP << 1);
		SPI.transfer(registerAddress);
		SPI.transfer((uint8_t)(word));
		SPI.transfer((uint8_t)(word >> 8));
		digitalWrite(_slaveSelectorPin, HIGH);
	}

	uint8_t readByte(const uint8_t registerAddress)
	{
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer((EXP << 1) | 1);
		SPI.transfer(registerAddress);
		uint8_t data = SPI.transfer(0);
		digitalWrite(_slaveSelectorPin, HIGH);
		return data;
	}
};
#endif