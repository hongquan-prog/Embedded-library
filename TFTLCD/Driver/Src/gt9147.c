#include "gt9147.h"
#include "gpio.h"
#include "cmsis_os.h"
#include "fmc_lcd_driver.h"

static void gt9147_scl_write(bool level);
static void gt9147_sda_write(bool level);
static void gt9147_sda_set_input(void);
static void gt9147_sda_set_output(void);
static void gt9147_scl_set_output(void);
static bool gt9147_sda_read(void);
static void gt9147_rst_write(bool level);
static void gt9147_int_set_output(void);
static void gt9147_int_set_input(void);
static void gt9147_int_write(bool level);

gt9147_class_def g_gt9147;
iic_driver_class_def g_gt9147_iic_driver = {
    .scl_write = gt9147_scl_write,
    .sda_read = gt9147_sda_read,
    .sda_write = gt9147_sda_write,
    .sda_set_input = gt9147_sda_set_input,
    .sda_set_output = gt9147_sda_set_output,
    .scl_set_output = gt9147_scl_set_output};
gt9147_driver_class_def g_gt9147_driver = {
    .iic_driver = &g_gt9147_iic_driver,
    .rst_write = gt9147_rst_write,
    .int_set_input = gt9147_int_set_input,
    .int_set_output = gt9147_int_set_output,
    .int_write = gt9147_int_write,
    .pressed = NULL,
    .delay = vTaskDelay};

static const uint8_t s_gt9147_cfg[] = {
    0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
	0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
	0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
	0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
	0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
	0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
	0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
	0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
	0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
	0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
	0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
	0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
	0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
	0XFF,0XFF,0XFF,0XFF};

/*
 * 配置IIC硬件驱动(可配置)
 */

static void gt9147_scl_write(bool level)
{
    HAL_GPIO_WritePin(T_SCL_GPIO_Port, T_SCL_Pin, level);
}

static void gt9147_sda_write(bool level)
{
    HAL_GPIO_WritePin(T_SDA_GPIO_Port, T_SDA_Pin, level);
}

static void gt9147_sda_set_input(void)
{
    gpio_set_mode(T_SDA_GPIO_Port, T_SDA_Pin, GPIO_MODE_INPUT);
}

static void gt9147_sda_set_output(void)
{
    gpio_set_mode(T_SDA_GPIO_Port, T_SDA_Pin, GPIO_MODE_OUTPUT_PP);
}

static void gt9147_scl_set_output(void)
{
    gpio_set_mode(T_SCL_GPIO_Port, T_SCL_Pin, GPIO_MODE_OUTPUT_PP);
}

static bool gt9147_sda_read(void)
{
    return HAL_GPIO_ReadPin(T_SDA_GPIO_Port, T_SDA_Pin);
}

/*
 * gt9147驱动
 */
static void gt9147_rst_write(bool level)
{
    HAL_GPIO_WritePin(T_RST_GPIO_Port, T_RST_Pin, level);
}

static void gt9147_int_set_output(void)
{
    gpio_set_mode(T_IT_GPIO_Port, T_IT_Pin, GPIO_MODE_OUTPUT_PP);
}

static void gt9147_int_set_input(void)
{
    gpio_set_mode(T_IT_GPIO_Port, T_IT_Pin, GPIO_MODE_INPUT);
}

static void gt9147_int_write(bool level)
{
    HAL_GPIO_WritePin(T_IT_GPIO_Port, T_IT_Pin, level);
}
/*
 * gt9147成员函数
 */
void gt9147_read_reg(gt9147_class_def* obj, uint16_t reg, uint8_t *value, uint8_t len)
{
    if(!obj)
    {
        return;
    }
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, GT9147_CMD_WRREG);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg >> 8);
    iic_wait_ack(&obj->iic);
    iic_write_byte(&obj->iic, reg & 0xff);
    iic_wait_ack(&obj->iic);
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, GT9147_CMD_RDREG);
    iic_wait_ack(&obj->iic);
    for (int i = 0; i < len; i++)
    {
        value[i] = iic_read_byte(&obj->iic, i != (len - 1));
    }
    iic_stop(&obj->iic);
}

