#include "mcp8024.h"
#include "main.h"

void MCP8024_Init(MCP8024_t *mcp8024, UART_HandleTypeDef *com_uart, TIM_HandleTypeDef *h_mosfet_timer, TIM_HandleTypeDef *l_mosfet_timer) {

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	mcp8024->h_mosfet_timer = h_mosfet_timer;
	mcp8024->l_mosfet_timer = l_mosfet_timer;
	mcp8024->com_uart = com_uart;

	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_3);

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	// set configuration
	MCP8024_Config(mcp8024);
}

void MCP8024_Config(MCP8024_t *mcp8024) {
	MCP8024_Config_0_t config_0 = {0};
	MCP8024_Config_1_t config_1 = {0};
	MCP8024_Config_2_t config_2 = {0};

	config_0.disable_30k_pullup = MCP8024_DISCONNECTION_OF_30K_LEVEL_TRANSLATOR_PULLUP_WHEN_CE_0_DISABLE;
	config_0.undervoltage_lockout = MCP8024_UNDERVOLTAGE_LOCKOUT_ENABLED;
	config_0.ext_mosfet_overcurrent_detection = MCP8024_EXT_MOSFET_OVERCURRENT_DETECTION_ENABLED;
	config_0.ext_mosfet_overcurrent_limit = MCP8024_EXT_MOSFET_OVERCURRENT_LIMIT_0_250_V;

	config_1.dac_current_reference = MCP8024_GET_DAC_CURRENT_REFERENCE_VALUE(1.88f);

	config_2.driver_dead_time = MCP8024_DRIVER_DEAD_TIME_2_US;
	config_2.driver_blanking_time = MCP8024_DRIVER_BLANKING_TIME_4_US;

	MCP8024_SetConfig(mcp8024, config_0, config_1, config_2);
}

void MCP8024_GetStatus(MCP8024_t *mcp8024) {
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_0, NULL, NULL, &mcp8024->registers.raw.status_0);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_1, NULL, NULL, &mcp8024->registers.raw.status_1);
}

void MCP8024_GetConfig(MCP8024_t *mcp8024) {
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_0, NULL, NULL, &mcp8024->registers.raw.config_0);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_1, NULL, NULL, &mcp8024->registers.raw.config_1);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_2, NULL, NULL, &mcp8024->registers.raw.config_2);
}

void MCP8024_SetConfig(MCP8024_t *mcp8024, MCP8024_Config_0_t config_0, MCP8024_Config_1_t config_1, MCP8024_Config_2_t config_2) {
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_SET_CFG_0, (uint8_t *)&config_0, NULL, NULL);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_SET_CFG_1, (uint8_t *)&config_1, NULL, NULL);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_SET_CFG_2, (uint8_t *)&config_2, NULL, NULL);
}

void MCP8024_RxCpltCallback(MCP8024_t *mcp8024) {
	mcp8024->com_flag = 1;
}

void MCP8024_WriteCommand(MCP8024_t *mcp8024, MCP8024_Command_t cmd, uint8_t *argument, uint8_t *response_cmd, uint8_t *response_data) {
	mcp8024->com_flag = 0;
	HAL_UART_AbortReceive_IT(mcp8024->com_uart);

	if(!argument) {
		struct {
			uint8_t cmd_echo;
			MCP8024_Message_t response;
		} rx_buffer = {0};

		HAL_UART_Receive_IT(mcp8024->com_uart, (uint8_t *)&rx_buffer, 3);
		HAL_UART_Transmit(mcp8024->com_uart, &cmd, 1, HAL_MAX_DELAY);
		while(!mcp8024->com_flag) {}

		if(response_cmd)
			*response_cmd = rx_buffer.response.cmd;
		if(response_data)
			*response_data = rx_buffer.response.data;

		return;
	}

	struct {
		MCP8024_Message_t echo;
		MCP8024_Message_t response;
	} rx_buffer = {0};

	MCP8024_Message_t tx_buffer = {cmd, *argument};

	HAL_UART_Receive_IT(mcp8024->com_uart, (uint8_t *)&rx_buffer, 4);
	HAL_UART_Transmit(mcp8024->com_uart, (uint8_t *)&tx_buffer, 2, HAL_MAX_DELAY);
	while(!mcp8024->com_flag) {}

	if(response_cmd)
		*response_cmd = rx_buffer.response.cmd;
	if(response_data)
		*response_data = rx_buffer.response.data;
}


void MCP8024_SetFill(MCP8024_t *mcp8024, uint16_t u_l, uint16_t v_l, uint16_t w_l, uint16_t u_h, uint16_t v_h, uint16_t w_h) {

	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_1, u_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_2, v_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_3, w_h);

	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_1, u_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_2, v_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_3, w_l);
}


