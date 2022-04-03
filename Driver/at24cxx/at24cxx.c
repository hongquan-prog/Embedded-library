#include "at24cxx.h"

#define ENABLE_DEBUG 0

#if ENABLE_DEBUG
#include "err.h"
#define AT24CXX_LOG(e) LOG(ERR_CONSTRUCT(InvalidParameter), e)
#else
#define AT24CXX_LOG(e)
#endif

unsigned char at24cxx_init(at24cxx_t *obj, iic_interface_t *iic, at24cxx_chip_t type, unsigned char device_addr, void (*delay)(unsigned int ms))
{
    unsigned char ret = 0;

    if (obj && iic && delay)
    {
        ret = 1;
        obj->iic = iic;
        obj->delay = delay;
        obj->device_addr = device_addr;
        obj->type = type;

        switch (type)
        {
        case AT24C01:
            obj->address_limit = 128;
            obj->page_size = 8;
            break;
        case AT24C02:
            obj->address_limit = 256;
            obj->page_size = 8;
            break;
        case AT24C04:
            obj->address_limit = 512;
            obj->page_size = 16;
            break;
        case AT24C08:
            obj->address_limit = 1024;
            obj->page_size = 16;
            break;
        case AT24C16:
            obj->address_limit = 2048;
            obj->page_size = 16;
            break;
        default:
            ret = 0;
            AT24CXX_LOG("please check chicp\r\n");
            break;
        }
    }
    else
    {
        AT24CXX_LOG("please check obj, delay and iic\r\n");
    }

    return ret;
}

unsigned char at24cxx_page_write(at24cxx_t *obj, unsigned short addr, unsigned char *buf, unsigned int len)
{
    int i = 0;
    unsigned char ret = (len <= obj->page_size);
    unsigned short end_addr = addr + len;
    unsigned char addr_msb;

    if (ret && obj && buf && (end_addr <= obj->address_limit))
    {
        addr_msb = obj->device_addr | ((addr >> 8) << 1);
        iic_start(obj->iic);
        iic_write_byte(obj->iic, addr_msb);

        if (iic_wait_ack(obj->iic))
        {
            iic_write_byte(obj->iic, (addr & 0xff));
            iic_wait_ack(obj->iic);

            for (i = 0; i < len; i++)
            {
                iic_write_byte(obj->iic, buf[i]);
                iic_wait_ack(obj->iic);

                if ((++addr & (obj->page_size - 1)) == 0)
                {
                    break;
                }
            }

            len = len - i;
        }
        else
        {
            ret = 0;
        }

        iic_stop(obj->iic);
    }
    else
    {
        AT24CXX_LOG("please check obj, buf, len and addr\r\n");
    }

    return ret;
}

unsigned char at24cxx_write(at24cxx_t *obj, unsigned short addr, unsigned char *buf, unsigned int len)
{
    unsigned char ret = 0;
    unsigned int read_addr = addr;
    unsigned int read_len = obj->page_size - (read_addr % obj->page_size);
    read_len = (len > read_len) ? (read_len) : (len);

    while(len)
    {
        ret = at24cxx_page_write(obj, read_addr, buf + (read_addr - addr), read_len);
        obj->delay(3);
        len = len - read_len;
        read_addr = read_addr + read_len;
        read_len = obj->page_size - (read_addr % obj->page_size);
        read_len = (len > read_len) ? (read_len) : (len);
        
    }

    return ret;
}

unsigned char at24cxx_read(at24cxx_t *obj, unsigned short addr, unsigned char *buf, unsigned int len)
{
    int i = 0;
    unsigned char ret = 0;
    unsigned char addr_msb;

    if (obj && ((addr + len) < obj->address_limit) && buf)
    {
        ret = 1;

        addr_msb = obj->device_addr | ((addr >> 8) << 1);
        iic_start(obj->iic);
        iic_write_byte(obj->iic, addr_msb);
        if (iic_wait_ack(obj->iic))
        {
            iic_write_byte(obj->iic, (addr & 0xff));
            iic_wait_ack(obj->iic);
            iic_start(obj->iic);
            iic_write_byte(obj->iic, obj->device_addr | 1);
            iic_wait_ack(obj->iic);

            for (i = 0; i < len - 1; i++)
            {
                buf[i] = iic_read_byte(obj->iic, 1);
            }
            len = len - i;

            buf[i] = iic_read_byte(obj->iic, 0);
        }
        else
        {
            ret = 0;
        }

        iic_stop(obj->iic);
    }
    else
    {
        AT24CXX_LOG("please check obj, buf and addr\r\n");
    }

    return ret;
}
