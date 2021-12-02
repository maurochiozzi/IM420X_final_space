#include "dft.h"

void computeDFT(const double d_sample, double complex output[restrict], int current_index, size_t n) {
	double complex dc_angle;

	for (int i = 0; i < current_index; i++) {
		dc_angle = 2 * I * M_PI * current_index * i / n;

		output[i] += 2 * (d_sample * cexp( - dc_angle)) / n;
	}

	for (int i = 0; i < current_index; i++) {
		dc_angle = 2 * I * M_PI * current_index * i / n;

		output[current_index] += 2 * (output[i] * cexp( - dc_angle)) / n;
	}
}
