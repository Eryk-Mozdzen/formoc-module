#include "mcp8024.h"
#include "main.h"

void MCP8024_Init(MCP8024_t *mcp8024, UART_HandleTypeDef *com_uart, TIM_HandleTypeDef *h_mosfet_timer, TIM_HandleTypeDef *l_mosfet_timer) {

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);

	mcp8024->h_mosfet_timer = h_mosfet_timer;
	mcp8024->l_mosfet_timer = l_mosfet_timer;
	mcp8024->com_uart = com_uart;
	mcp8024->com_buffer_size = 0;
	memset(mcp8024->com_buffer, 0, MCP8024_BUFFER_SIZE);

	//HAL_UART_Receive_IT(mcp8024->com_uart, mcp8024->com_buffer, 1);

	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_3);

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

void MCP8024_ReadAll(MCP8024_t *mcp8024) {
	//MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_0, NULL);
	//MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_1, NULL);

	uint8_t config_0 = 0x03;
	MCP8024_WriteCommand(mcp8024, MCP8024_CMD_STATUS_1, &config_0);
}

void MCP8024_RxCpltCallback(MCP8024_t *mcp8024) {
	//if(mcp8024->com_buffer_size>=MCP8024_BUFFER_SIZE-1)
	//	return;

	//mcp8024->com_buffer_size++;

	//__HAL_UART_CLEAR_OREFLAG(mcp8024->com_uart);

	//HAL_UART_Receive_IT(mcp8024->com_uart, &mcp8024->com_buffer[mcp8024->com_buffer_size], 1);
}

void MCP8024_WriteCommand(MCP8024_t *mcp8024, MCP8024_Command_t cmd, uint8_t *argument) {
	if(!argument) {
		HAL_UART_Transmit(mcp8024->com_uart, &cmd, 1, HAL_MAX_DELAY);
		mcp8024->status = HAL_UART_Receive(mcp8024->com_uart, mcp8024->response, 3, 10);
		return;
	}

	uint8_t message[2] = {cmd, *argument};
	HAL_UART_Transmit(mcp8024->com_uart, message, 2, HAL_MAX_DELAY);

	__HAL_UART_CLEAR_OREFLAG(mcp8024->com_uart);
	//__HAL_UART_FLUSH_DRREGISTER(mcp8024->com_uart);

	mcp8024->status = HAL_UART_Receive(mcp8024->com_uart, mcp8024->response, 2, 10);
}

void MCP8024_SetFill(MCP8024_t *mcp8024, uint16_t u_l, uint16_t v_l, uint16_t w_l, uint16_t u_h, uint16_t v_h, uint16_t w_h) {

	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_1, u_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_2, v_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_3, w_h);

	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_1, u_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_2, v_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_3, w_l);
}


