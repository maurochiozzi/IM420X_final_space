#include "stm32g4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#ifndef __MAGNETIC_FIELD__
#define __MAGNETIC_FIELD__

#define MAGNETIC_PERMEABILITY 1.257 * 0.000001
#define PI_4 4 * M_PI

typedef struct {
	float x_intensity;
	float y_intensity;
	float z_intensity;
} MagneticField;

typedef struct {
	float x_position;
	float y_position;
	float z_position;
} SpacePosition;

typedef struct {
	float x;
	float y;
	float z;
} DistanceVector;


typedef struct {
	int id;
	float frequency;
	double magnetic_moment;
	double magnetic_cte;
	SpacePosition position;
	MagneticField magneticFieldIntensity;
} MagneticFieldSource;

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c);

MagneticFieldSource* identifyMagneticField(MagneticField *mfSamples);

double getDistanceFromRSS(MagneticFieldSource node);

SpacePosition estimatePoisition(MagneticFieldSource *nodes, int qnty_nodes);

#endif