bool gt9147_write_reg(gt9147_class_def* obj, uint16_t reg, const uint8_t *value, uint8_t len)
{
    bool ret = true;
    if(!obj)
    {
        return false;
    }
    iic_start(&obj->iic);
    iic_write_byte(&obj->iic, GT9147_CMD_WRREG);
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

uint16_t gt9147_read_id(gt9147_class_def* obj)
{
    uint32_t ret = 0;
    uint8_t temp[4];

    if(!obj)
    {
        return ret;
    }
    gt9147_read_reg(obj, GT9147_PID_REG, temp, 4);
    for(int i = 0; i < 4; i++)
    {
        ret *= 10;
        ret += temp[i] - '0';
    }

    return ret;
}

void gt9147_write_cfg(gt9147_class_def* obj)
{
    uint8_t crc[2] = {0, 1};

    if(!obj)
    {
        return;
    }

    for(int i = 0; i < sizeof(s_gt9147_cfg); i++)
    {
        crc[0] += s_gt9147_cfg[i];
    }
    crc[0] = (~crc[0]) + 1;
    gt9147_write_reg(obj, GT9147_CFGS_REG, s_gt9147_cfg, sizeof(s_gt9147_cfg));
    gt9147_write_reg(obj, GT9147_CHECK_REG, crc, sizeof(crc));
}

bool gt9147_init(gt9147_class_def* obj, gt9147_driver_class_def* driver, uint8_t orient)
{
    bool ret = false;
    if(obj && driver)
    {
        obj->orient = orient;
        iic_init(&(obj->iic), driver->iic_driver);
        obj->driver.rst_write = driver->rst_write;
        obj->driver.int_set_input = driver->int_set_input;
        obj->driver.int_set_output = driver->int_set_output;
        obj->driver.int_write = driver->int_write;
        obj->driver.pressed = driver->pressed;
        obj->driver.delay = driver->delay;

        // 若用地址 0x28/0x29，int则输出高；若用地址 0xBA/0xBB，int则输出低
        obj->driver.int_set_output();
        obj->driver.rst_write(0);
        obj->driver.int_write(0);
        obj->driver.delay(1);
        obj->driver.int_write(1);
        obj->driver.delay(1);
        obj->driver.rst_write(1);
        obj->driver.delay(10);
        obj->driver.int_set_input();
        obj->driver.delay(60);
        ret = (GT9147_ID == gt9147_read_id(obj));
        if(ret)
        {
            uint8_t value = 2;
            gt9147_write_reg(obj, GT9147_CTRL_REG, &value, 1);
            gt9147_read_reg(obj, GT9147_CTRL_REG, &value, 1);
            if(value < 0x60)
            {
                gt9147_write_cfg(obj);
            }
            obj->driver.delay(10);
            value = 0;
            gt9147_write_reg(obj, GT9147_CTRL_REG, &value, 1);
        }
    }

    return ret;
}

bool gt9147_read_coordinate(gt9147_class_def *obj)
{
    bool ret = false;
    uint8_t mode = 0;
    uint8_t pos[4];
    uint8_t temp = 0;

    if(!obj)
    {
        return false;
    }
    
    gt9147_read_reg(obj, GT9147_CSTID_REG, &mode, 1);
    if((mode & GT9147_BUFFER_STATUS))
    {
        ret = true;
        temp = 0;
        gt9147_read_reg(obj, GT9147_TP1_REG, pos, 4);
        gt9147_write_reg(obj, GT9147_CSTID_REG, &temp, 1);

        obj->lcd_x = ((pos[3] << 8) + pos[2]);
        (obj->lcd_x >= 800) ? (obj->lcd_x = 799) : (obj->lcd_x = obj->lcd_x);
        (obj->lcd_x < 0) ? (obj->lcd_x = 0) : (obj->lcd_x = obj->lcd_x);
        obj->lcd_y = 480 - ((pos[1] << 8) + pos[0]);
        (obj->lcd_y >= 480) ? (obj->lcd_y = 479) : (obj->lcd_y = obj->lcd_y);
        (obj->lcd_y < 0) ? (obj->lcd_y = 0) : (obj->lcd_y = obj->lcd_y);
        if(obj->orient == LCD_ORIENTATION_LANDSCAPE)
        {
        }
        else if(obj->orient == LCD_ORIENTATION_PORTRAIT)
        {
            int16_t pos = obj->lcd_x;
            obj->lcd_x = 480 - obj->lcd_y;
            obj->lcd_y = pos;
        }
    }
    
    return ret;
}