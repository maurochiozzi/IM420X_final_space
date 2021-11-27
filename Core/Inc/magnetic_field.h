#include "stm32g4xx_hal.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"

#ifndef __MAGNETIC_FIELD__
#define __MAGNETIC_FIELD__

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
	int id;
	float frequency;
	float magnetic_moment;
	SpacePosition position;
	MagneticField magneticFieldIntensity;
} MagneticFieldSource;

void sampleMagneticField(
		HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*),
		I2C_HandleTypeDef *i2c, osMessageQueueId_t queueId);

void identifyMagneticField(osMessageQueueId_t queueId);

float getDistanceFromRSS(MagneticField node);

SpacePosition estimatePoisition(MagneticFieldSource *nodes, int qnty_nodes);

#endif
