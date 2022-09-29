#include "port.h"

typedef struct
{
    void(*enter_critical)(void);
    void(*exit_critical)(void);
} freemodbus_critical_t;

freemodbus_critical_t s_critical_interface = { 0 };

void freemodbus_register_interface(freemodbus_critical_t *interface)
{
    s_critical_interface = *interface;
}

void ENTER_CRITICAL_SECTION(void)
{
    assert(s_critical_interface.enter_critical);
    s_critical_interface.enter_critical( );
}

void EXIT_CRITICAL_SECTION(void)
{
    assert(s_critical_interface.exit_critical);
    s_critical_interface.exit_critical( );
}