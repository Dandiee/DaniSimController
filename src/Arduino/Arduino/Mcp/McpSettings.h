#ifndef _MCPSETTINGS_h
#define _MCPSETTINGS_h

class McpSettings
{
public:

	McpSettings (uint8_t slaveSelectorPin) 
		: SlaveSelectorPin(slaveSelectorPin) { }

	// shared settings
	uint8_t SlaveSelectorPin = 0;
	uint8_t Iocon = 0;

	// Per bank states
	uint16_t IoDirections = 0x0000;
	uint16_t PullUps = 0x0000;
	uint16_t IoPolarities = 0x0000;
	uint16_t UseInterrupts = 0x0000;
	uint16_t InterruptControlModes = 0x0000;
	uint16_t InterruptDefaultValue = 0x0000;

	uint16_t InterruptPin = 0;
};
#endif

