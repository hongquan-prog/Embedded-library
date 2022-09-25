#pragma once

#include "iic_interface.h"

typedef struct 
{
    iic_vtable_t *vtabel;
    iic_hardware_drv_t *hardware;
}iic_sim_interface_t;

void iic_sim_init(iic_sim_interface_t *obj, iic_hardware_drv_t *hardware);
void iic_sim_start(iic_interface_t *obj);
void iic_sim_stop(iic_interface_t *obj);
void iic_sim_write_ack(iic_interface_t *obj);
void iic_sim_write_nack(iic_interface_t *obj);
unsigned char iic_sim_wait_ack(iic_interface_t *obj);
void iic_sim_write_byte(iic_interface_t *obj, unsigned char data);
int iic_sim_write_bytes(iic_interface_t *p, unsigned char addr, unsigned char *data, int len, unsigned char stop);
unsigned char iic_sim_read_byte(iic_interface_t *obj, unsigned char ack);
int iic_sim_read_bytes(iic_interface_t *p, unsigned char addr, unsigned char *data, int len);
int iic_sim_write_read(iic_interface_t *p, unsigned char addr, unsigned char *wdata, int wlen, unsigned char *rdata, int rlen);

