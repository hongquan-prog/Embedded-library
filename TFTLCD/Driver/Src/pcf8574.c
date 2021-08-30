#include "pcf8574.h"
#include "gpio.h"

static void pcf8574_scl_write(bool level);
static bool pcf8574_sda_read(void);
static void pcf8574_sda_write(bool level);
static void pcf8574_sda_set_input(void);
static void pcf8574_sda_set_output(void);
static void pcf8574_scl_set_output(void);

pcf8574_class_def g_pcf8574;
iic_driver_class_def g_pcf8574_iic_driver = {
    .sda_read = pcf8574_sda_read,
    .scl_write = pcf8574_scl_write,
    .sda_write = pcf8574_sda_write,
    .sda_set_input = pcf8574_sda_set_input,
    .sda_set_output = pcf8574_sda_set_output,
    .scl_set_output = pcf8574_scl_set_output};

/*
 * 配置IIC硬件驱动
 */

static void pcf8574_scl_write(bool level)
{
    HAL_GPIO_WritePin(PCF8574_SCL_GPIO_Port, PCF8574_SCL_Pin, level);
}

static bool pcf8574_sda_read(void)
{
    return HAL_GPIO_ReadPin(PCF8574_SDA_GPIO_Port, PCF8574_SDA_Pin);
}

static void pcf8574_sda_write(bool level)
{
    HAL_GPIO_WritePin(PCF8574_SDA_GPIO_Port, PCF8574_SDA_Pin, level);
}

static void pcf8574_sda_set_input(void)
{
    gpio_set_mode(PCF8574_SDA_GPIO_Port, PCF8574_SDA_Pin, GPIO_MODE_INPUT);
}

static void pcf8574_sda_set_output(void)
{
    gpio_set_mode(PCF8574_SDA_GPIO_Port, PCF8574_SDA_Pin, GPIO_MODE_OUTPUT_PP);
}

static void pcf8574_scl_set_output(void)
{
    gpio_set_mode(PCF8574_SCL_GPIO_Port, PCF8574_SCL_Pin, GPIO_MODE_OUTPUT_PP);
}

/*
 * pcf8574成员函数
 */

uint8_t pcf8574_read_byte(pcf8574_class_def *obj)
{
    uint8_t ret = 0;

    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, 0x41);
    iic_wait_ack(&obj->iic);
    ret = iic_read_byte(&obj->iic, 0);
    iic_stop(&obj->iic);

    return ret;
}

void pcf8574_write_byte(pcf8574_class_def *obj, uint8_t data)
{
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, 0x40);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, data);
    iic_wait_ack(&obj->iic);
    iic_stop(&obj->iic);
}

bool pcf8574_read_bit(pcf8574_class_def *obj, uint8_t pos)
{
    return (pcf8574_read_byte(obj) & (1 << pos));
}

void pcf8574_write_bit(pcf8574_class_def *obj, uint8_t pos, bool state)
{
    uint8_t ret = pcf8574_read_byte(obj);

    (state) ? (ret |= (1 << pos)) : (ret &= ~(1 << pos));
    pcf8574_write_byte(obj, ret);
}

bool pcf8574_init(pcf8574_class_def *obj, iic_driver_class_def *driver)
{
    bool ret = false;
    if (obj && driver)
    {
        iic_init(&(obj->iic), driver);
        pcf8574_write_byte(obj, 0xff);
        ret = true;
    }
    return ret;
}