#pragma once

#include <stdbool.h>
#include "fmc_lcd_driver.h"


#define ILI9341_ID 0x9341

/**
  * @brief  delay configure
  */
#define ili9341_delay(n) HAL_Delay(n)

/**
  * @brief  log configure
  */
#define ili9341_printf(...) // user_printf(__VA_ARGS__)

/* Width and Height in Portrait mode */
#define ILI9341_240X320_HEIGHT 320
#define ILI9341_240X320_WIDTH 240

/* Width and Height in Landscape mode */
#define ILI9341_320X240_HEIGHT 240
#define ILI9341_320X240_WIDTH 320

#define ILI9341_CMD_DISPOFF 0x28   /* Display off */
#define ILI9341_CMD_DISPON 0x29    /* Display on */
#define ILI9341_CMD_CASET 0x2A     /* Column address set */
#define ILI9341_CMD_RASET 0x2B     /* Row address set */
#define ILI9341_CMD_RAMWR 0x2C     /* Memory write */
#define ILI9341_CMD_MADCTL 0x36    /* Memory data access control */

bool ili9341_init(uint32_t orientation);
void ili9341_fill_rect(int x_begin, int y_begin, int x_end, int y_end, uint16_t* color);
void ili9341_display_off(void);
void ili9341_display_on(void);
void ili9341_draw_pixel(int x, int y, uint16_t color);
void ili9341_clear(uint32_t color);