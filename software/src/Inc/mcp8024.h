#ifndef __MCP8024_H__
#define __MCP8024_H__

#include "stm32l4xx_hal.h"
#include <string.h>

#define MCP8024_BUFFER_SIZE			64
#define MCP8024_PWM_COMPARE_MAX		3999

typedef enum {
	MCP8024_CMD_SET_CFG_0 = 0x81,
	MCP8024_CMD_GET_CFG_0 = 0x82,
	MCP8024_CMD_STATUS_0 = 0x85,
	MCP8024_CMD_STATUS_1 = 0x86,
	MCP8024_CMD_SET_CFG_1 = 0x83,
	MCP8024_CMD_GET_CFG_1 = 0x84,
	MCP8024_CMD_SET_CFG_2 = 0x87,
	MCP8024_CMD_GET_CFG_2 = 0x88,
} MCP8024_Command_t;

typedef struct {
	uint8_t cmd;
	uint8_t data;
} MCP8024_Message_t;

typedef struct {
	TIM_HandleTypeDef *h_mosfet_timer;
	TIM_HandleTypeDef *l_mosfet_timer;

	UART_HandleTypeDef *com_uart;
	uint8_t com_flag;

	struct {
		uint8_t status_0;
		uint8_t status_1;
		uint8_t config_0;
		uint8_t config_1;
		uint8_t config_2;
	} registers;
} MCP8024_t;

void MCP8024_Init(MCP8024_t *, UART_HandleTypeDef *, TIM_HandleTypeDef *, TIM_HandleTypeDef *);
void MCP8024_ReadAllRegisters(MCP8024_t *);
void MCP8024_WriteCommand(MCP8024_t *, MCP8024_Command_t, uint8_t *, uint8_t *, uint8_t *);
void MCP8024_SetFill(MCP8024_t *, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

void MCP8024_RxCpltCallback(MCP8024_t *);

#endif

