#include "magnetic_field.h"

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c) {
	MagneticField mfSample;

	float response[3];

	if (HAL_OK == readMagneticSensor(i2c, response)) {

		mfSample.x_intensity = response[0];
		mfSample.y_intensity = response[1];
		mfSample.z_intensity = response[2];
	}

	return mfSample;
}

MagneticFieldSource* identifyMagneticField(MagneticField *mfSamples) {
	MagneticFieldSource mfSources[3];

	// perform dft

	// identify fields and intensity

	return mfSources;
}

double getDistanceFromRSS(MagneticFieldSource node) {
	double distance = cbrt(
			node.magnetic_cte / node.magneticFieldIntensity.z_intensity);

	return distance;
}

SpacePosition estimatePoisition(MagneticFieldSource *nodes, int qnty_nodes) {
	SpacePosition spRXPosition;

	double fDistances[3];

	for (int node_index = 0; node_index < qnty_nodes; node_index++) {
		fDistances[node_index] = getDistanceFromRSS(nodes[node_index]);

	}

	return spRXPosition;
}

