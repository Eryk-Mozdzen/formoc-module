#ifndef INC_PHASE_CURRENT_H_
#define INC_PHASE_CURRENT_H_

#include "stm32l4xx_hal.h"
#include "utilities.h"

#define PHASE_CURRENT_ADC_RESOLUTION	4095	// LSB
#define PHASE_CURRENT_ADC_REFERENCE		3.3f	// V
#define PHASE_CURRENT_GET_ADC_VOLTAGE(lsb)		(((lsb)*PHASE_CURRENT_ADC_REFERENCE)/PHASE_CURRENT_ADC_RESOLUTION)	// V

#define PHASE_CURRENT_SHUNT_RESISTANCE	0.3f	// Ohm
//#define PHASE_CURRENT_AMP_GAIN		1.f		// V/v
//#define PHASE_CURRENT_AMP_OFFSET		0.f		// V

//#define PHASE_CURRENT_GET_AMP_VOLTAGE(vol)		(((vol)-PHASE_CURRENT_AMP_OFFSET)/PHASE_CURRENT_AMP_GAIN)

#define PHASE_CURRENT_A_CONST					(0.623f/(0.951f - 0.89f))
#define PHASE_CURRENT_B_CONST					(-PHASE_CURRENT_A_CONST*9.89f)
#define PHASE_CURRENT_VOLT_TO_AMP(vol)			(PHASE_CURRENT_A_CONST*(vol) + PHASE_CURRENT_B_CONST)

#define PHASE_CURRENT_GET_SHUNT_CURRENT(vol)	((vol)/PHASE_CURRENT_SHUNT_RESISTANCE)

typedef struct {
	ADC_HandleTypeDef *a_phase_adc;
	ADC_HandleTypeDef *b_phase_adc;
	ADC_HandleTypeDef *c_phase_adc;

	uint16_t a_phase_buffer;
	uint16_t b_phase_buffer;
	uint16_t c_phase_buffer;

	uint8_t ready;

	Vector3f_t voltage;
	Vector3f_t current;

	Vector3f_t peak_voltage;
	Vector3f_t peak_current;
} PhaseCurrent_t;

void PhaseCurrent_Init(PhaseCurrent_t *, ADC_HandleTypeDef *, ADC_HandleTypeDef *, ADC_HandleTypeDef *);
void PhaseCurrent_StartSample(PhaseCurrent_t *);

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *, ADC_HandleTypeDef *);
Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *);

#endif
