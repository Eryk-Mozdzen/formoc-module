#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>
//#include "arm_math.h"

#define MIN(a, b) ((a<b) ? a : b)
#define MAX(a, b) ((a>b) ? a : b)

#define MIN_3(a, b, c) (MIN(MIN(a, b), c))
#define MAX_3(a, b, c) (MAX(MAX(a, b), c))

#define PI					3.1415926f
#define SQRT_2				1.4142135f
#define SQRT_3				1.7320508f
#define SQRT_6				2.4494897f

#define PI_OVER_2			1.570796327f
#define PI_OVER_3			1.047197551f
#define _2_PI				6.283185307f
#define _3_PI_OVER_2		4.71238898f
#define _1_OVER_SQRT_2		0.707106781f
#define _1_OVER_SQRT_3		0.577350269f
#define _1_OVER_SQRT_6		0.40824829f
#define SQRT_2_OVER_3		0.471404521f
#define SQRT_3_OVER_2		0.866025404f
#define SQRT_2_OVER_SQRT_3	0.816496581f

#define DEG_TO_RAD			0.017453293f
#define RAD_TO_DEG			57.295779513f
#define RAD_TO_ROUNDS		0.159154943f
#define ROUNDS_TO_RAD		6.283185307f

typedef float float32_t;
typedef double float64_t;

typedef struct {
	float32_t x, y, z;
} float32x3_t;

typedef struct {
	uint16_t x, y, z;
} uint16x3_t;

typedef struct {
	int16_t x, y, z;
} int16x3_t;

#endif
