#ifndef INC_PHASE_CURRENT_H_
#define INC_PHASE_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "utilities.h"
#include <math.h>

#define PHASE_CURRENT_ADC_RESOLUTION	4095	// LSB
#define PHASE_CURRENT_ADC_REFERENCE		3.3f	// V
#define PHASE_CURRENT_GET_ADC_VOLTAGE(lsb)		(((lsb)*PHASE_CURRENT_ADC_REFERENCE)/PHASE_CURRENT_ADC_RESOLUTION)	// V

#define PHASE_CURRENT_TRIMM_B_0					1054
#define PHASE_CURRENT_TRIMM_B_100				1076
#define PHASE_CURRENT_TRIMM_C_0					1096
#define PHASE_CURRENT_TRIMM_C_100				1114

#define PHASE_CURRENT_A_CONST_B					(100.f/(PHASE_CURRENT_TRIMM_B_100 - PHASE_CURRENT_TRIMM_B_0))
#define PHASE_CURRENT_B_CONST_B					(-PHASE_CURRENT_A_CONST_B*PHASE_CURRENT_TRIMM_B_0)
#define PHASE_CURRENT_VOLT_TO_AMP_B(vol)		((PHASE_CURRENT_A_CONST_B*(vol) + PHASE_CURRENT_B_CONST_B)*0.001f)

#define PHASE_CURRENT_A_CONST_C					(100.f/(PHASE_CURRENT_TRIMM_C_100 - PHASE_CURRENT_TRIMM_C_0))
#define PHASE_CURRENT_B_CONST_C					(-PHASE_CURRENT_A_CONST_C*PHASE_CURRENT_TRIMM_C_0)
#define PHASE_CURRENT_VOLT_TO_AMP_C(vol)		((PHASE_CURRENT_A_CONST_C*(vol) + PHASE_CURRENT_B_CONST_C)*0.001f)

typedef struct {
	ADC_HandleTypeDef *b_phase_adc;
	ADC_HandleTypeDef *c_phase_adc;

	uint16_t phase_buffer[2];

	uint8_t ready;

	float32x3_t voltage;
	float32x3_t current;

	float32x3_t peak_voltage;
	float32x3_t peak_current;
} PhaseCurrent_t;

void PhaseCurrent_Init(PhaseCurrent_t *, ADC_HandleTypeDef *, ADC_HandleTypeDef *);
void PhaseCurrent_Reset(PhaseCurrent_t *);

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *, ADC_HandleTypeDef *);

uint8_t PhaseCurrent_IsReady(PhaseCurrent_t *);
float32x3_t PhaseCurrent_GetCurrent(PhaseCurrent_t *);

#endif
