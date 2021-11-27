#include <my_lsm303dlhc.h>
#include <string.h>

typedef union {
	uint8_t ui8[6];
	int16_t i16[3];
} union_ui8_ui16;

HAL_StatusTypeDef initializeLSM303DHLC(I2C_HandleTypeDef *i2c) {
	HAL_StatusTypeDef ret;

	uint8_t ui_init_params[3][2] = { { CRA_REG_M, CRA_REG_M_CONFIG }, {
	CRB_REG_M, CRB_REG_M_CONFIG }, { MR_REG_M, MR_REG_M_CONFIG } };

	ret = HAL_I2C_IsDeviceReady(i2c, MY_LSM303DLHC_MAG_ADD, 10,
	HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[0],
			2,
			HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[1],
			2,
			HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	ret = HAL_I2C_Master_Transmit(i2c, MY_LSM303DLHC_MAG_ADD, ui_init_params[2],
			2,
			HAL_MAX_DELAY);
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
		float *f_response) {
	HAL_StatusTypeDef ret;

	int16_t i16_raw_response[3];
	uint8_t ui8_raw_response[6];

	ret = readRawMagnetometerData(i2c, i16_raw_response, ui8_raw_response);
	if (ret == HAL_OK) {
		// X conversion
		f_response[0] = i16_raw_response[0] / (float) X_Y_GAIN;
		// Z conversion
		f_response[1] = i16_raw_response[1] / (float) Z_GAIN;

		// Y conversion
		f_response[2] = i16_raw_response[2] / (float) X_Y_GAIN;

	}

	return ret;
}

