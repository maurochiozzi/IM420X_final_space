#include "magnetic_field.h"

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c) {
	MagneticField mf_sample;

	float d_response[3];

	if (HAL_OK == readMagneticSensor(i2c, d_response)) {

		mf_sample.x = d_response[0];
		mf_sample.y = d_response[1];
		mf_sample.z = d_response[2];

	}

	return mf_sample;
}

void identifyMagneticField(float complex *d_mf_x_dft, float complex *d_mf_y_dft,
		float complex *d_mf_z_dft, MagneticFieldSource *mf_nodes) {

	for (int i = 0; i < 3; i++) {
		int bin = (int) mf_nodes[i].i_frequency / 2;

		float complex value = d_mf_z_dft[bin];

		mf_nodes[i].mf_intensity.z = cabs(value);
	}
}

float getDistanceFromRSS(MagneticFieldSource node) {
	float distance = cbrt(node.d_magnetic_cte / node.mf_intensity.z);

	return distance;
}

SpacePosition estimatePoisition(MagneticFieldSource *nodes) {
	SpacePosition sp_rx_position;

	float d_distances[3];

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

