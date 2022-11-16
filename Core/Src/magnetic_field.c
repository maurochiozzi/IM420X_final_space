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

void identifyMagneticField(MagneticFieldComplex *mfc_dft, MagneticFieldSource *mf_nodes) {

	for (int i = 0; i < 3; i++) {
		int bin = (int) mf_nodes[i].i_frequency / 2;

		MagneticFieldComplex value = mfc_dft[bin];

		mf_nodes[i].mf_intensity.x = cabs(value.x);
		mf_nodes[i].mf_intensity.y = cabs(value.y);
		mf_nodes[i].mf_intensity.z = cabs(value.z);
	}
}

float getDistanceFromRSS(MagneticFieldSource node) {
	float total_intensity = sqrt(pow(node.mf_intensity.x, 2) + pow(node.mf_intensity.y, 2) + pow(node.mf_intensity.z, 2));

	float distance = cbrt(node.d_magnetic_cte / total_intensity * 1000000);

	return distance;
}

SpacePosition estimatePoisition(MagneticFieldSource *nodes) {
	SpacePosition sp_rx_position;

	float d_distances[3];

	for (int nodeIndex = 0; nodeIndex < 3; nodeIndex++) {
		d_distances[nodeIndex] = getDistanceFromRSS(nodes[nodeIndex]);
	}

	/*sp_rx_position.x = (pow(d_distances[0], 2) - pow(d_distances[1], 2)
			+ pow(nodes[1].sp_position.x, 2)) / (2 * nodes[1].sp_position.x);
	sp_rx_position.y = (pow(d_distances[0], 2) - pow(d_distances[2], 2)
			+ pow(nodes[2].sp_position.x, 2) + pow(nodes[2].sp_position.y, 2)
			- 2 * nodes[2].sp_position.x * sp_rx_position.x)
			/ (2 * nodes[2].sp_position.y);
	sp_rx_position.z = 0;*/

	sp_rx_position.x = d_distances[0];
	sp_rx_position.y = d_distances[1];
	sp_rx_position.z = d_distances[2];

	return sp_rx_position;
}

