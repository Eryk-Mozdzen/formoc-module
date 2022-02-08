#ifndef __PID_H__
#define __PID_H__

#include <stdlib.h>
#include <stdint.h>
#include "time_base.h"

typedef struct {
    float Kp, Ki, Kd;
    float integral, prev_process_value;
    float integral_band;

    float delta_time, error, derivative, output;	// for debug

    TimeBase_StructTypeDef tbase;
} PID_StructTypeDef;

void PID_Init(PID_StructTypeDef *, float, float, float, float);
void PID_Reset(PID_StructTypeDef *);
float PID_Update(PID_StructTypeDef *, float, float);

#endif
