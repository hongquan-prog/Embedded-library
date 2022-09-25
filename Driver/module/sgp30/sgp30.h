#pragma once

#include "err.h"
#include "iic_interface.h"

#define SGP30_CACHE_SIZE 10

typedef enum
{
    SGP30_SOFT_RESET = 0x0006,
    SGP30_IAQ_INIT = 0x2003,
    SGP30_MEASURE_IAQ = 0x2008,
    SGP30_GET_IAQ_BASELINE = 0x2015,
    SGP30_SET_IAQ_BASELINE = 0x201E,
    SGP30_SET_AH = 0x2061,
    SGP30_MEASURE_TEST10 = 0x2032,
    SGP30_GET_FEATURE_SET = 0x202f,
    SGP30_MEASURE_RAW = 0x2050,
    SGP30_GET_TVOC_INCEPTIVE_BASELINE = 0x20b3,
    SGP30_SET_TVOC_BASELINE = 0x2077,

    /*  The get serial ID command returns 3 words, and every word is
        followed by an 8-bit CRC checksum. Together the 3 words
        constitute a unique serial ID with a length of 48 bits
    */
    SGP30_ID = 0x3682,
} sgp30_cmd_def;

typedef struct
{
    /* unit: ppm */
    uint16_t eco2;
    /* unit: ppb */
    uint16_t tvoc;
} sgp30_iaq_t;

typedef struct
{
    uint16_t ethanol;
    uint16_t h2;
} sgp30_raw_t;

typedef struct
{
    unsigned short feature_set;
    /*  Save the data of writing and result of reading  */
    unsigned char cache[SGP30_CACHE_SIZE];
    /*  IAQ measure result  */
    sgp30_iaq_t iaq;
    /*  raw measure result  */
    sgp30_raw_t raw;

    iic_interface_t *iic;
} sgp30_t;

void sgp30_init(sgp30_t *obj, iic_interface_t *iic);
void sgp30_reset(sgp30_t *obj);
unsigned short sgp30_get_feature_set(sgp30_t *obj);
unsigned char sgp30_measure_iaq(sgp30_t *obj);
unsigned short sgp30_get_eco2(sgp30_t *obj);
unsigned short sgp30_get_tvoc(sgp30_t *obj);
unsigned char sgp30_measure_raw(sgp30_t *obj);
unsigned short sgp30_get_ethanol(sgp30_t *obj);
unsigned short sgp30_get_h2(sgp30_t *obj);
unsigned char sgp30_get_id(sgp30_t *obj, unsigned char *id);
unsigned char sgp30_get_iaq_baseline(sgp30_t *obj, unsigned short *eco2_base, unsigned short *tvoc_base);
void sgp30_set_iaq_baseline(sgp30_t *obj, unsigned short eco2_base, unsigned short tvoc_base);
void sgp30_set_absolute_humidity(sgp30_t *obj, unsigned int ah);
