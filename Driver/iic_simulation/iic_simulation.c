#include "iic_simulation.h"

void iic_sim_init(iic_sim_interface_t *obj, iic_hardware_interface_t *hardware)
{
    static iic_vtable_t s_vtable = {
        .start = iic_sim_start,
        .stop = iic_sim_stop,
        .read_byte = iic_sim_read_byte,
        .write_byte = iic_sim_write_byte,
        .write_ack = iic_sim_write_ack,
        .write_nack = iic_sim_write_nack,
        .wait_ack = iic_sim_wait_ack};

    if (obj && hardware)
    {
        obj->vtabel = &s_vtable;
        obj->hardware = hardware;
    }
}

void iic_sim_start(iic_interface_t *p)
{
	iic_sim_interface_t *obj = (iic_sim_interface_t *)p;

    obj->hardware->sda_set_mode(IIC_PORT_OUT);
    obj->hardware->sda_write(1);
    obj->hardware->scl_write(1);
    obj->hardware->delay();
    obj->hardware->sda_write(0);
    obj->hardware->delay();
    obj->hardware->scl_write(0);
    obj->hardware->delay();
}

void iic_sim_stop(iic_interface_t *p)
{
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;

    obj->hardware->sda_set_mode(IIC_PORT_OUT);
    obj->hardware->sda_write(0);
    obj->hardware->scl_write(1);
    obj->hardware->delay();
    obj->hardware->sda_write(1);
    obj->hardware->delay();
}

unsigned char iic_sim_wait_ack(iic_interface_t *p)
{
    unsigned char ucErrTime = 0;
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;

    obj->hardware->sda_set_mode(IIC_PORT_IN);
    obj->hardware->sda_write(1);
    obj->hardware->delay();
    obj->hardware->scl_write(1);
    obj->hardware->delay();

    while (obj->hardware->sda_read())
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            iic_sim_stop(obj);
            return 0;
        }
    }
    obj->hardware->scl_write(0);
    obj->hardware->delay();

    return 1;
}

void iic_sim_write_ack(iic_interface_t *p)
{
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;

    obj->hardware->scl_write(0);
    obj->hardware->sda_set_mode(IIC_PORT_OUT);
    obj->hardware->sda_write(0);
    obj->hardware->delay();
    obj->hardware->scl_write(1);
    obj->hardware->delay();
    obj->hardware->scl_write(0);
    obj->hardware->sda_write(1);
}

void iic_sim_write_nack(iic_interface_t *p)
{
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;
    obj->hardware->scl_write(0);
    obj->hardware->sda_set_mode(IIC_PORT_OUT);
    obj->hardware->sda_write(1);
    obj->hardware->delay();
    obj->hardware->scl_write(1);
    obj->hardware->delay();
    obj->hardware->scl_write(0);
    obj->hardware->sda_write(1);
}

void iic_sim_write_byte(iic_interface_t *p, unsigned char data)
{
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;
    obj->hardware->sda_set_mode(IIC_PORT_OUT);
    obj->hardware->scl_write(0);

    for (int i = 0; i < 8; i++)
    {
        obj->hardware->sda_write((data & 0x80) >> 7);
        data <<= 1;
        obj->hardware->delay();
        obj->hardware->scl_write(1);
        obj->hardware->delay();
        obj->hardware->scl_write(0);
        obj->hardware->delay();
    }
}

unsigned char iic_sim_read_byte(iic_interface_t *p, unsigned char ack)
{
    unsigned char ret = 0;
    iic_sim_interface_t *obj = (iic_sim_interface_t *)p;

    obj->hardware->sda_set_mode(IIC_PORT_IN);
    for (int i = 0; i < 8; i++)
    {
        obj->hardware->scl_write(0);
        obj->hardware->delay();
        obj->hardware->scl_write(1);
        ret <<= 1;

        if (obj->hardware->sda_read())
        {
            ret++;
        }

        obj->hardware->delay();
    }

    if (ack)
    {
        iic_sim_write_ack(obj);
    }
    else
    {
        iic_sim_write_nack(obj);
    }

    return ret;
}
