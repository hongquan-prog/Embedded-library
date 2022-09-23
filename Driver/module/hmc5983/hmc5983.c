#include "hmc5983.h"

#define ENABLE_DEBUG 0

#if ENABLE_DEBUG
#include "err.h"
#define HMC_LOG(e)   LOG(ERR_CONSTRUCT(InvalidParameter), e)
#else
#define HMC_LOG(e)
#endif

#define HMC_DEFAULT_ADDR 0x1E

static void hmc5983_read_byte(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char *buf)
{
    iic_write_read(obj->iic, HMC_DEFAULT_ADDR, &reg, 1, buf, 1);
}

static void hmc5983_read_bytes(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char *buf, int len)
{
    iic_write_read(obj->iic, HMC_DEFAULT_ADDR, &reg, 1, buf, len);
}

static void hmc5983_write_byte(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char val)
{
    unsigned char wdata[2] = { reg, val };
    
    iic_write_bytes(obj->iic, HMC_DEFAULT_ADDR, wdata, 2, 1);
}

unsigned char hmc5983_init(hmc5983_t *obj, iic_interface_t *iic, hmc_measurement_mode_t mode)
{
    unsigned char ret = 0;

    if (obj && iic)
    {
        ret = 1;
        obj->iic = iic;

        // set mode
        hmc5983_write_byte(obj, HMC_CRA, 0x70);
        hmc5983_write_byte(obj, HMC_CRB, 0xA0);
        hmc5983_set_measurement_mode(obj, mode);

        // get identifiction
        hmc5983_read_identification(obj);
    }
    else
    {
        HMC_LOG("please check obj and iic\r\n");
    }

    return ret;
}

static unsigned char hmc5983_set_parameter(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char begin, int len, unsigned char value)
{
    unsigned char ret = 0;
    unsigned char val = 0;
    unsigned char mask = ~(((1 << len) - 1) << begin);

    if (obj && (value < (1 << len)))
    {
        ret = 1;
        hmc5983_read_byte(obj, reg, &val);
        val &= mask;
        val |= (value << begin);
        hmc5983_write_byte(obj, reg, val);
    }
    else
    {
        HMC_LOG("please check obj and val\r\n");
    }

    return ret;
}

static unsigned char hmc5983_get_parameter(hmc5983_t *obj, hmc_reg_addr_t reg, unsigned char begin, int len, unsigned char *val)
{
    unsigned char ret = 0;
    unsigned char tmp = 0;

    if (obj && val)
    {
        ret = 1;
        hmc5983_read_byte(obj, reg, &tmp);
        *val = (tmp >> begin) & ((1 << len) - 1);
    }
    else
    {
        HMC_LOG("please check obj and val\r\n");
    }

    return ret;
}

unsigned char hmc5983_set_temperature_compensation(hmc5983_t *obj, unsigned char state)
{
    return hmc5983_set_parameter(obj, HMC_CRA, 7, 1, state);
}

unsigned char hmc5983_set_measurement_average(hmc5983_t *obj, hmc_measurement_average_t times)
{
    return hmc5983_set_parameter(obj, HMC_CRA, 5, 2, times);
}

unsigned char hmc5983_set_data_out_rate(hmc5983_t *obj, hmc_data_output_rate_t rate)
{
    return hmc5983_set_parameter(obj, HMC_CRA, 2, 3, rate);
}

unsigned char hmc5983_set_measurement_configuration(hmc5983_t *obj, hmc_measurement_configuration_t mode)
{
    return hmc5983_set_parameter(obj, HMC_CRA, 0, 2, mode);
}

unsigned char hmc5983_set_gain(hmc5983_t *obj, hmc_gain_t gain)
{
    return hmc5983_set_parameter(obj, HMC_CRB, 5, 3, gain);
}

unsigned char hmc5983_set_high_speed_mode(hmc5983_t *obj, unsigned char state)
{
    return hmc5983_set_parameter(obj, HMC_MR, 7, 1, state);
}

unsigned char hmc5983_set_low_power_mode(hmc5983_t *obj, unsigned char state)
{
    return hmc5983_set_parameter(obj, HMC_MR, 5, 1, state);
}

unsigned char hmc5983_set_measurement_mode(hmc5983_t *obj, hmc_measurement_mode_t mode)
{
    obj->mode = mode;

    return hmc5983_set_parameter(obj, HMC_MR, 0, 2, mode);
}

unsigned char hmc5983_get_result(hmc5983_t *obj, short *x, short *y, short *z)
{
    unsigned char ret = 0;
    unsigned char val[6];

    switch (obj->mode)
    {
    case HMC_SINGLE_MEASUREMENT:
        hmc5983_set_measurement_mode(obj, HMC_SINGLE_MEASUREMENT);
        if (hmc5983_is_ready(obj))
        {
            ret = 1;
            hmc5983_read_bytes(obj, HMC_DO_XH, val, 6);
            *x = val[0] << 8 | val[1];
            *y = val[2] << 8 | val[3];
            *z = val[4] << 8 | val[5];
        }
        break;
    case HMC_CONTINUOUS_MEASUREMENT:
        if (hmc5983_is_ready(obj))
        {
            ret = 1;
            hmc5983_read_bytes(obj, HMC_DO_XH, val, 6);
            *x = val[0] << 8 | val[1];
            *y = val[2] << 8 | val[3];
            *z = val[4] << 8 | val[5];
        }
        break;
    default:
        HMC_LOG("please check obj->mode\r\n");
        ret = 0;
        break;
    }

    return ret;
}

unsigned char hmc5983_is_over_written(hmc5983_t *obj)
{
    unsigned char ret = 0;

    hmc5983_get_parameter(obj, HMC_SR, 4, 1, &ret);

    return ret;
}

unsigned char hmc5983_is_lock(hmc5983_t *obj)
{
    unsigned char ret = 0;

    hmc5983_get_parameter(obj, HMC_SR, 1, 1, &ret);

    return ret;
}

unsigned char hmc5983_is_ready(hmc5983_t *obj)
{
    unsigned char ret = 0;

    hmc5983_get_parameter(obj, HMC_SR, 0, 1, &ret);

    return ret;
}

char *hmc5983_read_identification(hmc5983_t *obj)
{
    hmc5983_read_bytes(obj, HMC_IRA, (unsigned char *)obj->dentification, 3);

    return obj->dentification;
}

unsigned int hmc5983_read_temperature(hmc5983_t *obj)
{
    unsigned int ret = 0;
    unsigned char val = 0;

    hmc5983_read_byte(obj, HMC_TMP_H, (unsigned char *)&val);
    ret = val << 8;
    hmc5983_read_byte(obj, HMC_TMP_L, (unsigned char *)&val);
    ret |= val;

    return ret;
}

float hmc5983_temperature_convert(hmc5983_t *obj, unsigned int value)
{
    return value / 128.0 + 25.0;
}
