/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define T_RST_Pin GPIO_PIN_8
#define T_RST_GPIO_Port GPIOI
#define SD_CHECK_Pin GPIO_PIN_0
#define SD_CHECK_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_1
#define LED0_GPIO_Port GPIOB
#define T_SCL_Pin GPIO_PIN_6
#define T_SCL_GPIO_Port GPIOH
#define T_IT_Pin GPIO_PIN_7
#define T_IT_GPIO_Port GPIOH
#define T_SDA_Pin GPIO_PIN_3
#define T_SDA_GPIO_Port GPIOI
#define LCD_BL_Pin GPIO_PIN_5
#define LCD_BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define PCF8574_BEEP_Pin 0
#define PCF8574_AP3216_INT_Pin 1
#define PCF8574_DCMI_PWR_Pin 2
#define PCF8574_USB_PWR_Pin 3
#define PCF8574_Pin_4 4
#define PCF8574_MPU9250_INT_Pin 5
#define PCF8574_RS485__RE_Pin 6
#define PCF8574_ETH_NRST_Pin 7
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
