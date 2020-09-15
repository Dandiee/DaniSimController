#include "McpSettings.h"

#ifndef _MCPBUILDER_h
#define _MCPBUILDER_h

class McpBuilder
{
public:

	McpBuilder::McpBuilder(uint8_t slaveSelectorPin)
		: _settings(slaveSelectorPin) { }

	#pragma region ICON settings builders
	/// <summary>
	/// Controls how the registers are addressed
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = The registers are in the same bank (addresses are sequential).
	/// 1 = The registers associated with each port are separated into different banks.
	/// </param>
	/// <returns></returns>
	McpBuilder withIconBank(bool isEnabled) {
		return setSettingsFlag(7, isEnabled);
	}

	/// <summary>
	/// INT Pins Mirror bit
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with
	/// 1 = The INT pins are internally connected
	/// </param>
	/// <returns></returns>
	McpBuilder withIconMirror(bool isEnabled) {
		return setSettingsFlag(6, isEnabled);
	}


	/// <summary>
	/// Sequential Operation mode bit
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Sequential operation enabled, address pointer increments.
	/// 1 = Sequential operation disabled, address pointer does not increment.
	/// </param>
	/// <returns></returns>
	McpBuilder withIconSequentialOperation(bool isEnabled) {
		return setSettingsFlag(5, isEnabled);
	}

	/// <summary>
	/// Slew Rate control bit for SDA output
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Slew rate enabled
	/// 1 = Slew rate disabled
	/// </param>
	/// <returns></returns>
	McpBuilder withIconSlowRate(bool isEnabled) {
		return setSettingsFlag(4, isEnabled);
	}

	/// <summary>
	/// Hardware Address Enable bit
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Disables the MCP23S17 address pins
	/// 1 = Enables the MCP23S17 address pins
	/// </param>
	/// <returns></returns>
	McpBuilder withIconHardwareAddress(bool isEnabled) {
		return setSettingsFlag(3, isEnabled);
	}

	/// <summary>
	/// Configures the INT pin as an open-drain output
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Active driver output (INTPOL bit sets the polarity.)
	/// 1 = Open-drain output (overrides the INTPOL bit.)
	/// </param>
	/// <returns></returns>
	McpBuilder withIconOpenDrain(bool isEnabled) {
		return setSettingsFlag(2, isEnabled);
	}

	/// <summary>
	/// This bit sets the polarity of the INT output pin
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Active-low
	/// 1 = Active-high
	/// </param>
	/// <returns></returns>
	McpBuilder withIconInterruptPolarity(bool isEnabled) {
		return setSettingsFlag(1, isEnabled);
	}
	#pragma endregion

	#pragma region Port settings builders
	/// <summary>
	/// Pin direction
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Output
	/// 1 = Input
	/// </param>
	/// <returns></returns>
	McpBuilder withPinDirections(uint16_t bitmask) {
		return setBothBanks(&_settings.IoDirections, bitmask);
	}


	/// <summary>
	/// Pull-up resistor enabled
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Disabled
	/// 1 = Enabled
	/// </param>
	/// <returns></returns>
	McpBuilder withPullUps(uint16_t bitmask) {
		return setBothBanks(&_settings.PullUps, bitmask);
	}

	/// <summary>
	/// IO polarity
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Normal
	/// 1 = INversed
	/// </param>
	/// <returns></returns>
	McpBuilder withIoPolarity(uint16_t bitmask) {
		return setBothBanks(&_settings.IoPolarities, bitmask);
	}

	/// <summary>
	/// Interrupts enabled
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = Disabled
	/// 1 = Enabled
	/// </param>
	/// <returns></returns>
	McpBuilder withInterrupts(uint16_t bitmask) {
		return setBothBanks(&_settings.UseInterrupts, bitmask);
	}

	/// <summary>
	/// Interrupt control mode
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = OnChange
	/// 1 = ChangeFrom: Interrupt Default Value
	/// </param>
	/// <returns></returns>
	McpBuilder withInterruptControlMode(uint16_t bitmask) {
		return setBothBanks(&_settings.InterruptControlModes, bitmask);
	}

	/// <summary>
	/// Default intertupt value
	/// </summary>
	/// <param name="isEnabled">
	/// 0 = LOW
	/// 1 = HIGH
	/// </param>
	/// <returns></returns>
	McpBuilder withDefaultInterruptValue(uint16_t bitmask) {
		return setBothBanks(&_settings.InterruptDefaultValue, bitmask);
	}
	#pragma endregion

	/// <summary>
	/// Interruptor pin on the arduino board
	/// </summary>
	McpBuilder withInterruptPin(uint8_t pin) {
		return setBothBanks(&_settings.InterruptPin, pin);
	}

	Mcp build() {
		return Mcp(_settings);
	}

private:

	McpBuilder setSettingsFlag(uint8_t bit, bool value) {
		bitWrite(_settings.Iocon, bit, value);
		return *this;
	}

	McpBuilder setBothBanks(uint16_t *stateField, uint16_t bitMask) {
		*stateField = bitMask;
		return *this;
	}

	McpSettings _settings;
};
#endif

