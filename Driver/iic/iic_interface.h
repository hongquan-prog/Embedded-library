#pragma once

typedef struct iic_interface
{
    void (*start)(void);
    void (*stop)(void);
    void (*write_ack)(void);
    void (*write_nack)(void);
    unsigned char (*wait_ack)(void);
    void (*write_byte)(unsigned char data);
    unsigned char (*read_byte)(unsigned char ack);
}iic_interface_t;