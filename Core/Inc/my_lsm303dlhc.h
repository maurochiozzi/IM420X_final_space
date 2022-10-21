/**
 * File name: 			my_lsm303dlhc.h
 * File description:	This library implements the necessary functions to work
 * 						with the LSM303DHLC Magnetometer. Other resources such
 *						as Temperature and Linear Accelerometer, will not be used.
 *
 * 							-> Temperature sensor off
 * 							-> +- 8.1 G for FullScale
 * 							-> 220 Hz operation mode
 * 							-> Continuous convert mode
 *
 * Author name:			Mauro Chiozzi
 *
 * Reference: [1] - https://pdf1.alldatasheet.com/datasheet-pdf/view/505234/STMICROELECTRONICS/LSM303DLHC.html
 *
 */

#include "stm32g4xx_hal.h"

#ifndef __MY_LSM303DLHC_H__
#define __MY_LSM303DLHC_H__

/**
 * I2C slave address for interface
 *
 * Reference: [1] section 5.1.3, p21
 */
#define MY_LSM303DLHC_MAG_ADD (0x1E << 1)

/**
 * Standard timeout in milliseconds that will be used for each request
 */
#define LSM303_DLHC_TIMEOUT HAL_MAX_DELAY

/**
 * Registers for initial configurations.
 *
 * Reference: [1] section 6, Table 17, p22-23
 */
#define CRA_REG_M 0x00
#define CRB_REG_M 0x01
#define MR_REG_M 0x02

/**
 * Operation values for configuration.
 */

/**
 * CRA_REG_M Configuration
 *
 * Temperature off and data output rate bits to 220 Hz
 *
 * Reference: [1] section 7.2.1, Table 72, p36
 */
#define CRA_REG_M_CONFIG 0b00011100

/**
 * CRB_REG_M Configuration
 *
 * Fullscale to +- 8.1 G
 *
 * Reference: [1] section 7.2.2, Table 75, p37
 */
#define CRB_REG_M_CONFIG 0b11100000

/**
 * Define the gains for each axis according to the FS chosen.
 *
 * LSB / Gauss
 *
 * 100.0 is the conversion from gauss to micro tesla.
 *
 * Reference: [1] section 7.2.2, Table 75, p37
 */
#define X_Y_GAIN 230 / 100.0
#define Z_GAIN 205 / 100.0

/**
 * MR_REG_M Configuration
 *
 * Continuous-conversion mode
 *
 * Reference: [1] section 7.2.3, Table 78, p37-38
 */
#define MR_REG_M_CONFIG 0b00000000

/**
 * Registers to request data read. Note since LSM303DHCL has auto increment
 * feature to reduce the communication between master and slave, it's not
 * required to define data output registers (0x04 to 0x08), only 0x03.
 *
 * Reference: [1] section 5.1.3, p21
 */
#define OUT_X_H_M 0x03
#define OUT_X_L_M 0x04
#define OUT_Z_H_M 0x05
#define OUT_Z_L_M 0x06
#define OUT_Y_H_M 0x07
#define OUT_Y_L_M 0x08

#define MOCK_VALUES 0
#define MOCK_SIZE 256

/**
 * Method name: 			initializeLSM303DHLC
 * Method description:		is responsible to init the LSM303DHLC magnetometer
 * 							with preset configurations of operation:
 * 								-> Temperature sensor off
 * 								-> +- 8.1 G for FullScale
 * 								-> 220 Hz operation mode
 * 								-> Continuous convert mode
 *
 * Input params:			I2C_HandleTypeDef *i2c
 * 							The I2C pointer which the LSM303DHLC is connected to
 *
 * Output params:			HAL_statusTypeDef
 * 							Returns the status of the communication and
 * 							initialization attempt
 */
HAL_StatusTypeDef initializeLSM303DHLC(I2C_HandleTypeDef *i2c);

/**
 * Method name: 			readRawMagnetometerData
 * Method description:		responsible to read the magnetic fields intensity in
 * 							X Y and Z axis and return its value in raw format
 *
 * Input params:			I2C_HandleTypeDef *i2c
 * 							The I2C pointer which the LSM303DHLC is connected to
 *
 * 							int16_t *i16_response
 * 							The array composed by three int16 elements to store
 * 							the read value.
 *
 * 							Index 0 -> value in X
 * 							Index 1 -> value in Z
 * 							Index 2 -> value in Y
 *
 * 							Reference: [1] section 6, Table 17, p22-23
 *
 * Output params:			HAL_statusTypeDef
 * 							Returns the status of the communication and
 * 							initialization attempt
 */
HAL_StatusTypeDef readRawMagnetometerData(I2C_HandleTypeDef *i2c,
										  int16_t *i16_raw_response, uint8_t *ui8_buf_response);

/**
 * Method name: 			readMagnetometerData
 * Method description:		responsible to read the magnetic fields intensity in
 * 							X Y and Z axis and return its value
 *
 * Input params:			I2C_HandleTypeDef *i2c
 * 							The I2C pointer which the LSM303DHLC is connected to
 *
 * 							double *d_response
 * 							The array composed by three float elements to store
 * 							the read value.
 *
 * 							Index 0 -> value in X
 * 							Index 1 -> value in Z
 * 							Index 2 -> value in Y
 *
 * 							Reference: [1] section 6, Table 17, p22-23
 *
 * Output params:			HAL_statusTypeDef
 * 							Returns the status of the communication and
 * 							initialization attempt
 */
HAL_StatusTypeDef readMagnetometerData(I2C_HandleTypeDef *i2c,
		float *d_response);

#endif
