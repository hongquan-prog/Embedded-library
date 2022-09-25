#include "iic_hardware.h"
#include "gpio.h"

static uint32_t mode_table[IIC_PORT_MODE_NUM] = {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP};

static void scl_set_mode(iic_port_mode_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(mode < IIC_PORT_MODE_NUM)
    {
        GPIO_InitStruct.Pin = IIC_SCL_Pin;
        GPIO_InitStruct.Mode = mode_table[mode];
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(IIC_SCL_GPIO_Port, &GPIO_InitStruct);
    }
}

static void sda_set_mode(iic_port_mode_t mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(mode < IIC_PORT_MODE_NUM)
    {
        GPIO_InitStruct.Pin = IIC_SDA_Pin;
        GPIO_InitStruct.Mode = mode_table[mode];
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(IIC_SDA_GPIO_Port, &GPIO_InitStruct);
    }
}

static void scl_write(unsigned char level)
{
    HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, (level) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET));
}

static void sda_write(unsigned char level)
{
    HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, (level) ? (GPIO_PIN_SET) : (GPIO_PIN_RESET));
}

static unsigned char sda_read(void)
{
    return HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin);
}

static void delay(void)
{
    int i = 0; 

    for(i = 0; i < 100; i++)
    {
        (void)i;
    }
}

iic_hardware_drv_t *iic_hardware_get_drv(void)
{
    static iic_hardware_drv_t drv = {
        .delay = delay,
        .scl_set_mode = scl_set_mode,
        .scl_write = scl_write,
        .sda_read = sda_read,
        .sda_set_mode = sda_set_mode,
        .sda_write = sda_write
    };

    return &drv;
}