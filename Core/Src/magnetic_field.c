#include "magnetic_field.h"
#include "dft.h"

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c) {
	MagneticField mf_sample;

	float f_response[3];

	if (HAL_OK == readMagneticSensor(i2c, f_response)) {

		mf_sample.x = f_response[0];
		mf_sample.y = f_response[1];
		mf_sample.z = f_response[2];
	}

	return mf_sample;
}

void identifyMagneticField(double *d_mf_x_samples, double *d_mf_y_samples, double *d_mf_z_samples,
		MagneticFieldSource *mf_nodes, uint16_t ui16_sample_size) {

	double inimag[ui16_sample_size];

	double f_mf_intensity_z_real_out[ui16_sample_size];
	double f_mf_intensity_z_img_out[ui16_sample_size];


	compute_dft_real_pair(d_mf_z_samples, inimag, f_mf_intensity_z_real_out,
			f_mf_intensity_z_img_out, ui16_sample_size);

	f_mf_intensity_z_real_out;
	f_mf_intensity_z_img_out;

	// perform dft

	// identify fields and intensity
}

double getDistanceFromRSS(MagneticFieldSource node) {
	double distance = cbrt(node.d_magnetic_cte / node.mf_intensity.z);

	return distance;
}

SpacePosition estimatePoisition(MagneticFieldSource *nodes ) {
	SpacePosition sp_rx_position;

	double d_distances[3];

	for (int nodeIndex = 0; nodeIndex < 3; nodeIndex++) {
		d_distances[nodeIndex] = getDistanceFromRSS(nodes[nodeIndex]);
	}

	sp_rx_position.x = (pow(d_distances[0], 2) - pow(d_distances[1], 2)
			+ pow(nodes[1].sp_position.x, 2)) / (2 * nodes[1].sp_position.x);
	sp_rx_position.y = (pow(d_distances[0], 2) - pow(d_distances[2], 2)
			+ pow(nodes[2].sp_position.x, 2) + pow(nodes[2].sp_position.y, 2)
			- 2 * nodes[2].sp_position.x * sp_rx_position.x)
			/ (2 * nodes[2].sp_position.y);
	sp_rx_position.z = 0;

	return sp_rx_position;
}

