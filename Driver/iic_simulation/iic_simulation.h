#pragma once

#include "iic_interface.h"
#include "iic_hardware.h"

typedef struct 
{
    iic_vtable_t *vtabel;
    iic_hardware_interface_t *hardware;
}iic_sim_interface_t;

void iic_sim_init(iic_sim_interface_t *obj, iic_hardware_interface_t *hardware);
void iic_sim_start(iic_interface_t *obj);
void iic_sim_stop(iic_interface_t *obj);
void iic_sim_write_ack(iic_interface_t *obj);
void iic_sim_write_nack(iic_interface_t *obj);
unsigned char iic_sim_wait_ack(iic_interface_t *obj);
void iic_sim_write_byte(iic_interface_t *obj, unsigned char data);
unsigned char iic_sim_read_byte(iic_interface_t *obj, unsigned char ack);
