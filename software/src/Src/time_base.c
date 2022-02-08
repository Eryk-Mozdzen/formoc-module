/*
 * time_base.c
 *
 *  Created on: Aug 15, 2021
 *      Author: ermoz
 */

#include "time_base.h"

void TimeBase_Init(TimeBase_StructTypeDef *tbase, TIM_HandleTypeDef *timer, float scale) {
	tbase->timer = timer;
	tbase->scale = scale;

	tbase->time_start = __HAL_TIM_GET_COUNTER(tbase->timer);
}

void TimeBase_Start(TimeBase_StructTypeDef *tbase) {
	tbase->time_start = __HAL_TIM_GET_COUNTER(tbase->timer);
}

uint32_t TimeBase_Restart(TimeBase_StructTypeDef *tbase) {
	uint32_t time_elapsed = __HAL_TIM_GET_COUNTER(tbase->timer) - tbase->time_start;

	tbase->time_start = __HAL_TIM_GET_COUNTER(tbase->timer);

	return time_elapsed;
}

uint32_t TimeBase_GetTimeElapsed(TimeBase_StructTypeDef *tbase) {
	return __HAL_TIM_GET_COUNTER(tbase->timer) - tbase->time_start;
}

float TimeBase_GetScale(TimeBase_StructTypeDef *tbase) {
	return tbase->scale;
}
