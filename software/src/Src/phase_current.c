
#include "phase_current.h"

void PhaseCurrent_Init(PhaseCurrent_t *curr, ADC_HandleTypeDef *b_phase_adc, ADC_HandleTypeDef *c_phase_adc) {
	curr->b_phase_adc = b_phase_adc;
	curr->c_phase_adc = c_phase_adc;

	HAL_ADC_Start_DMA(curr->b_phase_adc, (uint32_t *)curr->b_phase_buffer_raw, PHASE_CURRENT_CONVERSION_NUM);
	HAL_ADC_Start_DMA(curr->c_phase_adc, (uint32_t *)curr->c_phase_buffer_raw, PHASE_CURRENT_CONVERSION_NUM);

	curr->current = (const Vector3f_t){0};
}

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *curr, ADC_HandleTypeDef *hadc) {
	if(hadc->Instance!=curr->b_phase_adc->Instance && hadc->Instance!=curr->c_phase_adc->Instance)
		return;

	if(hadc->Instance==curr->b_phase_adc->Instance) {
		for(uint8_t i=0; i<PHASE_CURRENT_CONVERSION_NUM; i++) {
			float b_adc_voltage = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->b_phase_buffer_raw[i]);
			float b_amp_voltage = PHASE_CURRENT_GET_AMP_VOLTAGE(b_adc_voltage);
			float b_current = PHASE_CURRENT_GET_SHUNT_CURRENT(b_amp_voltage);

			curr->current.y = (1.f - PHASE_CURRENT_FILTER_CONST)*curr->current.y + PHASE_CURRENT_FILTER_CONST*b_current;
		}
	}

	if(hadc->Instance==curr->c_phase_adc->Instance) {
		for(uint8_t i=0; i<PHASE_CURRENT_CONVERSION_NUM; i++) {
			float c_adc_voltage = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->b_phase_buffer_raw[i]);
			float c_amp_voltage = PHASE_CURRENT_GET_AMP_VOLTAGE(c_adc_voltage);
			float c_current = PHASE_CURRENT_GET_SHUNT_CURRENT(c_amp_voltage);

			curr->current.z = (1.f - PHASE_CURRENT_FILTER_CONST)*curr->current.z + PHASE_CURRENT_FILTER_CONST*c_current;
		}
	}

	// Kirchhoff's current law
	curr->current.x = -(curr->current.y + curr->current.z);
}

Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *curr) {
	return curr->current;
}
