#ifndef __MCP8024_H__
#define __MCP8024_H__

#include "stm32l4xx_hal.h"
#include <string.h>
#include "mcp8024_def.h"

typedef struct {
	TIM_HandleTypeDef *h_mosfet_timer;
	TIM_HandleTypeDef *l_mosfet_timer;

	UART_HandleTypeDef *com_uart;
	uint8_t com_flag;

	union {
		struct {
			uint8_t status_0;
			uint8_t status_1;
			uint8_t config_0;
			uint8_t config_1;
			uint8_t config_2;
		} raw;
		struct {
			MCP8024_Status_0_t status_0;
			MCP8024_Status_1_t status_1;
			MCP8024_Config_0_t config_0;
			MCP8024_Config_1_t config_1;
			MCP8024_Config_2_t config_2;
		} decoded;
	} registers;
} MCP8024_t;

void MCP8024_Init(MCP8024_t *, UART_HandleTypeDef *, TIM_HandleTypeDef *, TIM_HandleTypeDef *);
void MCP8024_Config(MCP8024_t *);

void MCP8024_GetStatus(MCP8024_t *);
void MCP8024_GetConfig(MCP8024_t *);
void MCP8024_SetConfig(MCP8024_t *, MCP8024_Config_0_t, MCP8024_Config_1_t, MCP8024_Config_2_t);

void MCP8024_RxCpltCallback(MCP8024_t *);
void MCP8024_WriteCommand(MCP8024_t *, MCP8024_Command_t, uint8_t *, uint8_t *, uint8_t *);

void MCP8024_SetFill(MCP8024_t *, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);

#endif

