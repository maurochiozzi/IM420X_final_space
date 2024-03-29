#include "stm32g4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dft.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <complex.h>
#include <stddef.h>


#ifndef __MAGNETIC_FIELD__
#define __MAGNETIC_FIELD__

#define MAGNETIC_PERMEABILITY 1.257 * 0.000001
#define PI_4 4 * M_PI

#define SENSOR_SAMPLE_RATE 220.0
#define SENSOR_SAMPLE_PERIOD (1.0 / SENSOR_SAMPLE_RATE) // 220 Hz
#define SAMPLE_SIZE 110
#define DATA_SAMPLE_PERIOD (SENSOR_SAMPLE_PERIOD * SAMPLE_SIZE)

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
	int i_frequency;
	double d_magnetic_moment;
	double d_magnetic_cte;
	SpacePosition sp_position;
	MagneticField mf_intensity;
} MagneticFieldSource;

MagneticField sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, double*),
		I2C_HandleTypeDef *i2c);

void identifyMagneticField(double complex *d_mf_x_dft, double complex *d_mf_y_dft, double complex *d_mf_z_dft,
		MagneticFieldSource *mf_nodes);

double getDistanceFromRSS(MagneticFieldSource node);

SpacePosition estimatePoisition(MagneticFieldSource *nodes);

#endif
