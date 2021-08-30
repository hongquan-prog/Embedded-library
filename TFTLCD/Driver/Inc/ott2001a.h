#pragma once

#include "iic.h"
#include <stdbool.h>

#define OTT2001A_MAX_X 2700
#define OTT2001A_MAX_Y 1500

#define OTT2001A_CMD_RDREG 0xB3
#define OTT2001A_CMD_WRREG 0xB2

#define OTT2001A_CSTID_REG 0x0000
#define OTT2001A_TP1_REG 0x0100
#define OTT2001A_TP2_REG 0x0500
#define OTT2001A_TP3_REG 0x1000
#define OTT2001A_TP4_REG 0x1400
#define OTT2001A_TP5_REG 0x1800
#define OTT2001A_RESOLUTION_REG 0x0900
#define OTT2001A_CTRL_REG 0x0D00

typedef enum
{
    OTT2001A_SENSOR_OFF = 0x00,
    OTT2001A_SENSOR_ON = 0x80,
}ott2001a_sensor_state_def;

typedef struct 
{
    iic_driver_class_def* iic_driver;
    void (*rst_write)(bool level);
    bool (*pressed)(void);
    void (*delay)(uint32_t n);
}ott2001a_driver_class_def;

typedef struct
{
    iic_class_def iic;
    ott2001a_driver_class_def driver;
    int16_t touch_x;
    int16_t touch_y;
    int16_t lcd_x;
    int16_t lcd_y;
}ott2001a_class_def;

bool ott2001a_init(ott2001a_class_def* obj, ott2001a_driver_class_def* driver);
void ott2001a_set_sensor_sate(ott2001a_class_def* obj, ott2001a_sensor_state_def state);
bool ott2001a_read_coordinate(ott2001a_class_def* obj);
void ott2001a_coordinate_convert(ott2001a_class_def* obj, uint8_t orient);