#include "modbus_task.h"
#include "mb.h"
#include "tim.h"
#include "usart.h"
#include "cmsis_os.h"

/* uart2 data cahce */
uint8_t s_uart2_cache = 0;

/* modbus event group */
static osMessageQueueId_t s_modbus_queueHandle;
static const osMessageQueueAttr_t s_modbus_queue_attributes = {
    .name = "modbus_port_queue"};

/****************************************************************************
 * event group driver
 ***************************************************************************/
static BOOL event_init(void)
{
    s_modbus_queueHandle = osMessageQueueNew(1, sizeof(eMBEventType), &s_modbus_queue_attributes);
    return (s_modbus_queueHandle != NULL);
}

static BOOL event_post(eMBEventType eEvent)
{
    return (osOK == osMessageQueuePut(s_modbus_queueHandle, &eEvent, 0, 0)) ? TRUE : FALSE;
}

static BOOL event_get(eMBEventType *eEvent)
{
    return (osMessageQueueGet(s_modbus_queueHandle, eEvent, 0, osWaitForever) == osOK);
}

/****************************************************************************
 * serial driver
 ****************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        pxMBFrameCBTransmitterEmpty( );
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        pxMBFrameCBByteReceived( );
    }
}

static BOOL modbus_serial_init(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity)
{
    MX_USART2_UART_Init( );
    HAL_UART_Receive_IT(&huart2, &s_uart2_cache, 1);
    
    return TRUE;
}

static void modbus_serial_enable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xRxEnable)
    {
        // gpio_bit_reset(MB_UART_TEN_PORT, MB_UART_TEN_PIN);
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
    }

    if (xTxEnable)
    {
        // gpio_bit_set(MB_UART_TEN_PORT, MB_UART_TEN_PIN);
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);
    }
    else
    {
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);
    }
}

static BOOL modbus_write_byte(CHAR ucByte)
{
    huart2.Instance->TDR = ucByte;
    return TRUE;
}

static BOOL modbus_read_byte(CHAR *pucByte)
{
    *pucByte = huart2.Instance->RDR;
    return TRUE;
}

/****************************************************************************
 * timer driver
 ****************************************************************************/
void HAL_TIM7_Register_PeriodElapsedCallback(void (*cb)(TIM_HandleTypeDef *));

static void timer_cb(TIM_HandleTypeDef *htim)
{
    pxMBPortCBTimerExpired( );
}

static BOOL timer_init(USHORT usTim1Timerout50us)
{
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* TIMER7CLK = 108 / 216 = 0.5MHZ */
    htim7.Instance = TIM7;
    htim7.Init.Prescaler = 215;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.Period = (25 * usTim1Timerout50us - 1) * 2;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM7_Register_PeriodElapsedCallback(timer_cb);

    return TRUE;
}

static void timer_enable()
{
    __HAL_TIM_SetCounter(&htim7, 0);
    __HAL_TIM_ENABLE(&htim7);
}

static void timer_disable()
{
    __HAL_TIM_DISABLE(&htim7);
    __HAL_TIM_SetCounter(&htim7, 0);
}

void timer_delay(USHORT usTimeOutMS)
{
    osDelay(usTimeOutMS);
}

void modbus_task_init(void)
{
    xMBInterface drv = {
        vPortEnterCritical,
        vPortExitCritical,
        modbus_serial_init,
        modbus_serial_enable,
        modbus_write_byte,
        modbus_read_byte,
        event_init,
        event_post,
        event_get,
        timer_init,
        timer_enable,
        timer_disable,
        timer_delay
    };

    vMBRegisterInterface(&drv);

    eMBInit(MB_RTU, 1, 0, 115200, MB_PAR_EVEN);
    eMBEnable( );
}

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode)
{
    return MB_ENOERR;
}