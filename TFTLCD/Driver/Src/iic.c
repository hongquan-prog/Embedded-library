#include "iic.h"

static void iic_delay()
{
    uint8_t count = 0x7f;
    while(count--)
    {}
}

void iic_start(iic_class_def* obj)
{
    if(!obj)
    {
        return;
    }
    obj->driver.sda_set_output();
    obj->driver.scl_write(1);
    obj->driver.sda_write(1);
    iic_delay();
    obj->driver.sda_write(0);
    iic_delay();
    obj->driver.scl_write(0);
}

void iic_stop(iic_class_def* obj)
{
    if(!obj)
    {
        return;
    }
    obj->driver.sda_set_output();
    obj->driver.scl_write(0);
    obj->driver.sda_write(0);
    iic_delay();
    obj->driver.scl_write(1);
    obj->driver.sda_write(1);
    iic_delay();
}

void iic_nack(iic_class_def* obj)
{
    if(!obj)
    {
        return;
    }
    obj->driver.sda_set_output();
    obj->driver.scl_write(0);
    obj->driver.sda_write(1);
    iic_delay();
    obj->driver.scl_write(1);
    iic_delay();
    obj->driver.scl_write(0);
}

void iic_ack(iic_class_def* obj)
{
    if(!obj)
    {
        return;
    }
    
    obj->driver.scl_write(0);
    obj->driver.sda_set_output();
    iic_delay();
    obj->driver.sda_write(0);
    iic_delay();
    obj->driver.scl_write(1);
    iic_delay();
    obj->driver.scl_write(0);
}

uint8_t iic_read_byte(iic_class_def* obj, uint8_t ack)
{
    uint8_t ret = 0;

    if(!obj)
    {
        return ret;
    }
    obj->driver.sda_set_input();
    for (int i = 0; i < 8; i++)
    {
        ret <<= 1;
        obj->driver.scl_write(0);
        iic_delay();
        obj->driver.scl_write(1);
        iic_delay();
        if (obj->driver.sda_read())
        {
            ret++;
        }
        iic_delay();
    }
    (ack == 0) ? (iic_nack(obj)) : (iic_ack(obj));

    return ret;
}

void iic_write_byte(iic_class_def* obj, uint8_t data)
{
    if(!obj)
    {
        return;
    }
    obj->driver.sda_set_output();
    obj->driver.scl_write(0);
    for (int i = 0; i < 8; i++)
    {
        obj->driver.sda_write((data & 0x80) != 0);
        data <<= 1;
        iic_delay(); 
        obj->driver.scl_write(1);
        iic_delay();             
        obj->driver.scl_write(0);
        iic_delay();            
    }
}

bool iic_wait_ack(iic_class_def* obj)
{
    uint8_t timeout = 200;
    bool ret = false;

    if(!obj)
    {
        return ret;
    }
    obj->driver.sda_set_input();
    obj->driver.sda_write(1);
    iic_delay();
    obj->driver.scl_write(1);
    iic_delay();
    while (obj->driver.sda_read())
    {
        if(timeout == 0)
        {
            iic_stop(obj);
            break;
        }
        timeout--;
    }
    if(timeout > 0)
    {
        ret = true;
    }
    obj->driver.scl_write(0);

    return ret;
}

bool iic_init(iic_class_def* obj, iic_driver_class_def* driver)
{
    bool ret = false;
    if(obj && driver)
    {
        ret = true;
        obj->driver.scl_write = driver->scl_write;
        obj->driver.sda_read = driver->sda_read;
        obj->driver.sda_write = driver->sda_write;
        obj->driver.sda_set_input = driver->sda_set_input;
        obj->driver.sda_set_output = driver->sda_set_output;
        obj->driver.scl_set_output = driver->scl_set_output;
        obj->driver.scl_set_output();
        obj->driver.sda_set_output();
    }
    return ret;
}