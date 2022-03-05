
#include "phase_current.h"

void PhaseCurrent_Init(PhaseCurrent_t *curr, ADC_HandleTypeDef *b_phase_adc, ADC_HandleTypeDef *c_phase_adc) {
	curr->b_phase_adc = b_phase_adc;
	curr->c_phase_adc = c_phase_adc;

	HAL_ADC_Start_DMA(curr->b_phase_adc, (uint32_t *)curr->b_phase_buffer_raw, PHASE_CURRENT_CONVERSION_NUM);
	HAL_ADC_Start_DMA(curr->c_phase_adc, (uint32_t *)curr->c_phase_buffer_raw, PHASE_CURRENT_CONVERSION_NUM);

	curr->voltage = (const Vector3f_t){0};
	curr->current = (const Vector3f_t){0};
	curr->avg_avg_voltage = (const Vector3f_t){0};
	curr->peak_avg_voltage = (const Vector3f_t){0};
	curr->peak_voltage = (const Vector3f_t){0};
	curr->peak_current = (const Vector3f_t){0};
}

void PhaseCurrent_ConvCpltCallback(PhaseCurrent_t *curr, ADC_HandleTypeDef *hadc) {
	if(hadc->Instance!=curr->b_phase_adc->Instance && hadc->Instance!=curr->c_phase_adc->Instance)
		return;

	if(hadc->Instance==curr->b_phase_adc->Instance) {

		float sum = 0.f;

		for(uint8_t i=0; i<PHASE_CURRENT_CONVERSION_NUM; i++) {
			float voltage = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->b_phase_buffer_raw[i]);

			sum +=voltage;

			curr->peak_voltage.y = MAX(curr->peak_voltage.y, voltage);
			curr->peak_current.y = MAX(curr->peak_current.y, voltage/0.3f);
		}

		curr->voltage.y = sum>0.01f ? sum/PHASE_CURRENT_CONVERSION_NUM : 0.f;
		curr->current.y = curr->voltage.y/0.3f;

		curr->avg_avg_voltage.y = 0.9f*curr->avg_avg_voltage.y + 0.1*curr->voltage.y;
		curr->peak_avg_voltage.y = MAX(curr->peak_avg_voltage.y, curr->voltage.y);
	}

	if(hadc->Instance==curr->c_phase_adc->Instance) {
		float sum = 0.f;

		for(uint8_t i=0; i<PHASE_CURRENT_CONVERSION_NUM; i++) {
			float voltage = PHASE_CURRENT_GET_ADC_VOLTAGE(curr->c_phase_buffer_raw[i]);

			sum +=voltage;

			curr->peak_voltage.z = MAX(curr->peak_voltage.z, voltage);
			curr->peak_current.z = MAX(curr->peak_current.z, voltage/0.3f);
		}

		curr->voltage.z = sum>0.01f ? sum/PHASE_CURRENT_CONVERSION_NUM : 0.f;
		curr->current.z = curr->voltage.z/0.3f;

		curr->avg_avg_voltage.z = 0.9f*curr->avg_avg_voltage.z + 0.1*curr->voltage.z;
		curr->peak_avg_voltage.z = MAX(curr->peak_avg_voltage.z, curr->voltage.z);
	}

	// Kirchhoff's current law
	curr->current.x = -(curr->current.y + curr->current.z);
	curr->peak_current.x = MAX(curr->peak_current.x, curr->current.x);

	if(curr->current.x<0.001f)
		curr->current.x = 0.f;
}

Vector3f_t PhaseCurrent_GetCurrent(PhaseCurrent_t *curr) {
	return curr->current;
}
