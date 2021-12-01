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
	double x;
	double y;
	double z;
} MagneticField;

typedef struct {
	float x;
	float y;
	float z;
} SpacePosition;

typedef struct {
	float x;
	float y;
	float z;
} DistanceVector;

typedef struct {
	int id;
	float f_frequency;
	double d_magnetic_moment;
	double d_magnetic_cte;
	SpacePosition sp_position;
	MagneticField mf_intensity;
} MagneticFieldSource;

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c);

void identifyMagneticField(double *d_mf_x_samples, double *d_mf_y_samples,
		double *d_mf_z_samples, MagneticFieldSource *mf_nodes,
		uint16_t ui16_sample_size);

double getDistanceFromRSS(MagneticFieldSource node);

SpacePosition estimatePoisition(MagneticFieldSource *nodes);

#endif
