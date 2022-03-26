#include "iic_interface.h"

#define ENABLE_DEBUG 1

#if ENABLE_DEBUG
#include "err.h"
#define NULL_POINTER_ASSET(p, e)                \
    {                                           \
        if (!p)                                 \
        {                                       \
            LOG(ERR_CONSTRUCT(NullPointer), e); \
            while (1)                           \
                ;                               \
        }                                       \
    }
#else
#define NULL_POINTER_ASSET(p, e)
#endif

void iic_start(iic_interface_t *obj)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->start, "start function not exist in vtable");
    (*((iic_vtable_t **)obj))->start(obj);
}
void iic_stop(iic_interface_t *obj)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->stop, "stop function not exist in vtable");
    (*((iic_vtable_t **)obj))->stop(obj);
}
void iic_write_ack(iic_interface_t *obj)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->write_ack, "write_ack function not exist in vtable");
    (*((iic_vtable_t **)obj))->write_ack(obj);
}
void iic_write_nack(iic_interface_t *obj)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->write_nack, "write_nack function not exist in vtable");
    (*((iic_vtable_t **)obj))->write_nack(obj);
}
unsigned char iic_wait_ack(iic_interface_t *obj)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->wait_ack, "wait_ack function not exist in vtable");
    return (*((iic_vtable_t **)obj))->wait_ack(obj);
}
void iic_write_byte(iic_interface_t *obj, unsigned char data)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->write_byte, "write_byte function not exist in vtable");
    (*((iic_vtable_t **)obj))->write_byte(obj, data);
}
unsigned char iic_read_byte(iic_interface_t *obj, unsigned char ack)
{
    NULL_POINTER_ASSET((*((iic_vtable_t **)obj))->read_byte, "read_byte function not exist in vtable");
    return (*((iic_vtable_t **)obj))->read_byte(obj, ack);
}
