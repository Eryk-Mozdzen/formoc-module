
#include "motor.h"

void Motor_Init(Motor_t *motor, TIM_HandleTypeDef *encoder_timer) {
	motor->encoder_timer = encoder_timer;

	HAL_TIM_Encoder_Start(motor->encoder_timer, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(motor->encoder_timer, TIM_CHANNEL_2);
}

float Motor_GetMechanicalPosition(Motor_t *motor) {
	uint16_t encoder = __HAL_TIM_GET_COUNTER(motor->encoder_timer);
	float mech = encoder/MOTOR_ENCODER_CPR;

	return mech;
}

float Motor_GetElectricalPosition(Motor_t *motor) {
	return Motor_GetMechanicalPosition(motor)/MOTOR_POLE_PAIRS;
}
