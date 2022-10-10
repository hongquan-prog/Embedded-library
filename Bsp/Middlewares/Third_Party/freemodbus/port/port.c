#include "port.h"

xMBInterface prvxMBInterface = { 0 };

void vMBRegisterInterface(xMBInterface *interface)
{
    prvxMBInterface = *interface;
}

void ENTER_CRITICAL_SECTION(void)
{
    prvxMBInterface.enter_critical( );
}

void EXIT_CRITICAL_SECTION(void)
{
    prvxMBInterface.exit_critical( );
}

BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
    return prvxMBInterface.serial_init(ucPort, ulBaudRate, ucDataBits, eParity);
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    prvxMBInterface.serial_enable(xRxEnable, xTxEnable);
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    return prvxMBInterface.serial_write_byte(ucByte);
}

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    pxMBFrameCBByteReceived( );
    return prvxMBInterface.serial_read_byte(pucByte);
}

BOOL xMBPortEventInit(void)
{
    return prvxMBInterface.event_init( );
}

BOOL xMBPortEventPost(eMBEventType eEvent)
{
    return prvxMBInterface.event_post(eEvent);
}

BOOL xMBPortEventGet(eMBEventType *eEvent)
{
    return prvxMBInterface.event_get(eEvent);
}

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    return prvxMBInterface.timer_init(usTim1Timerout50us);
}

void vMBPortTimersEnable( )
{
    prvxMBInterface.timer_enable( );
}

void vMBPortTimersDisable( )
{
    prvxMBInterface.timer_disable( );
}

void vMBPortTimersDelay(USHORT usTimeOutMS)
{
    prvxMBInterface.timer_delay(usTimeOutMS);
}