#ifndef INC_VECTOR3_H_
#define INC_VECTOR3_H_

#include <stdint.h>

typedef struct {
	float x, y, z;
} Vector3f_t;

typedef struct {
	uint8_t x, y, z;
} Vector3u8_t;

typedef struct {
	uint16_t x, y, z;
} Vector3u16_t;

typedef struct {
	uint32_t x, y, z;
} Vector3u32_t;


#endif
