#pragma once

#include "nt35510.h"
#include "ili9341.h"
#include "sdram.h"
#include "xpt2046.h"

#define LV_TOUCH_XPT2046 0 
#define LV_TOUCH_GT9147 1

#define LV_DISPLAY  LCD_ILI9341
#define LV_TOUCH  LV_TOUCH_XPT2046
#define LV_ORIENTATION LCD_ORIENTATION_LANDSCAPE

#if (LV_DISPLAY == LCD_ILI9341)

#if (LV_ORIENTATION == LCD_ORIENTATION_LANDSCAPE)
#define LCD_WIDTH ILI9341_320X240_WIDTH
#define LCD_HEIGHT ILI9341_320X240_HEIGHT
#elif (LV_ORIENTATION == LCD_ORIENTATION_PORTRAIT)
#define LCD_WIDTH ILI9341_240X320_WIDTH
#define LCD_HEIGHT ILI9341_240X320_HEIGHT
#endif
#define LCD_BUF1 (SDRAM_BASE_ADDR + LV_MEM_SIZE)
#define LCD_BUF2 (LCD_BUF1 + (2 * LCD_HEIGHT * LCD_HEIGHT))

#elif (LV_DISPLAY == LCD_NT35510)

#if (LV_ORIENTATION == LCD_ORIENTATION_LANDSCAPE)
#define LCD_WIDTH NT35510_800X480_WIDTH
#define LCD_HEIGHT NT35510_800X480_HEIGHT
#elif (LV_ORIENTATION == LCD_ORIENTATION_PORTRAIT)
#define LCD_WIDTH NT35510_480X800_WIDTH
#define LCD_HEIGHT NT35510_480X800_HEIGHT
#endif
#define LCD_BUF1 (SDRAM_BASE_ADDR + LV_MEM_SIZE)
#define LCD_BUF2 (LCD_BUF1 + (2 * LCD_WIDTH * LCD_HEIGHT))

#endif