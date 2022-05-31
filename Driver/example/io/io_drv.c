#include "io_drv.h"
#include "stm32f7xx_hal.h"

struct gpio_map_t
{
    GPIO_TypeDef *GPIO;
    uint32_t GPIO_Pin;
};

static const struct gpio_map_t s_io_map[IO_NUM] = {
    {GPIOA, GPIO_PIN_0},
    {GPIOA, GPIO_PIN_1},
    {GPIOA, GPIO_PIN_2},
    {GPIOA, GPIO_PIN_3}};

static void bsp_io_init(io_pin_def pin, io_pull_def pull, io_mode_def mode)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (IO_PULL_NO == pull)
        GPIO_InitStruct.Pull =GPIO_NOPULL;
    else if(IO_PULL_UP == pull)
        GPIO_InitStruct.Pull =GPIO_PULLUP;
    else if(IO_PULL_DOWN == pull)
        GPIO_InitStruct.Pull =GPIO_PULLDOWN;

    GPIO_InitStruct.Pin = s_io_map[pin].GPIO_Pin;
    GPIO_InitStruct.Mode = (IO_INPUT == mode) ? (GPIO_MODE_INPUT) : (GPIO_MODE_OUTPUT_PP);
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(s_io_map[pin].GPIO , &GPIO_InitStruct);
}

static io_status_def bsp_io_read(io_pin_def pin)
{
    return (HAL_GPIO_ReadPin(s_io_map[pin].GPIO, s_io_map[pin].GPIO_Pin) == GPIO_PIN_RESET) ? (IO_RESET) : (IO_SET);
}

static void bsp_io_write(io_pin_def pin, io_status_def status)
{
    HAL_GPIO_WritePin(s_io_map[pin].GPIO, s_io_map[pin].GPIO_Pin, (IO_RESET == status) ? (GPIO_PIN_RESET) : (GPIO_PIN_SET));
}

io_interface_t *example_io_interface_get(void)
{
    static io_drv_t io = {
        .init = bsp_io_init,
        .read = bsp_io_read,
        .write = bsp_io_write};

    return &io;
}