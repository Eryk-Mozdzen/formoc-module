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
}

void MCP8024_ReadAllRegisters(MCP8024_t *mcp8024) {
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_0, NULL, NULL, &mcp8024->registers.status_0);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_1, NULL, NULL, &mcp8024->registers.status_1);

	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_0, NULL, NULL, &mcp8024->registers.config_0);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_1, NULL, NULL, &mcp8024->registers.config_1);
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_GET_CFG_2, NULL, NULL, &mcp8024->registers.config_2);
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


