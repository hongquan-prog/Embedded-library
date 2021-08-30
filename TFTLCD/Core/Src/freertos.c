/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
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
#include "lvgl.h"
#include "lv_port_indev.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "lv_demo.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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
/* Definitions for lwipInitTask */
osThreadId_t lwipInitTaskHandle;
const osThreadAttr_t lwipInitTask_attributes = {
  .name = "lwipInitTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for lvglTask */
osThreadId_t lvglTaskHandle;
const osThreadAttr_t lvglTask_attributes = {
  .name = "lvglTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 512 * 4
};
/* Definitions for lv_tick_timer */
osTimerId_t lv_tick_timerHandle;
const osTimerAttr_t lv_tick_timer_attributes = {
  .name = "lv_tick_timer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void lwipInitTaskEntry(void *argument);
void lvglTaskEntry(void *argument);
void lv_tick_callback(void *argument);

extern void MX_LWIP_Init(void);
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

  /* Create the timer(s) */
  /* creation of lv_tick_timer */
  lv_tick_timerHandle = osTimerNew(lv_tick_callback, osTimerPeriodic, NULL, &lv_tick_timer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of lwipInitTask */
  lwipInitTaskHandle = osThreadNew(lwipInitTaskEntry, NULL, &lwipInitTask_attributes);

  /* creation of lvglTask */
  lvglTaskHandle = osThreadNew(lvglTaskEntry, NULL, &lvglTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_lwipInitTaskEntry */
/**
  * @brief  Function implementing the lwipInitTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_lwipInitTaskEntry */
void lwipInitTaskEntry(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN lwipInitTaskEntry */
  osThreadExit();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END lwipInitTaskEntry */
}

/* USER CODE BEGIN Header_lvglTaskEntry */
/**
* @brief Function implementing the lvglTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lvglTaskEntry */
void lvglTaskEntry(void *argument)
{
  /* USER CODE BEGIN lvglTaskEntry */
  osTimerStart(lv_tick_timerHandle,5);
  lv_init();
  lv_port_fs_init();
  lv_port_disp_init();
  lv_port_indev_init();
  lv_demo_widgets();
  /* Infinite loop */
  for(;;)
  {
    lv_timer_handler();
    osDelay(5);
  }
  /* USER CODE END lvglTaskEntry */
}

/* lv_tick_callback function */
void lv_tick_callback(void *argument)
{
  /* USER CODE BEGIN lv_tick_callback */
  lv_tick_inc(5);
  /* USER CODE END lv_tick_callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
