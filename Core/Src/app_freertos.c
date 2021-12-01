/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "my_lsm303dlhc.h"
#include "magnetic_field.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENSOR_SAMPLE_RATE (1.0 / 220.0) // 220 Hz
#define SAMPLE_SIZE 110
#define DATA_SAMPLE_PERIOD (SENSOR_SAMPLE_RATE * SAMPLE_SIZE)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
MagneticFieldSource mf_nodes[] = { [0] = { .id = 1, .f_frequency = 50,
		.d_magnetic_moment = 0.6997897635871264, .d_magnetic_cte =
				0.6997897635871264 * MAGNETIC_PERMEABILITY / (PI_4),
		.sp_position = { .x = 0.0, .y = 0.0, .z = 0.0 }, .mf_intensity = { .x =
				0.0, .y = 0.0, .z = 8.749898437499997 * 0.000001 } }, [1 ] = {
		.id = 3, .f_frequency = 90, .d_magnetic_moment = 0.6997897635871264,
		.d_magnetic_cte = 0.6997897635871264 * MAGNETIC_PERMEABILITY / (PI_4),
		.sp_position = { .x = 0.5, .y = 0.0, .z = 0.0 }, .mf_intensity = { .x =
				0.0, .y = 0.0, .z = 2.5925625 * 0.000001 } }, [2] = { .id = 2,
		.f_frequency = 70, .d_magnetic_moment = 0.6997897635871264,
		.d_magnetic_cte = 0.6997897635871264 * MAGNETIC_PERMEABILITY / (PI_4),
		.sp_position = { .x = 0.3, .y = 0.4, .z = 0.0 }, .mf_intensity = { .x =
				0.0, .y = 0.0, .z = 0.9986645113145853 * 0.000001 } } };

SpacePosition sp_rx_position = { .x = 0, .y = 0, .z = 0 };

double d_mf_x_samples[2][SAMPLE_SIZE];
double d_mf_y_samples[2][SAMPLE_SIZE];
double d_mf_z_samples[2][SAMPLE_SIZE];

uint8_t buffer_index = 0;
uint8_t buffer_index_to_retrive_data;

double d_mf_x_input[SAMPLE_SIZE];
double d_mf_y_input[SAMPLE_SIZE];
double d_mf_z_input[SAMPLE_SIZE];

MagneticField mf_samples_buff[SAMPLE_SIZE];

uint16_t ui16_sample_index = 0;

/* USER CODE END Variables */
/* Definitions for identifyMagneti */
osThreadId_t identifyMagnetiHandle;
const osThreadAttr_t identifyMagneti_attributes = { .name = "identifyMagneti",
		.priority = (osPriority_t) osPriorityHigh3, .stack_size = 128 * 4 };
/* Definitions for estimatePositio */
osThreadId_t estimatePositioHandle;
const osThreadAttr_t estimatePositio_attributes =
		{ .name = "estimatePositio", .priority =
				(osPriority_t) osPriorityAboveNormal3, .stack_size = 128 * 4 };
/* Definitions for sendData */
osThreadId_t sendDataHandle;
const osThreadAttr_t sendData_attributes = { .name = "sendData", .priority =
		(osPriority_t) osPriorityLow, .stack_size = 256 * 4 };
/* Definitions for my22HzTimer */
osTimerId_t my22HzTimerHandle;
const osTimerAttr_t my22HzTimer_attributes = { .name = "my22HzTimer" };
/* Definitions for spRXPositionMutex */
osMutexId_t spRXPositionMutexHandle;
const osMutexAttr_t spRXPositionMutex_attributes =
		{ .name = "spRXPositionMutex" };

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
__weak void initMagnetiFieldISR();
__weak void stopMagnetiFieldISR();

void sampleMagneticFieldISR(I2C_HandleTypeDef *i2c);
void sendPositionData();
/* USER CODE END FunctionPrototypes */

