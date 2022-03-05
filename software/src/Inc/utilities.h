#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <stdint.h>

#define MIN(a, b) (a<b) ? a : b
#define MAX(a, b) (a>b) ? a : b

#define MIN_3(a, b, c) MIN(MIN(a, b), c)
#define MAX_3(a, b, c) MAX(MAX(a, b), c)

#define PI					3.1415926f
#define SQRT_2				1.4142135f
#define SQRT_3				1.7320508f
#define SQRT_6				2.4494897f

#define DEG_TO_RAD			PI/180.0
#define RAD_TO_DEG			180.0/PI
#define RAD_TO_ROUNDS		1/(2.0*PI)
#define ROUNDS_TO_RAD		2.0*PI

typedef struct {
	float x, y, z;
} Vector3f_t;

typedef struct {
	uint16_t x, y, z;
} Vector3uint16_t;

typedef struct {
	int16_t x, y, z;
} Vector3int16_t;

#endif
