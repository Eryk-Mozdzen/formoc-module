#include "mcp8024.h"
#include "main.h"

void MCP8024_Init(MCP8024_t *mcp8024, TIM_HandleTypeDef *encoder_timer, UART_HandleTypeDef *com_uart) {

	mcp8024->encoder_timer = encoder_timer;
	mcp8024->com_uart = com_uart;
	mcp8024->flag = 0;

	HAL_TIM_Encoder_Start(mcp8024->encoder_timer, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(mcp8024->encoder_timer, TIM_CHANNEL_2);

	HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_Port, CHIP_ENABLE_Pin, GPIO_PIN_SET);

	HAL_Delay(100);
}

void MCP8024_ReadAll(MCP8024_t *mcp8024) {
	//MCP8024_WriteCommand(mcp8024, MCP8024_STATUS_0, NULL, NULL, &mcp8024->status_0);
	//MCP8024_WriteCommand(mcp8024, MCP8024_STATUS_1, NULL, NULL, &mcp8024->status_1);

	//uint8_t config_0 = 0x03;
	mcp8024->config_0 = 0xFF;
	//MCP8024_WriteCommand(mcp8024, MCP8024_SET_CFG_0, &config_0, NULL, NULL);
	MCP8024_WriteCommand(mcp8024, MCP8024_GET_CFG_0, NULL, NULL, &mcp8024->config_0);
}

HAL_StatusTypeDef MCP8024_WriteCommand(MCP8024_t *mcp8024, MCP8024_Command_t cmd, uint8_t *argument, uint8_t *response_cmd, uint8_t *response_data) {
	HAL_UART_Transmit(mcp8024->com_uart, (uint8_t *)&cmd, 1, HAL_MAX_DELAY);

	if(argument)
		HAL_UART_Transmit(mcp8024->com_uart, argument, 1, HAL_MAX_DELAY);

	/*struct {
		uint8_t data;
		uint8_t cmd;
	} response;*/

	//mcp8024->flag = 0;

	__HAL_UART_CLEAR_OREFLAG(mcp8024->com_uart);

	mcp8024->com_status = HAL_UART_Receive(mcp8024->com_uart, mcp8024->response, 3, 100);
	//mcp8024->com_status = HAL_UART_Receive_IT(mcp8024->com_uart, response, 3 + argument ? 1 : 0);

	//while(!mcp8024->flag) {}

	if(mcp8024->com_status!=HAL_OK)
		return mcp8024->com_status;

	/*if(response_cmd)
		*response_cmd = response.cmd;

	if(response_data)
		*response_data = response.data;*/

	if(response_cmd)
		*response_cmd = mcp8024->response[0];

	if(response_data)
		*response_data = mcp8024->response[1];

	return mcp8024->com_status;
}
