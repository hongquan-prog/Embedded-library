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
    void (*write_bytes)(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len, unsigned char stop);
    unsigned char (*read_byte)(iic_interface_t *obj, unsigned char ack);
    int (*read_bytes)(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len);
    int (*write_read)(iic_interface_t *obj, unsigned char addr, unsigned char *wdata, int wlen, unsigned char *rdata, int rlen);
} iic_vtable_t;

void iic_start(iic_interface_t *obj);
void iic_stop(iic_interface_t *obj);
void iic_write_ack(iic_interface_t *obj);
void iic_write_nack(iic_interface_t *obj);
unsigned char iic_wait_ack(iic_interface_t *obj);
void iic_write_byte(iic_interface_t *obj, unsigned char data);
void iic_write_bytes(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len, unsigned char stop);
unsigned char iic_read_byte(iic_interface_t *obj, unsigned char ack);
int iic_read_bytes(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len);
int iic_write_read(iic_interface_t *obj, unsigned char addr, unsigned char *wdata, int wlen, unsigned char *rdata, int rlen);
