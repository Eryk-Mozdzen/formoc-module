#ifndef INC_PHASE_CURRENT_H_
#define INC_PHASE_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "utilities.h"

#define PHASE_CURRENT_ADC_RESOLUTION	4095	// LSB
#define PHASE_CURRENT_ADC_REFERENCE		3.3f	// V
#define PHASE_CURRENT_AMP_GAIN			1.f		// V/v
#define PHASE_CURRENT_AMP_OFFSET		0.f		// V
#define PHASE_CURRENT_SHUNT_RESISTANCE	0.3f	// Ohm

#define PHASE_CURRENT_GET_ADC_VOLTAGE(lsb)		(((lsb)*PHASE_CURRENT_ADC_REFERENCE)/PHASE_CURRENT_ADC_RESOLUTION)	// V
#define PHASE_CURRENT_GET_AMP_VOLTAGE(vol)		(((vol)-PHASE_CURRENT_AMP_OFFSET)/PHASE_CURRENT_AMP_GAIN)			// V
#define PHASE_CURRENT_GET_SHUNT_CURRENT(vol)	((vol)/PHASE_CURRENT_SHUNT_RESISTANCE)								// A

#define PHASE_CURRENT_CONVERSION_NUM	6

typedef struct {
	ADC_HandleTypeDef *b_phase_adc;
	ADC_HandleTypeDef *c_phase_adc;

	uint16_t b_phase_buffer_raw[PHASE_CURRENT_CONVERSION_NUM];
	uint16_t c_phase_buffer_raw[PHASE_CURRENT_CONVERSION_NUM];

	Vector3f_t voltage;
	Vector3f_t current;

	Vector3f_t avg_avg_voltage;
	Vector3f_t peak_avg_voltage;

	Vector3f_t peak_voltage;
	Vector3f_t peak_current;
} PhaseCurrent_t;

void PhaseCurrent_Init(PhaseCurrent_t *, ADC_HandleTypeDef *, ADC_HandleTypeDef *);

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *, ADC_HandleTypeDef *);
Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *);

#endif
