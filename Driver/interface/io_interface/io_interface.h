#pragma once

#include "err.h"

typedef void io_interface_t;

typedef enum
{
    IO_RESET,
    IO_SET
}io_status_def;

typedef enum
{
    IO_INPUT,
    IO_OUTPUT
}io_mode_def;

typedef enum
{
    IO_PULL_NO,
    IO_PULL_UP,
    IO_PULL_DOWN
}io_pull_def;

typedef enum
{
    IO_1,
    IO_2,
    IO_3,
    IO_4,
    IO_NUM
}io_pin_def;



typedef struct
{
    void (*init)(io_pin_def port, io_pull_def pull, io_mode_def mode);
    io_status_def (*read)(io_pin_def port);
    void (*write)(io_pin_def port, io_status_def status);
}io_drv_t;

void io_init(io_interface_t *obj, io_pin_def port, io_pull_def pull, io_mode_def mode);
io_status_def io_read(io_interface_t *obj, io_pin_def port);
void io_write(io_interface_t *obj, io_pin_def port, io_status_def status);

