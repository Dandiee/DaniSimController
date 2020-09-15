void onExpanderInterrupt();

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
		_slaveSelectorPin = _settings.SlaveSelectorPin;

		pinMode(_slaveSelectorPin, OUTPUT);
		digitalWrite(_slaveSelectorPin, HIGH);
	}

	void begin() {

		writeByte(IOCON, _settings.Iocon);

		writeByte(IODIRA, _settings.IoDirections);
		writeByte(GPPUA, _settings.PullUps);
		writeByte(IOPOLA, _settings.IoPolarities);
		writeByte(GPINTENA, _settings.UseInterrupts);
		writeByte(INTCONA, _settings.InterruptControlModes);
		writeByte(DEFVALA, _settings.InterruptDefaultValue);

		writeByte(IODIRB, _settings.IoDirections >> 8);
		writeByte(GPPUB, _settings.PullUps >> 8);
		writeByte(IOPOLB, _settings.IoPolarities >> 8);
		writeByte(GPINTENB, _settings.UseInterrupts >> 8);
		writeByte(INTCONB, _settings.InterruptControlModes >> 8);
		writeByte(DEFVALB, _settings.InterruptDefaultValue >> 8);

		if (_settings.UseInterrupts && _settings.InterruptPin)
		{
			pinMode(_settings.InterruptPin, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(_settings.InterruptPin), ::onExpanderInterrupt, FALLING);

			readByte(INTCAPA);
			readByte(INTCAPB);
		}

		printSettings();
	}

	void printSettings() {
		
		Serial.println();
		Serial.println("IoDirections:" + String(_settings.IoDirections));
		Serial.println("PullUps:" + String(_settings.PullUps));
		Serial.println("IoPolarities:" + String(_settings.IoPolarities));
		Serial.println("UseInterrupts:" + String(_settings.UseInterrupts));
		Serial.println("InterruptControlModes:" + String(_settings.InterruptControlModes));
		Serial.println("InterruptDefaultValue:" + String(_settings.InterruptDefaultValue));
		Serial.println("InterruptPin:" + String(_settings.InterruptPin));
		Serial.println("SlaveSelector:" + String(_slaveSelectorPin));
		Serial.println("Iocon:" + String(_settings.Iocon));
		
	}

	uint16_t readGpio() {
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer((EXP << 1) | 1);
		SPI.transfer(GPIOA);
		uint16_t value = SPI.transfer(0x00);
		value |= (SPI.transfer(0x00) << 8);
		digitalWrite(_slaveSelectorPin, HIGH);
		_lastKnownGpio = value; // TODO: maybe we dont need that value declared
		return value;
	}

	void writePin(uint8_t pin, bool value) {
		uint16_t word = 0;
		bitWrite(word, pin, value);
		writeGpio(word);
	}

private:
	McpSettings _settings;
	uint8_t _slaveSelectorPin;
	uint16_t _lastKnownGpio;

	void writeGpio(uint16_t value) {
		writeWord(GPIOA, value);
		_lastKnownGpio = value;
	}

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

	uint8_t readByte(const uint8_t registerAddress) {
		digitalWrite(_slaveSelectorPin, LOW);
		SPI.transfer((EXP << 1) | 1);
		SPI.transfer(registerAddress);
		uint8_t data = SPI.transfer(0);
		digitalWrite(_slaveSelectorPin, HIGH);
		return data;
	}
};
#endif