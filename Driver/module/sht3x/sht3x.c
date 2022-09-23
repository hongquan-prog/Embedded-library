#include "sht3x.h"
#include "err.h"

#define SHT3X_DEFAULT_ADDR 0x44
#define SHT3X_READ ((SHT3X_DEFAULT_ADDR << 1) | 1)
#define SHT3X_WRITE (SHT3X_DEFAULT_ADDR << 1)

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
    iic_start(obj->iic);
    iic_write_byte(obj->iic, SHT3X_WRITE);
    iic_wait_ack(obj->iic);
    iic_write_byte(obj->iic, cmd >> 8);
    iic_wait_ack(obj->iic);
    iic_write_byte(obj->iic, cmd & 0xff);
    iic_wait_ack(obj->iic);
    iic_stop(obj->iic);
}

static void sht3x_read_bytes(sht3x_t *obj, sht3x_cmd_def cmd, unsigned char *buf, int len)
{
    int i = 0;

    iic_start(obj->iic);
    iic_write_byte(obj->iic, SHT3X_WRITE);
    iic_wait_ack(obj->iic);
    iic_write_byte(obj->iic, cmd >> 8);
    iic_wait_ack(obj->iic);
    iic_write_byte(obj->iic, cmd & 0xff);
    iic_wait_ack(obj->iic);
    iic_start(obj->iic);
    iic_write_byte(obj->iic, SHT3X_READ);
    iic_wait_ack(obj->iic);

    for (i = 0; i < len - 1; i++)
    {
        buf[i] = iic_read_byte(obj->iic, 1);
    }
    buf[i] = iic_read_byte(obj->iic, 0);
    iic_stop(obj->iic);
}

void sht3x_reset(sht3x_t *obj)
{
    sht3x_write_cmd(obj, SHT3X_SOFT_RESET);
}

sht3x_ret_t sht3x_read_humudity_tempture(sht3x_t *obj)
{
    unsigned char data[6] = {0};
    sht3x_ret_t ret = {0};

    sht3x_read_bytes(obj, SHT3X_MEDIUM_REPEAT_STRETCH, data, 6);

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

    sht3x_read_bytes(obj, SHT3X_READ_STATUS, data, 2);
    *((unsigned short *)(&ret)) = (data[0] << 8) | data[1];

    return ret;
}
