#pragma once

#include "iic_interface.h"

typedef enum
{
    AT24C01, // Internally organized with 16 pages of 8 bytes each, the 1K requires a 7-bit data word address for random word addressing.
    AT24C02, // Internally organized with 32 pages of 8 bytes each, the 2K requires an 8-bit data word address for random word addressing.
    AT24C04, // Internally organized with 32 pages of 16 bytes each, the 4K requires a 9-bit data word address for random word addressing.
    AT24C08, // Internally organized with 64 pages of 16 bytes each, the 8K requires a 10-bit data word address for random word addressing.
    AT24C16  // Internally organized with 128 pages of 16 bytes each, the 16K requires an 11-bit data word address for random word addressing
}at24cxx_chip_t;

typedef struct 
{
    // iic interface
    iic_interface_t *iic;
    // delay function interface
    void (*delay)(unsigned int ms);

    // device address
    unsigned char device_addr;
    // device type
    at24cxx_chip_t type;
    // max address 
    unsigned int address_limit;
    // page size
    unsigned int page_size;
}at24cxx_t;


unsigned char at24cxx_init(at24cxx_t* obj, iic_interface_t* iic, at24cxx_chip_t type, unsigned char device_addr, void (*delay)(unsigned int ms));
unsigned char at24cxx_read(at24cxx_t* obj, unsigned short addr, unsigned char* buf, unsigned int len);
unsigned char at24cxx_write(at24cxx_t* obj, unsigned short addr, unsigned char* buf, unsigned int len);
