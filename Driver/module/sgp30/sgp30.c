#include "sgp30.h"

#define SGP30_DEFAULT_ADDR 0x58
#define SGP30_PRODUCT_TYPE 0

static volatile int s_delay_cnt = 0;
static unsigned char crc8(unsigned char *data, int len);
static void sgp30_write_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len);
static unsigned char sgp30_read_cmd(sgp30_t *obj, sgp30_cmd_def cmd, unsigned short *data, unsigned char len);

/*  tvoc and eco2 range
    tvoc:   > 750ppb    danger
    eco2:   < 350       outdoor
            350~450     acceptable
            450~700     uncomfortable
            700~1000    felt tired
            1000~2000   bad for your health
            2000~5000   headache, sick, heart racing
            > 5000      depletion of oxygen
 */

void sgp30_init(sgp30_t *obj, iic_interface_t *iic)
{
    DEBUG_ASSERT(obj && iic);

    obj->iic = iic;

    /*  reset */
    sgp30_write_cmd(obj, SGP30_SOFT_RESET, NULL, 0);

    /*  read feature set */
    sgp30_read_cmd(obj, SGP30_GET_FEATURE_SET, &obj->feature_set, 1);

    /*  Sending an “sgp30_iaq_init” command starts the air quality measurement.
        After the “sgp30_iaq_init” command, a “sgp30_measure_iaq” command has
        to be sent in regular intervals of 1s to ensure proper operation of the
        dynamic baseline compensation algorithm.
    */
    sgp30_write_cmd(obj, SGP30_IAQ_INIT, NULL, 0);
}

void sgp30_measure_delay(void)
{
    int i = 0;

    for(i = 0; i < 500000; i++)
    {
        asm volatile("");
    }
}

static unsigned char crc8(unsigned char *data, int len)
{
    unsigned char crc = 0xff;
    const unsigned char polynomial = 0x31;

    for (int i = 0; i < len; i++)
    {
        crc ^= data[i];

        for (int j = 0; j < 8; j++)
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

        if (iic_write_read(obj->iic, SGP30_DEFAULT_ADDR, obj->cache, 2, obj->cache, len * 3) == (len * 3))
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

unsigned short sgp30_get_feature_set(sgp30_t *obj)
{
    return obj->feature_set;
}

unsigned char sgp30_measure_iaq(sgp30_t *obj)
{
    int i = 0;
    unsigned char ret = 0;
    unsigned char pos = 0;
    unsigned short data[2] = {0};

    if (SGP30_CACHE_SIZE > 6)
    {
        obj->cache[0] = SGP30_MEASURE_IAQ >> 8;
        obj->cache[1] = SGP30_MEASURE_IAQ & 0xff;

        if (2 == iic_write_bytes(obj->iic, SGP30_DEFAULT_ADDR, obj->cache, 2, 1))
        {
            sgp30_measure_delay();

            if (6 == iic_read_bytes(obj->iic, SGP30_DEFAULT_ADDR, obj->cache, 6))
            {
                for (i = 0, pos = 0; i < 2; ++i, pos += 3)
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

                if (i == 2)
                {
                    ret = 1;
                    obj->iaq.eco2 = data[0];
                    obj->iaq.tvoc = data[1];
                }
            }
        }
    }

    return ret;
}

unsigned short sgp30_get_eco2(sgp30_t *obj)
{
    return obj->iaq.eco2;
}

unsigned short sgp30_get_tvoc(sgp30_t *obj)
{
    return obj->iaq.tvoc;
}


unsigned char sgp30_measure_raw(sgp30_t *obj)
{
    return sgp30_read_cmd(obj, SGP30_MEASURE_RAW, (unsigned short *)&obj->raw, 2);
}

unsigned short sgp30_get_ethanol(sgp30_t *obj)
{
    return obj->raw.ethanol;
}

unsigned short sgp30_get_h2(sgp30_t *obj)
{
    return obj->raw.h2;
}

unsigned char sgp30_get_id(sgp30_t *obj, unsigned char *id)
{
    return sgp30_read_cmd(obj, SGP30_ID, (unsigned short *)id, 3);
}

unsigned char sgp30_get_iaq_baseline(sgp30_t *obj, unsigned short *eco2_base, unsigned short *tvoc_base)
{
    unsigned char ret = 0;
    unsigned short rdata[2] = {0};

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
    unsigned short wdata[2] = {tvoc_base, eco2_base};

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