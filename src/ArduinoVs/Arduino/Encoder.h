#ifndef SRC_DANIENCODER_H_
#define SRC_DANIENCODER_H_

#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

#define R_START      0b0000
#define R_CW_FINAL   0b0001
#define R_CW_BEGIN   0b0010
#define R_CW_NEXT    0b0011
#define R_CCW_BEGIN  0b0100
#define R_CCW_FINAL  0b0101
#define R_CCW_NEXT   0b0110

const byte stateTable[][4] =
{
	// 00         01           10           11
	{R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},           // R_START 
	{R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},  // R_CW_FINAL
	{R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},           // R_CW_BEGIN
	{R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},           // R_CW_NEXT
	{R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},           // R_CCW_BEGIN
	{R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW}, // R_CCW_FINAL
	{R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START}            // R_CCW_NEXT
};

typedef void (*encoderCallback)(int8_t change, uint8_t id, int value);
class Encoder
{
public:
	Encoder() { }
	Encoder(uint8_t pinA, uint8_t pinB, uint8_t id, encoderCallback callback = nullptr)
		: pinA(pinA), pinB(pinB), id(id), callback(callback), state(state), value(value) { }

	uint8_t process(uint16_t gpio)
	{
		uint8_t a = bitRead(gpio, pinA);
		uint8_t b = bitRead(gpio, pinB);

		if (a != stateA || b != stateB)
		{
			stateA = a;
			stateB = b;

			uint8_t pinState = (a << 1) | b;
			state = stateTable[state & 0b00001111][pinState];
			uint8_t result = (state & 0b00110000);

			if (result)
			{
				int8_t change = result == DIR_CW ? -1 : 1;
				value += change;
				if (callback)
				{
					callback(change, id, value);
				}
			}

			return result;
		}

		return 0;
	}

	int value = 0;
	uint8_t pinA = 0;
	uint8_t pinB = 0;
private:

	uint8_t id = 0;
	encoderCallback callback = nullptr;
	uint8_t state = 0;
	uint8_t stateA = 1;
	uint8_t stateB = 1;

};

#endif
