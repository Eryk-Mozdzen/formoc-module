/*
 * time_base.h
 *
 *  Created on: Aug 15, 2021
 *      Author: ermoz
 */

#ifndef SNEAK100_UTILITIES_INC_TIME_BASE_H_
#define SNEAK100_UTILITIES_INC_TIME_BASE_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

typedef struct {
	TIM_HandleTypeDef *timer;

	uint32_t time_start;
	float scale;
} TimeBase_StructTypeDef;

void TimeBase_Init(TimeBase_StructTypeDef *, TIM_HandleTypeDef *, float);
void TimeBase_Start(TimeBase_StructTypeDef *);
uint32_t TimeBase_Restart(TimeBase_StructTypeDef *);
uint32_t TimeBase_GetTimeElapsed(TimeBase_StructTypeDef *);
float TimeBase_GetScale(TimeBase_StructTypeDef *);

#endif /* SNEAK100_UTILITIES_INC_TIME_BASE_H_ */
