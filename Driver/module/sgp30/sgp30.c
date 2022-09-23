#include "sgp30.h"

#define SGP30_DEFAULT_ADDR 0x58

static unsigned char crc8(unsigned char *data, int len);
static void sgp30_write_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len);
static unsigned char sgp30_read_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len);

void sgp30_init(sgp30_t *obj, iic_interface_t *iic)
{
    DEBUG_ASSERT(obj && iic);

    obj->iic = iic;

    /*  Sending an “sgp30_iaq_init” command starts the air quality measurement.
        After the “sgp30_iaq_init” command, a “sgp30_measure_iaq” command has
        to be sent in regular intervals of 1s to ensure proper operation of the
        dynamic baseline compensation algorithm.
    */
    sgp30_write_cmd(obj, SGP30_IAQ_INIT, NULL, 0);
}

static unsigned char crc8(unsigned char *data, int len)
{
    unsigned char crc = 0xff;
    const unsigned char polynomial = 0x31;

    for (int i = 0; i < len; i++)
    {
        crc ^= data[i];

        for (int b = 0; b < 8; b++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ polynomial;
            else
                crc <<= 1;
        }
    }

    return crc;
}

static void sgp30_write_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len)
{
    unsigned char i = 0;
    unsigned char pos = 0;

    if ((len * 3) <= SGP30_CACHE_SIZE)
    {
        obj->cache[0] = cmd >> 8;
        obj->cache[1] = cmd & 0xff;

        for (i = 0, pos = 2; i < len; ++i, pos += 3)
        {
            obj->cache[pos] = data[i] >> 8;
            obj->cache[pos + 1] = data[i] & 0xff;
            obj->cache[pos + 2] = crc8((unsigned char *)(data + i), 2);
        }

        iic_write_bytes(obj->iic, SGP30_DEFAULT_ADDR, obj->cache, pos, 1);
    }
}

static unsigned char sgp30_read_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len)
{
    unsigned char i = 0;
    unsigned char ret = 0;
    unsigned char pos = 0;

    if ((len * 3) <= SGP30_CACHE_SIZE)
    {
        obj->cache[0] = cmd >> 8;
        obj->cache[1] = cmd & 0xff;

        if (iic_write_read(obj->iic, SGP30_DEFAULT_ADDR, obj->cache, 2, obj->cache, len * 3))
        {
            for (i = 0, pos = 0; i < len; ++i, pos += 3)
            {
                if (obj->cache[pos + 2] == crc8(obj->cache + pos, 2))
                {
                    data[i] = (obj->cache[pos] << 8) + obj->cache[pos + 1];
                }
                else
                {
                    break;
                }
            }

            if (i == len)
            {
                ret = 1;
            }
        }
    }

    return ret;
}

unsigned char sgp30_measure_iaq(sgp30_t *obj)
{
    return sgp30_read_cmd(obj, SGP30_MEASURE_IAQ, (unsigned short *)&obj->iaq, 2);
}

sgp30_iaq_t sgp30_get_iaq(sgp30_t *obj)
{
    return obj->iaq;
}

unsigned char sgp30_measure_raw(sgp30_t *obj)
{
    return sgp30_read_cmd(obj, SGP30_MEASURE_RAW, (unsigned short *)&obj->raw, 2);
}

sgp30_raw_t sgp30_get_raw(sgp30_t *obj)
{
    return obj->raw;
}

unsigned char sgp30_get_id(sgp30_t *obj, unsigned char *id)
{
    return sgp30_read_cmd(obj, SGP30_ID, (unsigned short *)id, 3);
}

unsigned char sgp30_get_iaq_baseline(sgp30_t *obj, unsigned short *eco2_base, unsigned short *tvoc_base)
{
    unsigned char ret = 0;
    unsigned short rdata[2] = { 0 };

    ret = sgp30_read_cmd(obj, SGP30_GET_IAQ_BASELINE, rdata, 2);
    *eco2_base = rdata[0];
    *tvoc_base = rdata[1];

    return ret;
}

void sgp30_reset(sgp30_t *obj)
{
    sgp30_write_cmd(obj, SGP30_SOFT_RESET, NULL, 0);
}

void sgp30_set_iaq_baseline(sgp30_t *obj, unsigned short eco2_base, unsigned short tvoc_base)
{
    unsigned short wdata[2] = { tvoc_base, eco2_base };

    sgp30_write_cmd(obj, SGP30_SET_IAQ_BASELINE, wdata, 2);
}

void sgp30_set_absolute_humidity(sgp30_t *obj, unsigned int ah)
{
    unsigned short wdata = 0;

    if (ah < 256000)
    {
        wdata = (uint16_t)(((uint64_t)ah * 256 * 16777) >> 24);
        sgp30_write_cmd(obj->iic, SGP30_SET_AH, &wdata, 1);
    }
}