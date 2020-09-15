#ifndef _MCPSETTINGS_h
#define _MCPSETTINGS_h

class McpSettings
{
public:

	McpSettings::McpSettings (uint8_t slaveSelectorPin) 
		: SlaveSelectorPin(slaveSelectorPin) { }

	// shared settings
	uint8_t SlaveSelectorPin = 0;
	uint8_t Iocon = 0;

	// Per bank states
	uint16_t IoDirections = 0;
	uint16_t PullUps = 0;
	uint16_t IoPolarities = 0;
	uint16_t UseInterrupts = 0;
	uint16_t InterruptControlModes = 0;
	uint16_t InterruptDefaultValue = 0;

	uint16_t InterruptPin = 0;
};
#endif

