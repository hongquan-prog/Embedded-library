#include "fmc_lcd_driver.h"

void fmc_write_reg(volatile uint16_t cmd)
{
    cmd = cmd;
    LCD->reg = cmd;
}

void fmc_write_ram(volatile uint16_t data)
{
    data = data;
    LCD->ram = data;
}

uint16_t fmc_read_data(void)
{
    volatile uint16_t data;
    data = LCD->ram;
    return data;
}

void fmc_write_reg_and_ram(uint16_t reg, uint16_t data)
{
    LCD->reg = reg;
    LCD->ram = data;
}