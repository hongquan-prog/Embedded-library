#include "xpt2046.h"

static void xpt2046_cs_write(uint8_t level);
static void xpt2046_di_write(uint8_t level);
static void xpt2046_clk_write(uint8_t level);
static bool xpt2046_pressed(void);
static bool xpt2046_do_read(void);
static void xpt2046_hardware_init(void);

xpt2046_class_def g_xpt2046;
xpt2046_driver_class_def g_xpt2046_driver = {
    .cs_write = xpt2046_cs_write,
    .di_write = xpt2046_di_write,
    .clk_write = xpt2046_clk_write,
    .pressed = xpt2046_pressed,
    .do_read = xpt2046_do_read,
    .hardware_init = xpt2046_hardware_init};

/*
 * 配置硬件驱动(可配置)
 */
static void xpt2046_cs_write(uint8_t level)
{
    HAL_GPIO_WritePin(XPT2046_CS_GPIO_Port, XPT2046_CS_Pin, level);
}

static void xpt2046_di_write(uint8_t level)
{
    HAL_GPIO_WritePin(XPT2046_DI_GPIO_Port, XPT2046_DI_Pin, level);
}

static void xpt2046_clk_write(uint8_t level)
{
    HAL_GPIO_WritePin(XPT2046_CLK_GPIO_Port, XPT2046_CLK_Pin, level);
}

static bool xpt2046_pressed(void)
{
    return (HAL_GPIO_ReadPin(T_IT_GPIO_Port, T_IT_Pin) == 0);
}

static bool xpt2046_do_read(void)
{
    return HAL_GPIO_ReadPin(XPT2046_DO_GPIO_Port, XPT2046_DO_Pin);
}

static void xpt2046_hardware_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pins : PIPin PIPin */
    GPIO_InitStruct.Pin = XPT2046_CLK_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(XPT2046_CLK_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = XPT2046_DI_Pin | XPT2046_CS_Pin;
    HAL_GPIO_Init(XPT2046_DI_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = XPT2046_DO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(XPT2046_DO_GPIO_Port, &GPIO_InitStruct);
}

bool xpt2046_init(xpt2046_class_def *obj, xpt2046_driver_class_def *driver, uint8_t orient)
{
    bool ret = false;

    if (obj && driver)
    {
        obj->orient = orient;
        obj->driver.clk_write = driver->clk_write;
        obj->driver.cs_write = driver->cs_write;
        obj->driver.di_write = driver->di_write;
        obj->driver.do_read = driver->do_read;
        obj->driver.hardware_init = driver->hardware_init;
        obj->driver.pressed = driver->pressed;
        if(obj->driver.hardware_init)
        {
            obj->driver.hardware_init();
            ret = true;
        }
    }

    return ret;
}

/*
 * xpt2046成员函数
 */
bool xpt2046_is_pressed(xpt2046_class_def *obj)
{
    bool ret = false;
    if (obj)
    {
        ret = obj->driver.pressed();
    }
    return ret;
}

void xpt2046_write_byte(xpt2046_class_def *obj, uint8_t data)
{
    if(!obj)
    {
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        obj->driver.di_write((data << i) & 0x80);
        obj->driver.clk_write(0);
        obj->driver.clk_write(1);
    }
}

uint16_t xpt2046_read_data(xpt2046_class_def *obj)
{
    uint16_t ret = 0;
    if(!obj)
    {
        return ret;
    }
    for (int i = 12; i > 0; i--)
    {
        ret <<= 1;
        obj->driver.clk_write(0);
        obj->driver.clk_write(1);
        ret = ret + obj->driver.do_read();
    }
    return ret;
}

int16_t xpt2046_read_axis(xpt2046_class_def *obj, xpt2046_axis_def axis)
{
    int16_t ret = 0;
    if(!obj)
    {
        return ret;
    }
    obj->driver.clk_write(0);
    obj->driver.di_write(0);
    obj->driver.cs_write(0);

    if (axis)
    {
        xpt2046_write_byte(obj, 0x90);
    }
    else
    {
        xpt2046_write_byte(obj, 0xD0);
    }

    obj->driver.clk_write(0);
    obj->driver.clk_write(1);
    obj->driver.clk_write(0);
    ret = xpt2046_read_data(obj);
    obj->driver.cs_write(1);

    return ret;
}

bool xpt2046_read_coordinate(xpt2046_class_def *obj)
{
    if(!obj)
    {
        return false;
    }
    obj->lcd_x = xpt2046_read_axis(obj, XPT2046_AXIS_X);
    obj->lcd_y = xpt2046_read_axis(obj, XPT2046_AXIS_Y);

    obj->lcd_y = (obj->lcd_y - 260) / 11.54;
    obj->lcd_x = (obj->lcd_x - 440) / 13.22;

    obj->lcd_x = (obj->lcd_x < 0) ? (0) : (obj->lcd_x);
    obj->lcd_x = (obj->lcd_x > 239) ? (239) : (obj->lcd_x);
    obj->lcd_y = (obj->lcd_y < 0) ? (0) : (obj->lcd_y);
    obj->lcd_y = (obj->lcd_y > 319) ? (319) : (obj->lcd_y);

    if (obj->orient == LCD_ORIENTATION_PORTRAIT)
    {
    }
    else if (obj->orient == LCD_ORIENTATION_LANDSCAPE)
    {
        uint16_t temp = obj->lcd_x;
        obj->lcd_x = obj->lcd_y;
        obj->lcd_y = 239 - temp;
    }

    return true;
}

