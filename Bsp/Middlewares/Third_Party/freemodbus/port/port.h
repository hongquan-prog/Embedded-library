#pragma once

#include "mbport.h"

typedef struct
{
    void(*enter_critical)(void);
    void(*exit_critical)(void);
    BOOL(*serial_init)(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity);
    void(*serial_enable)(BOOL xRxEnable, BOOL xTxEnable);
    BOOL(*serial_write_byte)(CHAR ucByte);
    BOOL(*serial_read_byte)(CHAR *pucByte);
    BOOL(*event_init)(void);
    BOOL(*event_post)(eMBEventType eEvent);
    BOOL(*event_get)(eMBEventType *eEvent);
    BOOL(*timer_init)(USHORT usTim1Timerout50us);
    void (*timer_enable)(void);
    void (*timer_disable)(void);
    void (*timer_delay)(USHORT usTimeOutMS);
} xMBInterface;

void vMBRegisterInterface(xMBInterface *interface);