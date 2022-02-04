#include "mcp8024.h"
#include "main.h"

void MCP8024_Init(MCP8024_t *mcp8024, TIM_HandleTypeDef *encoder_timer, UART_HandleTypeDef *com_uart, TIM_HandleTypeDef *h_mosfet_timer, TIM_HandleTypeDef *l_mosfet_timer) {

	mcp8024->h_mosfet_timer = h_mosfet_timer;
	mcp8024->l_mosfet_timer = l_mosfet_timer;
	mcp8024->encoder_timer = encoder_timer;
	mcp8024->com_uart = com_uart;
	mcp8024->buffer_size = 0;

	memset(mcp8024->buffer, 0, MCP8024_BUFFER_SIZE);

	HAL_TIM_Encoder_Start(mcp8024->encoder_timer, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(mcp8024->encoder_timer, TIM_CHANNEL_2);

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_SET);

	HAL_Delay(100);

	//HAL_UART_Receive_IT(mcp8024->com_uart, mcp8024->buffer, 1);

	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->h_mosfet_timer, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(mcp8024->l_mosfet_timer, TIM_CHANNEL_3);
}

void MCP8024_ReadAll(MCP8024_t *mcp8024) {
	//MCP8024_WriteCommand(mcp8024, MCP8024_STATUS_0, NULL, NULL, &mcp8024->status_0);
	//MCP8024_WriteCommand(mcp8024, MCP8024_STATUS_1, NULL, NULL, &mcp8024->status_1);

	uint8_t config_0 = 0x03;
	mcp8024->config_0 = 0xFF;
	MCP8024_WriteCommand(mcp8024, MCP8024_SET_CFG_0, &config_0, NULL, NULL);
	//MCP8024_WriteCommand(mcp8024, MCP8024_GET_CFG_0, NULL, NULL, &mcp8024->config_0);
}

void MCP8024_RxCpltCallback(MCP8024_t *mcp8024) {
	if(mcp8024->buffer_size>=MCP8024_BUFFER_SIZE)
		return;

	mcp8024->buffer_size++;

	//__HAL_UART_CLEAR_OREFLAG(mcp8024->com_uart);

	//HAL_UART_Receive_IT(mcp8024->com_uart, &mcp8024->buffer[mcp8024->buffer_size], 1);
}

HAL_StatusTypeDef MCP8024_WriteCommand(MCP8024_t *mcp8024, MCP8024_Command_t cmd, uint8_t *argument, uint8_t *response_cmd, uint8_t *response_data) {

	//HAL_UART_DMAStop(mcp8024->com_uart);
	//HAL_UART_Receive_DMA(mcp8024->com_uart, mcp8024->response, 2 + (argument ? 1 : 0));

	HAL_UART_Transmit(mcp8024->com_uart, (uint8_t *)&cmd, 1, HAL_MAX_DELAY);

	HAL_Delay(1);

	if(argument)
		HAL_UART_Transmit(mcp8024->com_uart, argument, 1, HAL_MAX_DELAY);

	//memset(mcp8024->response, 0, 4);

	/*struct {
		uint8_t data;
		uint8_t cmd;
	} response;*/

	//mcp8024->flag = 0;

	//__HAL_UART_CLEAR_OREFLAG(mcp8024->com_uart);
	//__HAL_UART_FLUSH_DRREGISTER(mcp8024->com_uart);
	//__HAL_UART_CLEAR_PEFLAG(mcp8024->com_uart);
	//__HAL_UART_CLEAR_FEFLAG(mcp8024->com_uart);
	//__HAL_UART_CLEAR_NEFLAG(mcp8024->com_uart);

	//mcp8024->com_status = HAL_UART_Receive(mcp8024->com_uart, mcp8024->response, 2, 10);
	//mcp8024->com_status = HAL_UART_Receive_IT(mcp8024->com_uart, response, 3 + argument ? 1 : 0);

	//while(!mcp8024->flag) {}

	//if(mcp8024->com_status!=HAL_OK)
	//	return mcp8024->com_status;

	/*if(response_cmd)
		*response_cmd = response.cmd;

	if(response_data)
		*response_data = response.data;*/

	/*if(response_cmd)
		*response_cmd = mcp8024->response[0];

	if(response_data)
		*response_data = mcp8024->response[1];*/

	return mcp8024->com_status;
}

void MCP8024_SetFill(MCP8024_t *mcp8024, uint16_t u_l, uint16_t v_l, uint16_t w_l, uint16_t u_h, uint16_t v_h, uint16_t w_h) {

	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_1, u_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_2, v_h);
	__HAL_TIM_SET_COMPARE(mcp8024->h_mosfet_timer, TIM_CHANNEL_3, w_h);

	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_1, u_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_2, v_l);
	__HAL_TIM_SET_COMPARE(mcp8024->l_mosfet_timer, TIM_CHANNEL_3, w_l);
}