void startidentifyMagneticFieldTask(void *argument);
void startEstimatePosition(void *argument);
void startSendData(void *argument);
void my22HzTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */
	/* Create the mutex(es) */
	/* creation of spRXPositionMutex */
	spRXPositionMutexHandle = osMutexNew(&spRXPositionMutex_attributes);

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* Create the timer(s) */
	/* creation of my22HzTimer */
	my22HzTimerHandle = osTimerNew(my22HzTimerCallback, osTimerPeriodic, NULL,
			&my22HzTimer_attributes);

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	osTimerStart(my22HzTimerHandle, (uint16_t) (DATA_SAMPLE_PERIOD * 1000));

	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of identifyMagneti */
	identifyMagnetiHandle = osThreadNew(startidentifyMagneticFieldTask, NULL,
			&identifyMagneti_attributes);

	/* creation of estimatePositio */
	estimatePositioHandle = osThreadNew(startEstimatePosition, NULL,
			&estimatePositio_attributes);

	/* creation of sendData */
	sendDataHandle = osThreadNew(startSendData, NULL, &sendData_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	initMagnetiFieldISR();
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_startidentifyMagneticFieldTask */
/**
 * @brief  Function implementing the identifyMagneti thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_startidentifyMagneticFieldTask */
void startidentifyMagneticFieldTask(void *argument) {
	/* USER CODE BEGIN startidentifyMagneticFieldTask */

	/* Infinite loop */
	for (;;) {
		osThreadFlagsWait(0x0001U, osFlagsWaitAny, osWaitForever);

		buffer_index_to_retrive_data = (buffer_index + 1) % 2;

		memcpy(d_mf_x_input, d_mf_x_samples, SAMPLE_SIZE);
		memcpy(d_mf_y_input, d_mf_y_samples, SAMPLE_SIZE);
		memcpy(d_mf_z_input, d_mf_z_samples, SAMPLE_SIZE);
	}
	/* USER CODE END startidentifyMagneticFieldTask */
}

/* USER CODE BEGIN Header_startEstimatePosition */
/**
 * @brief Function implementing the estimatePositio thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startEstimatePosition */
void startEstimatePosition(void *argument) {
	/* USER CODE BEGIN startEstimatePosition */

	/* Infinite loop */
	for (;;) {
		osThreadFlagsWait(0x0001U, osFlagsWaitAny, osWaitForever);
		if (osMutexAcquire(spRXPositionMutexHandle, osWaitForever) == osOK) {
			sp_rx_position = estimatePoisition(mf_nodes);

			osMutexRelease(spRXPositionMutexHandle);
		}
	}

	osThreadTerminate(NULL);
	/* USER CODE END startEstimatePosition */
}

/* USER CODE BEGIN Header_startSendData */
/**
 * @brief Function implementing the sendData thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_startSendData */
void startSendData(void *argument) {
	/* USER CODE BEGIN startSendData */
	/* Infinite loop */
	for (;;) {
		osThreadFlagsWait(0x0001U, osFlagsWaitAny, osWaitForever);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

	}
//		if (osMutexAcquire(spRXPositionMutexHandle, osWaitForever) == osOK) {
////				printf("EsPos: x=%.2f y=%.2f z=%.2f\r\n", spRXPosition.x, spRXPosition.y,
////						spRXPosition.z);
//			osMutexRelease(spRXPositionMutexHandle);
//		}
//
//		osDelay(581);
//	}

	osThreadTerminate(NULL);
	/* USER CODE END startSendData */
}

/* my22HzTimerCallback function */
void my22HzTimerCallback(void *argument) {
	/* USER CODE BEGIN my22HzTimerCallback */
	osThreadFlagsSet(estimatePositioHandle, 0x0001U);
	osThreadFlagsSet(sendDataHandle, 0x0001U);
	/* USER CODE END my22HzTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void sampleMagneticFieldISR(I2C_HandleTypeDef *i2c) {
	MagneticField mf_sample;

//	if (osMessageQueueGetSpace(magneticFieldIntensitySampleQueueHandle) > 0) {
//		mfSample = sampleMagneticField(readMagnetometerData, i2c);
//		osMessageQueuePut(magneticFieldIntensitySampleQueueHandle, &mfSample, 0,
//				0);

	if (ui16_sample_index == SAMPLE_SIZE) {
		buffer_index = (buffer_index + 1) % 2;
		ui16_sample_index = 0;

		osThreadFlagsSet(identifyMagnetiHandle, 0x0001U);
	}

	mf_sample = sampleMagneticField(readMagnetometerData, i2c);

	d_mf_x_samples[buffer_index][ui16_sample_index] = mf_sample.x;
	d_mf_y_samples[buffer_index][ui16_sample_index] = mf_sample.y;
	d_mf_z_samples[buffer_index][ui16_sample_index] = mf_sample.z;

	ui16_sample_index++;

}

void sendPositionData() {

}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
