#pragma once

#include <stdbool.h>
#include "iic.h"

#define GT9147_ID 9147

#define GT9147_CMD_WRREG 0x28
#define GT9147_CMD_RDREG 0x29

#define GT9147_CTRL_REG 0x8040
#define GT9147_CFGS_REG 0x8047
#define GT9147_CHECK_REG 0x80FF
#define GT9147_PID_REG 0x8140

#define GT9147_CSTID_REG 0x814E
#define GT9147_BUFFER_STATUS 0x80
#define GT9147_TOUCH_POINT 0xF
#define GT9147_TP1_REG 0x8150
#define GT9147_TP2_REG 0x8158
#define GT9147_TP3_REG 0x8160
#define GT9147_TP4_REG 0x8168
#define GT9147_TP5_REG 0x8170

typedef struct 
{
    iic_driver_class_def* iic_driver;
    void (*rst_write)(bool level);
    void (*int_set_input)(void);
    void (*int_set_output)(void);
    void (*int_write)(bool);
    bool (*pressed)(void);
    void (*delay)(uint32_t n);
}gt9147_driver_class_def;

typedef struct
{
    iic_class_def iic;
    gt9147_driver_class_def driver;
    int16_t lcd_x;
    int16_t lcd_y;
    uint8_t orient;
}gt9147_class_def;

extern gt9147_class_def g_gt9147;
extern gt9147_driver_class_def g_gt9147_driver;

bool gt9147_init(gt9147_class_def* obj, gt9147_driver_class_def* driver, uint8_t orient);
bool gt9147_read_coordinate(gt9147_class_def *obj);