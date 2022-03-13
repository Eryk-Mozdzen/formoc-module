#include "transformations.h"

float32_t normalize_angle(float32_t angle){
	float a = fmod(angle, _2_PI);
	return (a>=0) ? a : (a+_2_PI);
}

float32x3_t clark_transformation(float32x3_t input) {
	//power-invariant
 	/*return (float32x3_t) {
 		(2.0*input.x - input.y - input.z)*(_1_OVER_SQRT_6),
		(input.y - input.z)*(_1_OVER_SQRT_2),
 		(input.x + input.y + input.z)*(_1_OVER_SQRT_3)
 	};*/
	
	//power-variant
	return (float32x3_t) {
		(2.f*input.x - input.y - input.z)*(0.3333f),
		(input.y - input.z)*(_1_OVER_SQRT_3),
		(input.x + input.y + input.z)*(SQRT_2_OVER_3)
	};
}

float32x3_t inverse_clark_transformation(float32x3_t input) {
	float32x3_t output;
	
	//power-invariant
// 	output.x = (_1_OVER_SQRT_3)*input.z;
// 	output.y = output.x - (_1_OVER_SQRT_6)*input.x;
// 	output.z = output.y - (_1_OVER_SQRT_2)*input.y;
// 	output.y +=(_1_OVER_SQRT_2)*input.y;
// 	output.x +=(SQRT_2_OVER_SQRT_3)*input.x;

	//power-variant
	output.x = (_1_OVER_SQRT_2)*input.z;
	output.y = output.x - (0.5f)*input.x;
	output.z = output.y - (SQRT_3_OVER_2)*input.y;
	output.y +=(SQRT_3_OVER_2)*input.y;
	output.x +=input.x;
	
	return output;
}

float32x3_t park_transformation(float32x3_t input, float32_t theta) {
	float32_t cos_theta = cosf(theta);
	float32_t sin_theta = sinf(theta);

	arm_sin_cos_f32(theta, &sin_theta, &cos_theta);	//?

	return (float32x3_t) {
		cos_theta*input.x + sin_theta*input.y,
		cos_theta*input.y - sin_theta*input.x,
		0
	};
}

float32x3_t inverse_park_transformation(float32x3_t input, float32_t theta) {
	float32_t cos_theta = cosf(theta);
	float32_t sin_theta = sinf(theta);
	
	return (float32x3_t) {
		cos_theta*input.x - sin_theta*input.y,
		sin_theta*input.x + cos_theta*input.y,
		0
	};
}

float32x3_t space_vector_modulation(float32x3_t Vref) {
	
	uint8_t N = 0 | (((SQRT_3*Vref.x + Vref.y)<0)<<2) | (((SQRT_3*Vref.x - Vref.y)>0)<<1) | ((Vref.y>0)<<0);

	float32_t Z = T_PERIOD*(Vref.y - SQRT_3*Vref.x)/(SQRT_2*V_DC);
	float32_t Y = T_PERIOD*(Vref.y + SQRT_3*Vref.x)/(SQRT_2*V_DC);
	float32_t X = T_PERIOD*(Vref.y * 2.f		  )/(SQRT_2*V_DC);
	
	float32_t t_1_in_case_of_N[] = {Z, Y, -Z, -X, X, -Y};
	float32_t t_2_in_case_of_N[] = {Y, -X, X, Z, -Y, -Z};
	
	float32_t t_1 = t_1_in_case_of_N[N-1];
	float32_t t_2 = t_2_in_case_of_N[N-1];

	if((t_1+t_2)>T_PERIOD) {
		float32_t T_1 = t_1*(T_PERIOD/(t_1 + t_2));
		float32_t T_2 = t_2*(T_PERIOD/(t_1 + t_2));
		t_1 = T_1;
		t_2 = T_2;
	}

	float32_t t_0 = T_PERIOD - t_1 - t_2;

	float32_t top = ((t_0*0.5) + t_2 + t_1)/T_PERIOD;
	float32_t mid = ((t_0*0.5) + t_2	  )/T_PERIOD;
	float32_t bot = ((t_0*0.5)			  )/T_PERIOD;
	
	switch(N) {
		case 1: return (float32x3_t) {mid, top, bot}; break;
		case 2: return (float32x3_t) {top, bot, mid}; break;
		case 3: return (float32x3_t) {top, mid, bot}; break;
		case 4: return (float32x3_t) {bot, mid, top}; break;
		case 5: return (float32x3_t) {bot, top, mid}; break;
		case 6: return (float32x3_t) {mid, bot, top}; break;

		default: return (float32x3_t) {0, 0, 0};		break;
	}
}
