#ifndef TRANSFORMATIONS_H_
#define TRANSFORMATIONS_H_

#include "utilities.h"
#include <math.h>

#define V_DC				12.6f
#define T_PERIOD			1.f

float32_t normalize_angle(float32_t);

float32x3_t clark_transformation(float32x3_t);
float32x3_t inverse_clark_transformation(float32x3_t);
float32x3_t park_transformation(float32x3_t, float32_t);
float32x3_t inverse_park_transformation(float32x3_t, float32_t);

float32x3_t space_vector_modulation(float32x3_t);

#endif
