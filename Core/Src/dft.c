#include "dft.h"

const float complex MINUS_PI_I = - M_PI * I;

size_t samples_size;

/**
 * This matrix with save the values of
 *
 * cexp( - dc_angle = 2 * I * M_PI * current_index * index / samples_size );
 *
 */
float complex dc_angles[110][110];

float double_per_sample_size;

void initDFT(size_t _samples_size) {

	samples_size = _samples_size;
	double_per_sample_size = 2.0 / samples_size;

	for (int i = 0; i < samples_size; i++) { // this will be the current_index
		for (int j = 0; j < samples_size; j++) {
			dc_angles[i][j] = cexp(i * j * double_per_sample_size * MINUS_PI_I);
		}
	}
}

void computeDFT(const MagneticField d_sample, MagneticFieldComplex output[restrict],MagneticField input[restrict], int current_index) {
	float complex dc_angle;

	for (int i = 0; i < current_index + 1; i++) {
		dc_angle = dc_angles[current_index][i];

		output[i].x += double_per_sample_size * (d_sample.x * ( dc_angle));
		output[i].y += double_per_sample_size * (d_sample.y * ( dc_angle));
		output[i].z += double_per_sample_size * (d_sample.z * ( dc_angle));
	}

	for (int i = 0; i < current_index; i++) {
		dc_angle = dc_angles[current_index][i];

		output[current_index].x += double_per_sample_size * (input[i].x * ( dc_angle));
		output[current_index].y += double_per_sample_size * (input[i].y * ( dc_angle));
		output[current_index].z += double_per_sample_size * (input[i].z * ( dc_angle));
	}
}
