/*
 * axis.h
 *
 *  Created on: Mar 13, 2022
 *      Author: emozdzen
 */

#ifndef INC_AXIS_H_
#define INC_AXIS_H_

#include "arm_math.h"

#include "motor.h"
#include "mcp8024.h"
#include "utilities.h"
#include "phase_current.h"
#include "pid.h"
#include "transformations.h"

typedef struct {
	MCP8024_t mcp8024;

	PhaseCurrent_t phase_current;
	Motor_t motor;

	arm_pid_instance_f32 Id_controller;
	arm_pid_instance_f32 Iq_controller;

	struct {
		float32_t theta;
		float32_t sin_theta;
		float32_t cos_theta;

		float32x3_t Iabc;
		float32x3_t Iab0;
		float32x3_t Idq0;
		float32x3_t Vdq0;
		float32x3_t Vab0;
		float32x3_t Vabc;
	} state;
} Axis_t;

void Axis_Init(Axis_t *);
void Axis_Update(Axis_t *);

void Axis_SimpleFOC(Axis_t *, float32_t, float32x3_t *);
void Axis_FOC(Axis_t *, float32x3_t, float32_t, float32x3_t *);

#endif
