#include "ott2001a.h"
#include "gpio.h"

static void ott2001a_scl_write(bool level);
static void ott2001a_sda_write(bool level);
static void ott2001a_sda_set_input(void);
static void ott2001a_sda_set_output(void);
static void ott2001a_scl_set_output(void);
static bool ott2001a_sda_read(void);
static void ott2001a_rst_write(bool level);

ott2001a_class_def g_ott2001a;
iic_driver_class_def g_ott2001a_iic_driver = {
    .scl_write = ott2001a_scl_write,
    .sda_read = ott2001a_sda_read,
    .sda_write = ott2001a_sda_write,
    .sda_set_input = ott2001a_sda_set_input,
    .sda_set_output = ott2001a_sda_set_output,
    .scl_set_output = ott2001a_scl_set_output};
ott2001a_driver_class_def g_ott2001a_driver = {
    .iic_driver = &g_ott2001a_iic_driver,
    .rst_write = ott2001a_rst_write,
    .pressed = NULL,
    .delay = HAL_Delay};

/*
 * 配置IIC硬件驱动(可配置)
 */
static void ott2001a_rst_write(bool level)
{
    HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, level);
}

static void ott2001a_scl_write(bool level)
{
    HAL_GPIO_WritePin(T_SCL_GPIO_Port, T_SCL_Pin, level);
}

static void ott2001a_sda_write(bool level)
{
    HAL_GPIO_WritePin(T_SDA_GPIO_Port, T_SDA_Pin, level);
}

static void ott2001a_sda_set_input(void)
{
    gpio_set_mode(T_SDA_GPIO_Port, T_SDA_Pin, GPIO_MODE_INPUT);
}

static void ott2001a_sda_set_output(void)
{
    gpio_set_mode(T_SDA_GPIO_Port, T_SDA_Pin, GPIO_MODE_OUTPUT_PP);
}

static void ott2001a_scl_set_output(void)
{
    gpio_set_mode(T_SCL_GPIO_Port, T_SCL_Pin, GPIO_MODE_OUTPUT_PP);
}

static bool ott2001a_sda_read(void)
{
    return HAL_GPIO_ReadPin(T_SDA_GPIO_Port, T_SDA_Pin);
}

/*
 * ott2001a成员函数
 */
void ott2001a_read_reg(ott2001a_class_def* obj, uint16_t reg, uint8_t *value, uint8_t len)
{
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, OTT2001A_CMD_WRREG);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg >> 8);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg & 0xff);
    iic_wait_ack(&obj->iic);
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, OTT2001A_CMD_RDREG);
    for (int i = 0; i < len; i++)
    {
        *value = iic_read_byte(&obj->iic, i != (len - 1));
        value++;
    }
    iic_stop(&obj->iic);
}

bool ott2001a_write_reg(ott2001a_class_def* obj, uint16_t reg, uint8_t *value, uint8_t len)
{
    bool ret = true;

    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, OTT2001A_CMD_WRREG);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg >> 8);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg & 0xff);
    iic_wait_ack(&obj->iic);
    for (int i = 0; i < len; i++)
    {
        iic_write_byte(&obj->iic, value[i]);
        if (iic_wait_ack(&obj->iic) == false)
        {
            ret = false;
            break;
        }
    }
    iic_stop(&obj->iic);

    return ret;
}

void ott2001a_set_sensor_sate(ott2001a_class_def* obj, ott2001a_sensor_state_def state)
{
    uint8_t value = state;
    ott2001a_write_reg(obj, OTT2001A_CTRL_REG, &value, 1);
}

bool ott2001a_init(ott2001a_class_def* obj, ott2001a_driver_class_def* driver)
{
    bool ret = false;
    uint8_t reg = 0;
    if(obj && driver)
    {
        iic_init(&(obj->iic), driver->iic_driver);
        obj->driver.rst_write = driver->rst_write;
        obj->driver.pressed = driver->pressed;
        obj->driver.delay = driver->delay;

        obj->driver.rst_write(0);
        obj->driver.delay(20);
        obj->driver.rst_write(1);
        obj->driver.delay(20);
        ott2001a_set_sensor_sate(obj, OTT2001A_SENSOR_ON);
        ott2001a_read_reg(obj, OTT2001A_CTRL_REG, &reg, 1);
        if(reg == OTT2001A_SENSOR_ON)
        {
            ret = true;
        }
    }
    return ret;
}

bool ott2001a_read_coordinate(ott2001a_class_def* obj)
{
    bool ret = false;

    if (obj->driver.pressed())
    {
        ret = true;
        // obj->x = ott2001a_read_axis(XPT2046_AXIS_X);
        // obj->y = ott2001a_read_axis(XPT2046_AXIS_Y);
    }

    return ret;
}

void ott2001a_coordinate_convert(ott2001a_class_def* obj, uint8_t orient)
{

}