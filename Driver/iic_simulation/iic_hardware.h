#pragma once

typedef enum 
{
    IIC_PORT_IN,
    IIC_PORT_OUT
} iic_port_mode_t;

typedef struct 
{
    void (*scl_set_mode)(iic_port_mode_t mode);
    void (*sda_set_mode)(iic_port_mode_t mode);
    void (*scl_write)(unsigned char level);
    void (*sda_write)(unsigned char level);
    void (*delay)(void);
    unsigned char (*sda_read)(void);
} iic_hardware_interface_t;
