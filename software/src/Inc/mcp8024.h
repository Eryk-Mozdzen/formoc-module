#ifndef __MCP8024_H__
#define __MCP8024_H__

#include "stm32l4xx_hal.h"
#include "mcp8024_def.h"
#include "utilities.h"

#define MCP8024_PWM_MAX_FILL	1999

typedef struct {
	GPIO_TypeDef *ce_port;
	uint16_t ce_pin;

	TIM_HandleTypeDef *mosfet_h_timer;
	TIM_HandleTypeDef *mosfet_l_timer;

	HAL_StatusTypeDef uart_status;
	UART_HandleTypeDef *com_uart;

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

void MCP8024_Init(MCP8024_t *, GPIO_TypeDef *, uint16_t, UART_HandleTypeDef *, TIM_HandleTypeDef *, TIM_HandleTypeDef *);
void MCP8024_Config(MCP8024_t *);

void MCP8024_GetStatus(MCP8024_t *);
void MCP8024_GetConfig(MCP8024_t *);
void MCP8024_SetConfig(MCP8024_t *, MCP8024_Config_0_t, MCP8024_Config_1_t, MCP8024_Config_2_t);

void MCP8024_RxCpltCallback(MCP8024_t *);

void MCP8024_SetCompare(MCP8024_t *, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
void MCP8024_SetFill(MCP8024_t *, float32x3_t);

#endif

