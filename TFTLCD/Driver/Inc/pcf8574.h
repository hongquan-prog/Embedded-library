#pragma once

#include "iic.h"
#include <stdbool.h>

#define PCF8574_SDA_GPIO_Port GPIOH
#define PCF8574_SDA_Pin GPIO_PIN_5
#define PCF8574_SCL_GPIO_Port GPIOH
#define PCF8574_SCL_Pin GPIO_PIN_4

typedef struct 
{
    iic_class_def iic;
}pcf8574_class_def;

extern pcf8574_class_def g_pcf8574;
extern iic_driver_class_def g_pcf8574_iic_driver;

uint8_t pcf8574_read_byte(pcf8574_class_def *obj);
void pcf8574_write_byte(pcf8574_class_def *obj, uint8_t data);
bool pcf8574_read_bit(pcf8574_class_def *obj, uint8_t pos);
void pcf8574_write_bit(pcf8574_class_def *obj, uint8_t pos, bool state);
bool pcf8574_init(pcf8574_class_def *obj, iic_driver_class_def* driver);