#include "err.h"
#include "iic_interface.h"

void iic_start(iic_interface_t *obj)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->start);
    (*((iic_vtable_t **)obj))->start(obj);
}
void iic_stop(iic_interface_t *obj)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->stop);
    (*((iic_vtable_t **)obj))->stop(obj);
}
void iic_write_ack(iic_interface_t *obj)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->write_ack);
    (*((iic_vtable_t **)obj))->write_ack(obj);
}
void iic_write_nack(iic_interface_t *obj)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->write_nack);
    (*((iic_vtable_t **)obj))->write_nack(obj);
}
unsigned char iic_wait_ack(iic_interface_t *obj)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->wait_ack);
    return (*((iic_vtable_t **)obj))->wait_ack(obj);
}
void iic_write_byte(iic_interface_t *obj, unsigned char data)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->write_byte);
    (*((iic_vtable_t **)obj))->write_byte(obj, data);
}
void iic_write_bytes(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len, unsigned char stop)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->write_bytes);
    (*((iic_vtable_t **)obj))->write_bytes(obj, addr, data, len, stop);
}
unsigned char iic_read_byte(iic_interface_t *obj, unsigned char ack)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->read_byte);
    return (*((iic_vtable_t **)obj))->read_byte(obj, ack);
}
int iic_read_bytes(iic_interface_t *obj, unsigned char addr, unsigned char *data, int len)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->read_bytes);
    return (*((iic_vtable_t **)obj))->read_bytes(obj, addr, data, len);
}
int iic_write_read(iic_interface_t *obj, unsigned char addr, unsigned char *wdata, int wlen, unsigned char *rdata, int rlen)
{
    DEBUG_ASSERT((*((iic_vtable_t **)obj))->write_read);
    return (*((iic_vtable_t **)obj))->write_read(obj, addr, wdata, wlen, rdata, rlen);
}