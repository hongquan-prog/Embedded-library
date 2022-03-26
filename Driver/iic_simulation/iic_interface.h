#pragma once

typedef void iic_interface_t;

typedef struct
{
    void (*start)(iic_interface_t *obj);
    void (*stop)(iic_interface_t *obj);
    void (*write_ack)(iic_interface_t *obj);
    void (*write_nack)(iic_interface_t *obj);
    unsigned char (*wait_ack)(iic_interface_t *obj);
    void (*write_byte)(iic_interface_t *obj, unsigned char data);
    unsigned char (*read_byte)(iic_interface_t *obj, unsigned char ack);
} iic_vtable_t;

void iic_start(iic_interface_t *obj);
void iic_stop(iic_interface_t *obj);
void iic_write_ack(iic_interface_t *obj);
void iic_write_nack(iic_interface_t *obj);
unsigned char iic_wait_ack(iic_interface_t *obj);
void iic_write_byte(iic_interface_t *obj, unsigned char data);
unsigned char iic_read_byte(iic_interface_t *obj, unsigned char ack);
