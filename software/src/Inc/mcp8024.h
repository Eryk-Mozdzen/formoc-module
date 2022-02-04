#ifndef __MCP8024_H__
#define __MCP8024_H__

#include "stm32l4xx_hal.h"
#include <string.h>

#define MCP8024_BUFFER_SIZE			256
#define MCP8024_PWM_COMPARE_MAX		4000

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
	TIM_HandleTypeDef *h_mosfet_timer;
	TIM_HandleTypeDef *l_mosfet_timer;
	TIM_HandleTypeDef *encoder_timer;

	UART_HandleTypeDef *com_uart;
	HAL_StatusTypeDef com_status;

	uint8_t buffer[MCP8024_BUFFER_SIZE];
	uint32_t buffer_size;

	uint8_t status_0;
	uint8_t status_1;
	uint8_t config_0;
} MCP8024_t;

void MCP8024_Init(MCP8024_t *, TIM_HandleTypeDef *, UART_HandleTypeDef *, TIM_HandleTypeDef *, TIM_HandleTypeDef *);
void MCP8024_ReadAll(MCP8024_t *);
HAL_StatusTypeDef MCP8024_WriteCommand(MCP8024_t *, MCP8024_Command_t, uint8_t *, uint8_t *, uint8_t *);
void MCP8024_SetFill(MCP8024_t *, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

void MCP8024_RxCpltCallback(MCP8024_t *);

#endif

