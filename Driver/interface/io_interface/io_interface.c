#include "io_interface.h"

void io_init(io_interface_t *obj, io_pin_def port, io_pull_def pull, io_mode_def mode)
{
    DEBUG_ASSET(((io_drv_t *)obj)->init);
    ((io_drv_t *)obj)->init(port, pull, mode);
}

io_status_def io_read(io_interface_t *obj, io_pin_def port)
{
    DEBUG_ASSET(((io_drv_t *)obj)->read);
    return ((io_drv_t *)obj)->read(port);
}

void io_write(io_interface_t *obj, io_pin_def port, io_status_def status)
{
    DEBUG_ASSET(((io_drv_t *)obj)->write);
    ((io_drv_t *)obj)->write(port, status);
}
