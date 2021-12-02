#include <my_lsm303dlhc.h>
#include <string.h>

typedef union {
	uint8_t ui8[6];
	int16_t i16[3];
} union_ui8_ui16;

int mock_value_index = 0;
double mocked_values_x[256] = { 0.0 };
double mocked_values_y[256] = { 0.0 };
double mocked_values_z[256] = { 14.1018982 ,  -2.0657993 ,  -3.34106226,   2.27404758,
	       -10.59071207,   2.52699216,   4.11077153,  -4.74067324,
	        11.16803319,   1.06706581,  -8.39797949,   1.87673398,
	        -8.39797949,   1.06706581,  11.16803319,  -4.74067324,
	         4.11077153,   2.52699216, -10.59071207,   2.27404758,
	        -3.34106226,  -2.0657993 ,  14.1018982 ,  -2.0657993 ,
	        -3.34106226,   2.27404758, -10.59071207,   2.52699216,
	         4.11077153,  -4.74067324,  11.16803319,   1.06706581,
	        -8.39797949,   1.87673398,  -8.39797949,   1.06706581,
	        11.16803319,  -4.74067324,   4.11077153,   2.52699216,
	       -10.59071207,   2.27404758,  -3.34106226,  -2.0657993 ,
	        14.1018982 ,  -2.0657993 ,  -3.34106226,   2.27404758,
	       -10.59071207,   2.52699216,   4.11077153,  -4.74067324,
	        11.16803319,   1.06706581,  -8.39797949,   1.87673398,
	        -8.39797949,   1.06706581,  11.16803319,  -4.74067324,
	         4.11077153,   2.52699216, -10.59071207,   2.27404758,
	        -3.34106226,  -2.0657993 ,  14.1018982 ,  -2.0657993 ,
	        -3.34106226,   2.27404758, -10.59071207,   2.52699216,
	         4.11077153,  -4.74067324,  11.16803319,   1.06706581,
	        -8.39797949,   1.87673398,  -8.39797949,   1.06706581,
	        11.16803319,  -4.74067324,   4.11077153,   2.52699216,
	       -10.59071207,   2.27404758,  -3.34106226,  -2.0657993 ,
	        14.1018982 ,  -2.0657993 ,  -3.34106226,   2.27404758,
	       -10.59071207,   2.52699216,   4.11077153,  -4.74067324,
	        11.16803319,   1.06706581,  -8.39797949,   1.87673398,
	        -8.39797949,   1.06706581,  11.16803319,  -4.74067324,
	         4.11077153,   2.52699216, -10.59071207,   2.27404758,
	        -3.34106226,  -2.0657993 };

HAL_StatusTypeDef initializeLSM303DHLC(I2C_HandleTypeDef *i2c) {
	HAL_StatusTypeDef ret;

	uint8_t ui_init_params[3][2] = { { CRA_REG_M, CRA_REG_M_CONFIG }, {
	CRB_REG_M, CRB_REG_M_CONFIG }, { MR_REG_M, MR_REG_M_CONFIG } };

	ret = HAL_I2C_IsDeviceReady(i2c, MY_LSM303DLHC_MAG_ADD, 10, 200);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[0],
			2, 200);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[1],
			2, 200);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[2],
			2, 200);
	if (ret != HAL_OK) {
		return ret;
	}

	return ret;
}

HAL_StatusTypeDef readRawMagnetometerData(I2C_HandleTypeDef *i2c,
		int16_t *i16_raw_response, uint8_t *ui8_buf_response) {
	HAL_StatusTypeDef ret;

	uint8_t reg = OUT_X_H_M;
	uint8_t ui_lsm_response[6];
	union_ui8_ui16 ui8_16_aux;

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, &reg, 1,
	HAL_MAX_DELAY);
	if (ret == HAL_OK) {
		ret = HAL_I2C_Master_Receive(i2c, MY_LSM303DLHC_MAG_ADD,
				ui_lsm_response, 6, HAL_MAX_DELAY);

		if (ret == HAL_OK) {
			ui8_16_aux.ui8[0] = ui_lsm_response[1];
			ui8_16_aux.ui8[1] = ui_lsm_response[0];

			ui8_16_aux.ui8[2] = ui_lsm_response[3];
			ui8_16_aux.ui8[3] = ui_lsm_response[2];

			ui8_16_aux.ui8[4] = ui_lsm_response[5];
			ui8_16_aux.ui8[5] = ui_lsm_response[4];

			memcpy(i16_raw_response, ui8_16_aux.i16, 3);
			memcpy(ui8_buf_response, ui8_16_aux.ui8, 6);
		}
	}

	return ret;
}

HAL_StatusTypeDef readMagnetometerData(I2C_HandleTypeDef *i2c,
		double *d_response) {
	HAL_StatusTypeDef ret;

	int16_t i16_raw_response[3];
	uint8_t ui8_raw_response[6];

	ret = readRawMagnetometerData(i2c, i16_raw_response, ui8_raw_response);
	if (ret == HAL_OK) {

#if MOCK_VALUES == 1
		d_response[0] = mocked_values_x[mock_value_index] * 0.000001;
		d_response[1] = mocked_values_y[mock_value_index] * 0.000001;
		d_response[2] = mocked_values_z[mock_value_index] * 0.000001;

		mock_value_index = (mock_value_index + 1) % MOCK_SIZE;
#else
		// X conversion
		d_response[0] = i16_raw_response[0] / (double) X_Y_GAIN;
		// Z conversion
		d_response[1] = i16_raw_response[1] / (double) Z_GAIN;

		// Y conversion
		d_response[2] = i16_raw_response[2] / (double) X_Y_GAIN;
#endif

	}

	return ret;
}

