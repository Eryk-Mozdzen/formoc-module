#ifndef __MCP8024_H__
#define __MCP8024_H__

#include "stm32l4xx_hal.h"
#include <string.h>

typedef enum {
	MCP8024_SET_CFG_0 = 0x81,
	MCP8024_GET_CFG_0 = 0x82,
	MCP8024_STATUS_0 = 0x85,
	MCP8024_STATUS_1 = 0x86,
	MCP8024_SET_CFG_1 = 0x83,
	MCP8024_GET_CFG_1 = 0x84,
	MCP8024_SET_CFG_2 = 0x87,
	MCP8024_GET_CFG_2 = 0x88,
} MCP8024_Command_t;

typedef struct {
	TIM_HandleTypeDef *encoder_timer;
	UART_HandleTypeDef *com_uart;
	HAL_StatusTypeDef com_status;

	uint8_t flag;

	uint8_t status_0;
	uint8_t status_1;
	uint8_t config_0;
	uint8_t response[4];
} MCP8024_t;

void MCP8024_Init(MCP8024_t *, TIM_HandleTypeDef *, UART_HandleTypeDef *);
void MCP8024_ReadAll(MCP8024_t *);

HAL_StatusTypeDef MCP8024_WriteCommand(MCP8024_t *, MCP8024_Command_t, uint8_t *, uint8_t *, uint8_t *);

#endif

