#ifndef TRANSFORMATIONS_H_
#define TRANSFORMATIONS_H_

#include "utilities.h"
#include <inttypes.h>
#include <math.h>

#define PI_OVER_2			PI/2.0
#define PI_OVER_3			PI/3.0
#define _2_PI				PI*2.0
#define _3_PI_OVER_2		PI*1.5
#define _1_OVER_SQRT_2		1/SQRT_2
#define _1_OVER_SQRT_3		1/SQRT_3
#define _1_OVER_SQRT_6		1/SQRT_6
#define SQRT_2_OVER_3		SQRT_2/3.0
#define SQRT_3_OVER_2		SQRT_3/2.0
#define SQRT_2_OVER_SQRT_3	SQRT_2/SQRT_3

#define V_DC				12.6
#define V_REF_MAX			V_DC/SQRT_3
#define T_PERIOD			1.0

float normalize_angle(float);

Vector3f_t clark_transformation(Vector3f_t);
Vector3f_t inverse_clark_transformation(Vector3f_t);
Vector3f_t park_transformation(Vector3f_t, float);
Vector3f_t inverse_park_transformation(Vector3f_t, float);

Vector3f_t space_vector_modulation(Vector3f_t);
Vector3f_t SVM(Vector3f_t, float, float);

#endif
