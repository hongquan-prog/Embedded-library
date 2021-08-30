#pragma once

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct 
{
    void (*scl_write)(bool);
    void (*sda_write)(bool);
    bool (*sda_read)(void);
    void (*sda_set_input)(void);
    void (*sda_set_output)(void);
    void (*scl_set_output)(void);
}iic_driver_class_def;

typedef struct 
{
    iic_driver_class_def driver;
}iic_class_def;

void iic_start(iic_class_def* obj);
void iic_stop(iic_class_def* obj);
void iic_nack(iic_class_def* obj);
void iic_ack(iic_class_def* obj);
uint8_t iic_read_byte(iic_class_def* obj, uint8_t ack);
void iic_write_byte(iic_class_def* obj, uint8_t data);
bool iic_wait_ack(iic_class_def* obj);
bool iic_init(iic_class_def* obj, iic_driver_class_def* driver);