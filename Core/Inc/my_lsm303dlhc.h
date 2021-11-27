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
#define MR_REG_M  0x02

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
#define CRB_REG_M_CONFIG 0b00011100

/**
 * Define the gains for each axis according to the FS chosen
 *
 * Reference: [1] section 7.2.2, Table 75, p37
 */
#define X_Y_GAIN 230 / 100.0
#define Z_GAIN   205 / 100.0

/**
 * MR_REG_M Configuration
 *
 * COntinuous-conversion mode
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


#define MOCK_VALUES true

#if MOCK_VALUES

#define MOCK_SIZE 256

int mock_value_index = 0;
float mocked_values_x[256] = { 0.0 };
float mocked_values_y[256] = { 0.0 };
float mocked_values_z[256] = { 12.34112545, 2.57507389, 0.72302239, -7.72322543,
		-9.61628568, -1.27698236, 0.86771959, 10.78372983, 5.96604882,
		1.38178531, -4.75913645, -10.3241729, -4.23809424, 0.4775156,
		7.01781306, 9.85023043, 1.46283627, -1.42023032, -9.90338648,
		-7.15001301, 0.08193131, 3.06178613, 12.16535829, 2.39718437,
		1.17144619, -8.26096925, -9.20968256, -1.36453786, 0.54346786,
		11.48344932, 5.05082067, 2.27268906, -5.37521013, -10.17007194,
		-3.86308671, -0.35160386, 8.10315937, 8.77432857, 2.27056372,
		-1.78025962, -10.04288152, -6.59744201, -0.68907257, 3.80967829,
		11.65526212, 2.54553063, 1.38330787, -8.71060635, -8.72337825,
		-1.67432626, 0.52182109, 11.8808955, 4.36171751, 3.06301897,
		-6.02600967, -9.87695451, -3.67082224, -1.02441309, 9.1158465, 7.661341,
		3.21183596, -2.3218281, -10.02111606, -6.11960193, -1.51451804,
		4.74554149, 10.86076871, 3.0055915, 1.33786894, -9.02812312,
		-8.20497561, -2.17602333, 0.80489822, 11.93716357, 3.96619356,
		3.67541205, -6.64783029, -9.47351298, -3.68012099, -1.47505282,
		9.95674557, 6.62021471, 4.1945147, -2.99192324, -9.84022066,
		-5.76326709, -2.31360474, 5.77776692, 9.85964861, 3.73233303,
		1.03957726, -9.18243881, -7.7052195, -2.82051946, 1.36498969,
		11.6467456, 3.90296547, 4.04992299, -7.17980385, -8.99923902,
		-3.89007275, -1.65941129, 10.54354352, 5.7528624, 5.1224085,
		-3.72495147, -9.5179026, -5.56331802, -3.00811248, 6.80531315,
		8.74989844, 4.65461668, 0.51763172, -9.15844792, -7.27302954,
		-3.54472687, 2.14726984, 11.03806975, 4.17822243, 4.14989205,
		-7.56985707, -8.50055786, -4.28012598, -1.55944223, 10.81880048,
		5.14418654, 5.90468865, -4.44915889, -9.08571264, -5.53932714,
		-3.53005803, 7.7275968, 7.64014826, 5.68216291, -0.17687603,
		-8.95849885, -6.95071149, -4.27775511, 3.07516364, 10.17071743,
		4.76502038, 3.96553358, -7.77980883, -8.02628389, -4.81209954,
		-1.18493128, 10.75557238, 4.85376857, 6.46478012, -5.09365502,
		-8.58595653, -5.69364283, -3.8283497, 8.45433833, 6.63902816,
		6.71438833, -0.97596272, -8.60216401, -6.76981608, -4.94785025,
		4.05784238, 9.12959114, 5.60591945, 3.51489385, -7.78910759,
		-7.62284236, -5.43392016, -0.57253821, 10.36004843, 4.91003664,
		6.74785725, -5.59535209, -8.0675539, -6.0111596, -3.87378863, 8.9143992,
		5.84453475, 7.65025153, -1.80140819, -8.12432394, -6.74805062,
		-5.48941872, 4.99911462, 8.01660357, 6.61860657, 2.84208463,
		-7.59684311, -7.32972493, -6.0847197, 0.2177917, 9.67094527, 5.30748282,
		6.72621048, -5.90514049, -7.58124959, -6.4607967, -3.66192695,
		9.06274546, 5.33443859, 8.3981437, -2.57241208, -7.57175294,
		-6.88754566, -5.84944803, 5.80684208, 6.94070171, 7.70395288,
		2.01296516, -7.22183559, -7.17562398, -6.70079337, 1.10869545,
		8.75571712, 6.00720232, 6.40195875, -5.99269598, -7.17464646,
		-6.99854052, -3.21350315, 8.88485593, 5.15867143, 8.88485593,
		-3.21350315, -6.99854052, -7.17464646, -5.99269598, 6.40195875,
		6.00720232, 8.75571712, 1.10869545, -6.70079337, -7.17562398,
		-7.22183559, 2.01296516, 7.70395288, 6.94070171, 5.80684208,
		-5.84944803, -6.88754566, -7.57175294, -2.57241208, 8.3981437,
		5.33443859, 9.06274546, -3.66192695, -6.4607967, -7.58124959,
		-5.90514049, 6.72621048, 5.30748282, 9.67094527, 0.2177917, -6.0847197,
		-7.32972493, -7.59684311, 2.84208463 };
#endif

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
 * 							float *i16_response
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
		float *f_response);


#endif
