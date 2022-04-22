#include "mcp8024.h"

static void __MCP8024_WriteRead(MCP8024_t *, uint8_t, uint8_t *);

void MCP8024_Init(MCP8024_t *mcp8024,
		GPIO_TypeDef *ce_port, uint16_t ce_pin,
		UART_HandleTypeDef *com_uart,
		TIM_HandleTypeDef *mosfet_h_timer, TIM_HandleTypeDef *mosfet_l_timer) {

	mcp8024->ce_port = ce_port;
	mcp8024->ce_pin = ce_pin;
	mcp8024->mosfet_h_timer = mosfet_h_timer;
	mcp8024->mosfet_l_timer = mosfet_l_timer;
	mcp8024->com_uart = com_uart;

	HAL_GPIO_WritePin(mcp8024->ce_port, mcp8024->ce_pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	HAL_TIM_PWM_Start(mcp8024->mosfet_h_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->mosfet_h_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->mosfet_h_timer, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(mcp8024->mosfet_l_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->mosfet_l_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->mosfet_l_timer, TIM_CHANNEL_3);

	MCP8024_SetCompare(mcp8024, 0, 0, 0, 0, 0, 0);

	__HAL_TIM_SET_COUNTER(mcp8024->mosfet_l_timer, 0);
	__HAL_TIM_SET_COUNTER(mcp8024->mosfet_h_timer, 0);

	HAL_GPIO_WritePin(mcp8024->ce_port, mcp8024->ce_pin, GPIO_PIN_SET);
	HAL_Delay(100);

	// set configuration
	MCP8024_Config(mcp8024);
}

void MCP8024_Config(MCP8024_t *mcp8024) {
	MCP8024_Config_0_t config_0 = {0};
	MCP8024_Config_1_t config_1 = {0};
	MCP8024_Config_2_t config_2 = {0};

	config_0.disable_30k_pullup = 				MCP8024_CONFIG_DISCONNECTION_OF_30K_LEVEL_TRANSLATOR_PULLUP_WHEN_CE_0_DISABLE;
	config_0.undervoltage_lockout = 			MCP8024_CONFIG_UNDERVOLTAGE_LOCKOUT_DISABLED;
	config_0.ext_mosfet_overcurrent_detection = MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_DETECTION_DISABLED;
	config_0.ext_mosfet_overcurrent_limit = 	MCP8024_CONFIG_EXT_MOSFET_OVERCURRENT_LIMIT_0_250_V;

	config_1.dac_current_reference = 			MCP8024_GET_DAC_CURRENT_REFERENCE_VALUE(1.88f);

	config_2.driver_dead_time = 				MCP8024_CONFIG_DRIVER_DEAD_TIME_2_US;
	config_2.driver_blanking_time = 			MCP8024_CONFIG_DRIVER_BLANKING_TIME_4_US;

	MCP8024_GetConfig(mcp8024);
	MCP8024_SetConfig(mcp8024, config_0, config_1, config_2);
	MCP8024_GetConfig(mcp8024);
}

void MCP8024_GetStatus(MCP8024_t *mcp8024) {
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_STATUS_0, NULL);
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_STATUS_1, NULL);
}

void MCP8024_GetConfig(MCP8024_t *mcp8024) {
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_GET_CFG_0, NULL);
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_GET_CFG_1, NULL);
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_GET_CFG_2, NULL);
}

void MCP8024_SetConfig(MCP8024_t *mcp8024, MCP8024_Config_0_t config_0, MCP8024_Config_1_t config_1, MCP8024_Config_2_t config_2) {
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_SET_CFG_0, (uint8_t *)&config_0);
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_SET_CFG_1, (uint8_t *)&config_1);
	__MCP8024_WriteRead(mcp8024, MCP8024_CMD_SET_CFG_2, (uint8_t *)&config_2);
}

void MCP8024_RxCpltCallback(MCP8024_t *mcp8024) {
	mcp8024->com_rx_ready = 1;

	switch(mcp8024->com_rx_buffer[2]) {
		case 0x05:
		case 0x45:
		case 0x85:	mcp8024->registers.raw.status_0 = mcp8024->com_rx_buffer[3];	break;

		case 0x06:
		case 0x46:
		case 0x86:	mcp8024->registers.raw.status_1 = mcp8024->com_rx_buffer[3];	break;

		case 0x01:
		case 0x02:
		case 0x41:
		case 0x42:	mcp8024->registers.raw.config_0 = mcp8024->com_rx_buffer[3];	break;

		case 0x03:
		case 0x04:
		case 0x43:
		case 0x44:	mcp8024->registers.raw.config_1 = mcp8024->com_rx_buffer[3];	break;

		case 0x07:
		case 0x08:
		case 0x47:
		case 0x48:	mcp8024->registers.raw.config_2 = mcp8024->com_rx_buffer[3];	break;
	}
}

void __MCP8024_WriteRead(MCP8024_t *mcp8024, uint8_t cmd, uint8_t *argument) {
	mcp8024->com_rx_ready = 0;

	uint8_t tx_buffer[2] = {cmd, argument ? *argument : 0};
	uint8_t *rx_buffer = argument ? mcp8024->com_rx_buffer : &mcp8024->com_rx_buffer[1];

	HAL_UART_Receive_DMA(mcp8024->com_uart, rx_buffer, argument ? 4 : 3);
	HAL_UART_Transmit_DMA(mcp8024->com_uart, tx_buffer, argument ? 2 : 1);

	while(!mcp8024->com_rx_ready);
}



void MCP8024_SetCompare(MCP8024_t *mcp8024, uint16_t fill_l_x, uint16_t fill_l_y, uint16_t fill_l_z, uint16_t fill_h_x, uint16_t fill_h_y, uint16_t fill_h_z) {
	const int16_t pwm_min = MCP8024_PWM_MAX_FILL*0.1f;
	const int16_t pwm_max = MCP8024_PWM_MAX_FILL*0.9f;

	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_h_timer, TIM_CHANNEL_1, MIN(MAX(fill_h_x, pwm_min), pwm_max));
	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_h_timer, TIM_CHANNEL_2, MIN(MAX(fill_h_y, pwm_min), pwm_max));
	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_h_timer, TIM_CHANNEL_3, MIN(MAX(fill_h_z, pwm_min), pwm_max));

	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_l_timer, TIM_CHANNEL_1, MIN(MAX(fill_l_x, pwm_min), pwm_max));
	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_l_timer, TIM_CHANNEL_2, MIN(MAX(fill_l_y, pwm_min), pwm_max));
	__HAL_TIM_SET_COMPARE(mcp8024->mosfet_l_timer, TIM_CHANNEL_3, MIN(MAX(fill_l_z, pwm_min), pwm_max));
}

void MCP8024_SetFill(MCP8024_t *mcp8024, float32x3_t fill) {
	MCP8024_SetCompare(mcp8024,
		fill.x*MCP8024_PWM_MAX_FILL,
		fill.y*MCP8024_PWM_MAX_FILL,
		fill.z*MCP8024_PWM_MAX_FILL,
		fill.x*MCP8024_PWM_MAX_FILL,
		fill.y*MCP8024_PWM_MAX_FILL,
		fill.z*MCP8024_PWM_MAX_FILL
	);
}


