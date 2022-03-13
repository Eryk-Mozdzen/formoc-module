#ifndef INC_PHASE_CURRENT_H_
#define INC_PHASE_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "utilities.h"
#include <math.h>

#define PHASE_CURRENT_ADC_RESOLUTION	4095	// LSB
#define PHASE_CURRENT_ADC_REFERENCE		3.3f	// V
#define PHASE_CURRENT_GET_ADC_VOLTAGE(lsb)		(((lsb)*PHASE_CURRENT_ADC_REFERENCE)/PHASE_CURRENT_ADC_RESOLUTION)	// V

#define PHASE_CURRENT_A_CONST					(0.623f/(0.951f - 0.89f))
#define PHASE_CURRENT_B_CONST					(-PHASE_CURRENT_A_CONST*0.89f)
#define PHASE_CURRENT_VOLT_TO_AMP(vol)			(PHASE_CURRENT_A_CONST*(vol) + PHASE_CURRENT_B_CONST)

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
