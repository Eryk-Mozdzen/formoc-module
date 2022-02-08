#ifndef INC_PHASE_CURRENT_H_
#define INC_PHASE_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "vector3.h"

#define PHASE_CURRENT_ADC_RESOLUTION	4095	// LSB
#define PHASE_CURRENT_ADC_REFERENCE		2.087f	// V
#define PHASE_CURRENT_AMP_GAIN			1.f
#define PHASE_CURRENT_AMP_OFFSET		0.f		// V
#define PHASE_CURRENT_SHUNT_RESISTANCE	0.003f	// Ohm

#define PHASE_CURRENT_GET_ADC_VOLTAGE(lsb)		(((lsb)*PHASE_CURRENT_ADC_REFERENCE)/PHASE_CURRENT_ADC_RESOLUTION)	// V
#define PHASE_CURRENT_GET_AMP_VOLTAGE(vol)		(((vol)-PHASE_CURRENT_AMP_OFFSET)/PHASE_CURRENT_AMP_GAIN)			// V
#define PHASE_CURRENT_GET_SHUNT_CURRENT(vol)	((vol)/PHASE_CURRENT_SHUNT_RESISTANCE)								// A

#define PHASE_CURRENT_CONVERSION_NUM	8
#define PHASE_CURRENT_FILTER_CONST		0.1f

typedef struct {
	ADC_HandleTypeDef *b_phase_adc;
	ADC_HandleTypeDef *c_phase_adc;

	uint16_t b_phase_buffer_raw[PHASE_CURRENT_CONVERSION_NUM];
	uint16_t c_phase_buffer_raw[PHASE_CURRENT_CONVERSION_NUM];

	Vector3f_t current;
} PhaseCurrent_t;

void PhaseCurrent_Init(PhaseCurrent_t *, ADC_HandleTypeDef *, ADC_HandleTypeDef *);

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *, ADC_HandleTypeDef *);
Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *);

#endif
