#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32l4xx_hal.h"

#define MOTOR_ENCODER_CPR	512
#define MOTOR_POLE_PAIRS	6

typedef struct {
	TIM_HandleTypeDef *encoder_timer;

} Motor_t;

void Motor_Init(Motor_t *, TIM_HandleTypeDef *);

float Motor_GetMechanicalPosition(Motor_t *);
float Motor_GetElectricalPosition(Motor_t *);

#endif
