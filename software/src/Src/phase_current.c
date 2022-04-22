
#include "phase_current.h"

void PhaseCurrent_Init(PhaseCurrent_t *curr, ADC_HandleTypeDef *b_phase_adc, ADC_HandleTypeDef *c_phase_adc) {
	curr->b_phase_adc = b_phase_adc;
	curr->c_phase_adc = c_phase_adc;

	curr->ready = 0;
	curr->voltage = (const float32x3_t){0};
	curr->current = (const float32x3_t){0};
	curr->peak_voltage = (const float32x3_t){0};
	curr->peak_current = (const float32x3_t){0};

	HAL_ADCEx_Calibration_Start(curr->b_phase_adc, ADC_SINGLE_ENDED);
	HAL_ADCEx_Calibration_Start(curr->c_phase_adc, ADC_SINGLE_ENDED);

	HAL_Delay(10);

	HAL_ADC_Start(curr->c_phase_adc);
	HAL_ADCEx_MultiModeStart_DMA(curr->b_phase_adc, (uint32_t *)curr->phase_buffer, 1);
}

void PhaseCurrent_Reset(PhaseCurrent_t *curr) {
	curr->ready = 0;
	curr->voltage = (const float32x3_t){0};
	curr->current = (const float32x3_t){0};
	curr->peak_voltage = (const float32x3_t){0};
	curr->peak_current = (const float32x3_t){0};
}

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *curr, ADC_HandleTypeDef *hadc) {
	curr->ready = 1;
}

uint8_t PhaseCurrent_IsReady(PhaseCurrent_t *curr) {
	return curr->ready;
}

float32x3_t PhaseCurrent_GetCurrent(PhaseCurrent_t *curr) {
	curr->ready = 0;

	// read voltage
	curr->voltage.x = 0.f;
	curr->voltage.y = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->phase_buffer[0]);
	curr->voltage.z = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->phase_buffer[1]);

	// calculate current
	curr->current.y = PHASE_CURRENT_VOLT_TO_AMP_B(curr->phase_buffer[0]);
	curr->current.z = PHASE_CURRENT_VOLT_TO_AMP_C(curr->phase_buffer[1]);

	// Kirchhoff's current law
	curr->current.x = -(curr->current.y + curr->current.z);

	curr->peak_voltage.x = 0.f;
	curr->peak_voltage.y = MAX(curr->peak_voltage.y, fabs(curr->voltage.y));
	curr->peak_voltage.z = MAX(curr->peak_voltage.z, fabs(curr->voltage.z));

	curr->peak_current.x = MAX(curr->peak_current.x, fabs(curr->current.x));
	curr->peak_current.y = MAX(curr->peak_current.y, fabs(curr->current.y));
	curr->peak_current.z = MAX(curr->peak_current.z, fabs(curr->current.z));

	return curr->current;
}
