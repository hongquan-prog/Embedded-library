#pragma once 

#include <stdbool.h>
#include <stdint.h>
#include "fmc_lcd_driver.h"
#include "main.h"

#define XPT2046_CLK_GPIO_Port GPIOH
#define XPT2046_CLK_Pin GPIO_PIN_6
#define XPT2046_DO_GPIO_Port GPIOG
#define XPT2046_DO_Pin GPIO_PIN_3
#define XPT2046_DI_GPIO_Port GPIOI
#define XPT2046_DI_Pin GPIO_PIN_3
#define XPT2046_CS_GPIO_Port GPIOI
#define XPT2046_CS_Pin GPIO_PIN_8

typedef enum
{
    XPT2046_AXIS_X,
    XPT2046_AXIS_Y
}xpt2046_axis_def;

typedef struct
{
    void (*cs_write)(uint8_t level);
    void (*di_write)(uint8_t level);
    void (*clk_write)(uint8_t level);
    bool (*do_read)(void);
    bool (*pressed)(void);
    void (*hardware_init)(void);
}xpt2046_driver_class_def;

typedef struct 
{
    xpt2046_driver_class_def driver;
    uint8_t orient;
    int16_t lcd_x;
    int16_t lcd_y;
}xpt2046_class_def;

extern xpt2046_class_def g_xpt2046;
extern xpt2046_driver_class_def g_xpt2046_driver;

bool xpt2046_init(xpt2046_class_def* obj, xpt2046_driver_class_def* driver, uint8_t orient);
bool xpt2046_is_pressed(xpt2046_class_def* obj);
bool xpt2046_read_coordinate(xpt2046_class_def* obj);
