/*
 * axis.c
 *
 *  Created on: Mar 13, 2022
 *      Author: emozdzen
 */

#include "axis.h"

void Axis_Init(Axis_t *axis) {

}

void Axis_Update(Axis_t *axis) {

}

void Axis_SimpleFOC(Axis_t *axis, float32_t rotor_angle, float32x3_t *phase_voltages) {
	axis->state.theta = rotor_angle;

	arm_sin_cos_f32(axis->state.theta, &axis->state.sin_theta, &axis->state.cos_theta);

	arm_clarke_f32(axis->state.Iabc.x, axis->state.Iabc.y, &axis->state.Iab0.x, &axis->state.Iab0.y);
	arm_park_f32(axis->state.Iab0.x, axis->state.Iab0.y, &axis->state.Idq0.x, &axis->state.Idq0.y, axis->state.sin_theta, axis->state.cos_theta);

	axis->state.Vdq0 = (const float32x3_t) {2.f, 0.f, 0.f};

	arm_inv_park_f32(axis->state.Vdq0.x, axis->state.Vdq0.y, &axis->state.Vab0.x, &axis->state.Vab0.y, axis->state.sin_theta, axis->state.cos_theta);
	axis->state.Vabc = space_vector_modulation(axis->state.Vab0);

	*phase_voltages = axis->state.Vabc;
}

void Axis_FOC(Axis_t *axis, float32x3_t phase_currents, float32_t rotor_angle, float32x3_t *phase_voltages) {
	axis->state.theta = rotor_angle;
	axis->state.Iabc = phase_currents;

	arm_sin_cos_f32(axis->state.theta, &axis->state.sin_theta, &axis->state.cos_theta);

	arm_clarke_f32(axis->state.Iabc.x, axis->state.Iabc.y, &axis->state.Iab0.x, &axis->state.Iab0.y);
	arm_park_f32(axis->state.Iab0.x, axis->state.Iab0.y, &axis->state.Idq0.x, &axis->state.Idq0.y, axis->state.sin_theta, axis->state.cos_theta);

	axis->state.Vdq0.x = -arm_pid_f32(&axis->Id_controller, axis->state.Idq0.x - 0.f);	// setpoint = 0
	axis->state.Vdq0.y = -arm_pid_f32(&axis->Iq_controller, axis->state.Idq0.y - 3.f);	// setpoint = 3

	axis->state.Vdq0.x = (1.f - axis->state.Vdq0.x)*0.5f;
	axis->state.Vdq0.y = (1.f - axis->state.Vdq0.y)*0.5f;

	arm_inv_park_f32(axis->state.Vdq0.x, axis->state.Vdq0.y, &axis->state.Vab0.x, &axis->state.Vab0.y, axis->state.sin_theta, axis->state.cos_theta);
	axis->state.Vabc = space_vector_modulation(axis->state.Vab0);

	*phase_voltages = axis->state.Vabc;
}
