
#include "phase_current.h"

void PhaseCurrent_Init(PhaseCurrent_t *curr, ADC_HandleTypeDef *a_phase_adc, ADC_HandleTypeDef *b_phase_adc, ADC_HandleTypeDef *c_phase_adc) {
	curr->a_phase_adc = a_phase_adc;
	curr->b_phase_adc = b_phase_adc;
	curr->c_phase_adc = c_phase_adc;

	curr->voltage = (const Vector3f_t){0};
	curr->current = (const Vector3f_t){0};
	curr->peak_voltage = (const Vector3f_t){0};
	curr->peak_current = (const Vector3f_t){0};
}

void PhaseCurrent_StartSample(PhaseCurrent_t *curr) {
	HAL_ADC_Start_DMA(curr->a_phase_adc, (uint32_t *)&curr->a_phase_buffer, 1);
	HAL_ADC_Start_DMA(curr->b_phase_adc, (uint32_t *)&curr->b_phase_buffer, 1);
	HAL_ADC_Start_DMA(curr->c_phase_adc, (uint32_t *)&curr->c_phase_buffer, 1);
}

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *curr, ADC_HandleTypeDef *hadc) {


	/*if(hadc->Instance!=curr->a_phase_adc->Instance && hadc->Instance!=curr->b_phase_adc->Instance && hadc->Instance!=curr->c_phase_adc->Instance)
		return;

	if(hadc->Instance==curr->b_phase_adc->Instance) {
		curr->voltage.y = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->b_phase_buffer);

		if(curr->voltage.y<0.001f)
			curr->voltage.y = 0.f;

		curr->current.y = PHASE_CURRENT_VOLT_TO_AMP(curr->voltage.y);
		curr->peak_current.y = MAX(curr->peak_current.y, curr->current.y);
	}

	if(hadc->Instance==curr->c_phase_adc->Instance) {
		curr->voltage.z = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->c_phase_buffer);

		if(curr->voltage.z<0.001f)
			curr->voltage.z = 0.f;

		curr->current.z = PHASE_CURRENT_VOLT_TO_AMP(curr->voltage.z);
		curr->peak_current.z = MAX(curr->peak_current.z, curr->current.z);
	}

	// Kirchhoff's current law
	curr->current.x = -(curr->current.y + curr->current.z);
	curr->peak_current.x = MAX(curr->peak_current.x, curr->current.x);

	if(curr->current.x<0.001f)
		curr->current.x = 0.f;*/
}

Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *curr) {
	return curr->current;
}
