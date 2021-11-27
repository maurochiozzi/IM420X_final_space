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
#include "magnetic_field.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticQueue_t osStaticMessageQDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for identifyMagneti */
osThreadId_t identifyMagnetiHandle;
const osThreadAttr_t identifyMagneti_attributes = {
  .name = "identifyMagneti",
  .priority = (osPriority_t) osPriorityHigh3,
  .stack_size = 128 * 4
};
/* Definitions for estimatePositio */
osThreadId_t estimatePositioHandle;
const osThreadAttr_t estimatePositio_attributes = {
  .name = "estimatePositio",
  .priority = (osPriority_t) osPriorityAboveNormal3,
  .stack_size = 128 * 4
};
/* Definitions for sendData */
osThreadId_t sendDataHandle;
const osThreadAttr_t sendData_attributes = {
  .name = "sendData",
  .priority = (osPriority_t) osPriorityLow3,
  .stack_size = 128 * 4
};
/* Definitions for magneticFieldIntensitySampleQueue */
osMessageQueueId_t magneticFieldIntensitySampleQueueHandle;
uint8_t magneticFieldIntensitySampleQueueBuffer[ 128 * sizeof( MagneticField ) ];
osStaticMessageQDef_t magneticFieldIntensitySampleQueueControlBlock;
const osMessageQueueAttr_t magneticFieldIntensitySampleQueue_attributes = {
  .name = "magneticFieldIntensitySampleQueue",
  .cb_mem = &magneticFieldIntensitySampleQueueControlBlock,
  .cb_size = sizeof(magneticFieldIntensitySampleQueueControlBlock),
  .mq_mem = &magneticFieldIntensitySampleQueueBuffer,
  .mq_size = sizeof(magneticFieldIntensitySampleQueueBuffer)
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osMessageQueueId_t getMagneticFieldQueue(void);
/* USER CODE END FunctionPrototypes */

void startidentifyMagneticFieldTask(void *argument);
void startEstimatePosition(void *argument);
void startSendData(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of magneticFieldIntensitySampleQueue */
  magneticFieldIntensitySampleQueueHandle = osMessageQueueNew (128, sizeof(MagneticField), &magneticFieldIntensitySampleQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of identifyMagneti */
  identifyMagnetiHandle = osThreadNew(startidentifyMagneticFieldTask, NULL, &identifyMagneti_attributes);

  /* creation of estimatePositio */
  estimatePositioHandle = osThreadNew(startEstimatePosition, NULL, &estimatePositio_attributes);

  /* creation of sendData */
  sendDataHandle = osThreadNew(startSendData, NULL, &sendData_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
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
void startidentifyMagneticFieldTask(void *argument)
{
  /* USER CODE BEGIN startidentifyMagneticFieldTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
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
void startEstimatePosition(void *argument)
{
  /* USER CODE BEGIN startEstimatePosition */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END startEstimatePosition */
}

/* USER CODE BEGIN Header_startSendData */
/**
* @brief Function implementing the sendData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startSendData */
void startSendData(void *argument)
{
  /* USER CODE BEGIN startSendData */
  /* Infinite loop */
  for(;;)
  {
    osDelay(500);
  }
  /* USER CODE END startSendData */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
osMessageQueueId_t getMagneticFieldQueue(void){
	return magneticFieldIntensitySampleQueueHandle;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
