#include "magnetic_field.h"

MagneticFieldSource mfNodes[] = {
		[0] = {
				.id=1,
				.frequency=50,
				.magnetic_moment=0.6997897635871264,
				.position={
						.x_position=0.2,
						.y_position=0.0,
						.z_position=0.0},
				.magneticFieldIntensity={
						.x_intensity=0.0,
						.y_intensity=0.0,
						.z_intensity=8.749898437499997 * 0.000001
				}
			},
		[1] = {
				.id=2,
				.frequency=70,
				.magnetic_moment=0.6997897635871264,
				.position={
						.x_position=0.0,
						.y_position=0.3,
						.z_position=0.0},
				.magneticFieldIntensity={
						.x_intensity=0.0,
						.y_intensity=0.0,
						.z_intensity=2.5925625 * 0.000001
				}
			},
		[2] = {
				.id=3,
				.frequency=90,
				.magnetic_moment=0.6997897635871264,
				.position={
						.x_position=0.4,
						.y_position=0.1,
						.z_position=0.0},
				.magneticFieldIntensity={
						.x_intensity=0.0,
						.y_intensity=0.0,
						.z_intensity=0.9986645113145853 * 0.000001
				}
			}
		};

void sampleMagneticField(HAL_StatusTypeDef (*readMagneticSensor)(I2C_HandleTypeDef*, float*), I2C_HandleTypeDef *i2c, osMessageQueueId_t queueId){
	MagneticField magneticField;

	float response[3];

	if(HAL_OK == readMagneticSensor(i2c, response)){

		magneticField.x_intensity = response[0];
		magneticField.y_intensity = response[1];
		magneticField.z_intensity = response[2];

		// ADD TO QUEUE
		osMessageQueuePut(queueId, &magneticField, 0, 0);
	}
}

void identifyMagneticField(osMessageQueueId_t queueId){

}
