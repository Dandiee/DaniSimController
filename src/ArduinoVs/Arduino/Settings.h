// MCP
const uint8_t MCP_INPUT_SS_PIN = 6;
const uint8_t MCP_OUTPUT_SS_PIN = 5;
const uint8_t MCP_INPUT_INTERRUPT_PIN = 7;

// ENCODERS
const uint8_t ENC_NUM = 4;
const uint8_t ENC_GPIO_PINS[ENC_NUM][2] =
{
	{7, 6},
	{5, 4},
	{3, 2},
	{1, 0}
};

const uint8_t ENC_BTN_GPIO_PINS[ENC_NUM] = { 8, 9, 10, 11 };

// DISPLAYS
const uint8_t DIS_CLK_PIN = 2;
const uint8_t DIS_0_0_DIO_PIN = 11;
const uint8_t DIS_0_1_DIO_PIN = 3;
const uint8_t DIS_1_0_DIO_PIN = 10;
const uint8_t DIS_1_1_DIO_PIN = 4;
const uint8_t DIS_2_0_DIO_PIN = 9;
const uint8_t DIS_2_1_DIO_PIN = 13;
const uint8_t DIS_3_0_DIO_PIN = 8;
const uint8_t DIS_3_1_DIO_PIN = 12;

// BUTTONS
const uint8_t BTN_GPIO_PINS[] = { 12, 13 };

// LEDS
const uint8_t LED_0_GPIO_PIN = 2;
const uint8_t LED_1_GPIO_PIN = 3;
const uint8_t LED_2_GPIO_PIN = 4;
const uint8_t LED_3_GPIO_PIN = 5;
const uint8_t LED_4_GPIO_PIN = 6;
const uint8_t LED_5_GPIO_PIN = 7;

// POT
const uint8_t POT_0_PIN = 0;
const uint8_t POT_1_PIN = 1;
const uint8_t POT_2_PIN = 2;
const uint8_t POT_3_PIN = 3;
const uint8_t POT_4_PIN = 4;