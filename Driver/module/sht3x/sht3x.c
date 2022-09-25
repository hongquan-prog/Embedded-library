#include "sht3x.h"
#include "err.h"

#define SHT3X_DEFAULT_ADDR 0x44

void sht3x_init(sht3x_t *obj, iic_interface_t *iic)
{
    DEBUG_ASSERT(obj && iic);

    obj->iic = iic;
    sht3x_reset(obj);
    sht3x_clear_status(obj);
}

static unsigned char crc8(const unsigned char *data, int len)
{
    unsigned char crc = 0xFF;
    const unsigned char POLYNOMIAL = 0x31;

    for (int i = 0; i < len; ++i)
    {
        crc ^= *data++;

        for (int j = 0; j < 8; j++)
        {
            crc = (crc & 0x80) ? ((crc << 1) ^ POLYNOMIAL) : (crc << 1);
        }
    }

    return crc;
}

static void sht3x_write_cmd(sht3x_t *obj, sht3x_cmd_def cmd)
{
    unsigned char wdata[2] = {(cmd >> 8), (cmd & 0xff)};

    iic_write_bytes(obj->iic, SHT3X_DEFAULT_ADDR, wdata, 2, 1);
}

static void sht3x_read_cmd(sht3x_t *obj, sht3x_cmd_def cmd, unsigned char *buf, int len)
{
    unsigned char wdata[2] = {(cmd >> 8), (cmd & 0xff)};

    iic_write_read(obj->iic, SHT3X_DEFAULT_ADDR, wdata, 2, buf, len);
}

void sht3x_reset(sht3x_t *obj)
{
    sht3x_write_cmd(obj, SHT3X_SOFT_RESET);
}

sht3x_ret_t sht3x_read_humudity_tempture(sht3x_t *obj)
{
    unsigned char data[6] = {0};
    sht3x_ret_t ret = {0};

    sht3x_read_cmd(obj, SHT3X_MEDIUM_REPEAT_STRETCH, data, 6);

    if ((data[2] == crc8(data, 2)) && (data[5] == crc8(data + 3, 2)))
    {
        ret.tempture = ((data[0] << 8) + data[1]);
        ret.tempture *= 175;
        ret.tempture /= 0xffff;
        ret.tempture = -45 + ret.tempture;

        ret.humidity = ((data[3] << 8) + data[4]);
        ret.humidity *= 100;
        ret.humidity /= 0xFFFF;
    }

    return ret;
}

void sht3x_clear_status(sht3x_t *obj)
{
    sht3x_write_cmd(obj, SHT3X_CLEAR_STATUS);
}

sht3x_status_t sht3x_read_status(sht3x_t *obj)
{
    unsigned char data[2] = {0};
    sht3x_status_t ret = {0};

    sht3x_read_cmd(obj, SHT3X_READ_STATUS, data, 2);
    *((unsigned short *)(&ret)) = (data[0] << 8) | data[1];

    return ret;
}
