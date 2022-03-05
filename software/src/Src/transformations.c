#include "transformations.h"

float normalize_angle(float angle){
	float a = fmod(angle, _2_PI);
	return (a>=0) ? a : (a+_2_PI);
}

Vector3f_t clark_transformation(Vector3f_t input) {
	//power-invariant
 	return (vector_3d) {
 		(2.0*input.x - input.y - input.z)*(_1_OVER_SQRT_6),
		(input.y - input.z)*(_1_OVER_SQRT_2),
 		(input.x + input.y + input.z)*(_1_OVER_SQRT_3)
 	};
	
	//power-variant
	/*return (Vector3f_t) {
		(2.f*input.x - input.y - input.z)*(0.3333f),
		(input.y - input.z)*(_1_OVER_SQRT_3),
		(input.x + input.y + input.z)*(SQRT_2_OVER_3)
	};*/
}

Vector3f_t inverse_clark_transformation(Vector3f_t input) {
	Vector3f_t output;
	
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

Vector3f_t park_transformation(Vector3f_t input, float theta) {
	float cos_theta = cosf(theta);
	float sin_theta = sinf(theta);
	
	return (Vector3f_t) {
		cos_theta*input.x + sin_theta*input.y,
		cos_theta*input.y - sin_theta*input.x,
		0
	};
}

Vector3f_t inverse_park_transformation(Vector3f_t input, float theta) {
	float cos_theta = cosf(theta);
	float sin_theta = sinf(theta);
	
	return (Vector3f_t) {
		cos_theta*input.x - sin_theta*input.y,
		sin_theta*input.x + cos_theta*input.y,
		0
	};
}

Vector3f_t space_vector_modulation(Vector3f_t Vref) {
	
	uint8_t N = 0 | (((SQRT_3*Vref.x + Vref.y)<0)<<2) | (((SQRT_3*Vref.x - Vref.y)>0)<<1) | ((Vref.y>0)<<0);

	float Z = T_PERIOD*(Vref.y - SQRT_3*Vref.x)/(SQRT_2*V_DC);
	float Y = T_PERIOD*(Vref.y + SQRT_3*Vref.x)/(SQRT_2*V_DC);
	float X = T_PERIOD*(Vref.y * 2.f		  )/(SQRT_2*V_DC);
	
	float t_1_in_case_of_N[] = {Z, Y, -Z, -X, X, -Y};
	float t_2_in_case_of_N[] = {Y, -X, X, Z, -Y, -Z};
	
	float t_1 = t_1_in_case_of_N[N-1];
	float t_2 = t_2_in_case_of_N[N-1];

	if((t_1+t_2)>T_PERIOD) {
		float T_1 = t_1*(T_PERIOD/(t_1 + t_2));
		float T_2 = t_2*(T_PERIOD/(t_1 + t_2));
		t_1 = T_1;
		t_2 = T_2;
	}

	float t_0 = T_PERIOD - t_1 - t_2;

	float top = ((t_0*0.5) + t_2 + t_1)/T_PERIOD;
	float mid = ((t_0*0.5) + t_2	  )/T_PERIOD;
	float bot = ((t_0*0.5)			  )/T_PERIOD;
	
	switch(N) {
		case 1: return (Vector3f_t) {mid, top, bot}; break;
		case 2: return (Vector3f_t) {top, bot, mid}; break;
		case 3: return (Vector3f_t) {top, mid, bot}; break;
		case 4: return (Vector3f_t) {bot, mid, top}; break;
		case 5: return (Vector3f_t) {bot, top, mid}; break;
		case 6: return (Vector3f_t) {mid, bot, top}; break;

		default: return (Vector3f_t) {0, 0, 0};		break;
	}
}

Vector3f_t SVM(Vector3f_t Vref, float DTC_MIN, float DTC_MAX) {
	float v_offset = (MIN_3(Vref.x, Vref.y, Vref.z) + MAX_3(Vref.x, Vref.y, Vref.z))/2.0;
	
	return (Vector3f_t) {
		MIN(MAX(((Vref.x - v_offset)/V_DC + 0.5f), DTC_MIN), DTC_MAX),
		MIN(MAX(((Vref.y - v_offset)/V_DC + 0.5f), DTC_MIN), DTC_MAX),
		MIN(MAX(((Vref.z - v_offset)/V_DC + 0.5f), DTC_MIN), DTC_MAX)
	};
}
