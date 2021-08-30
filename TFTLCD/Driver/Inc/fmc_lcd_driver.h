#pragma once

#include "fmc.h"

typedef struct
{
  uint16_t reg;
  uint16_t ram;
} fmc_reg_def;

#define LCD ((fmc_reg_def *)(0x60000000 | 0x7FFFE))
#define LCD_RAM_BASE 0x60080000
/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define LCD_ORIENTATION_PORTRAIT (0x00)  /* Portrait orientation choice of LCD screen  */
#define LCD_ORIENTATION_LANDSCAPE (0x01) /* Landscape orientation choice of LCD screen */

#define LCD_ILI9341 (0x00)
#define LCD_NT35510 (0x01)

/* RGB565 color value*/
#define LCD_WHITE 0xFFFF
#define LCD_BLACK 0x0000
#define LCD_BLUE 0x001F
#define LCD_BRED 0XF81F
#define LCD_GRED 0XFFE0
#define LCD_GBLUE 0X07FF
#define LCD_RED 0xF800
#define LCD_MAGENTA 0xF81F
#define LCD_GREEN 0x07E0
#define LCD_CYAN 0x7FFF
#define LCD_YELLOW 0xFFE0
#define LCD_BROWN 0XBC40
#define LCD_BRRED 0XFC07
#define LCD_GRAY 0X8430

void fmc_write_reg(volatile uint16_t cmd);
void fmc_write_ram(volatile uint16_t data);
uint16_t fmc_read_data(void);
void fmc_write_reg_and_ram(uint16_t reg, uint16_t data);