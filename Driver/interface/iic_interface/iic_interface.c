#include "err.h"
#include "iic_interface.h"

void iic_start(iic_interface_t *obj)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->start);
    (*((iic_vtable_t **)obj))->start(obj);
}
void iic_stop(iic_interface_t *obj)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->stop);
    (*((iic_vtable_t **)obj))->stop(obj);
}
void iic_write_ack(iic_interface_t *obj)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->write_ack);
    (*((iic_vtable_t **)obj))->write_ack(obj);
}
void iic_write_nack(iic_interface_t *obj)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->write_nack);
    (*((iic_vtable_t **)obj))->write_nack(obj);
}
unsigned char iic_wait_ack(iic_interface_t *obj)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->wait_ack);
    return (*((iic_vtable_t **)obj))->wait_ack(obj);
}
void iic_write_byte(iic_interface_t *obj, unsigned char data)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->write_byte);
    (*((iic_vtable_t **)obj))->write_byte(obj, data);
}
unsigned char iic_read_byte(iic_interface_t *obj, unsigned char ack)
{
    NULL_ASSET((*((iic_vtable_t **)obj))->read_byte);
    return (*((iic_vtable_t **)obj))->read_byte(obj, ack);
}
