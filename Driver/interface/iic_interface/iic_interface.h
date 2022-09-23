#pragma once

typedef void iic_interface_t;

typedef enum 
{
    IIC_PORT_IN,
    IIC_PORT_OUT,
    IIC_PORT_MODE_NUM
} iic_port_mode_t;

typedef struct 
{
    void (*scl_set_mode)(iic_port_mode_t mode);
    void (*sda_set_mode)(iic_port_mode_t mode);
    void (*scl_write)(unsigned char level);
    void (*sda_write)(unsigned char level);
    void (*delay)(void);
    unsigned char (*sda_read)(void);
} iic_hardware_drv_t;

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
