#ifndef led_H
#define led_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct led
{
    void(*led_write)(bool state);
    bool state_on_level;
}led_def;

typedef enum
{
    LED_WRITE,
    LED_STATE_ON_LEVEL,
}led_interface_def;

typedef enum
{
    LED_ON,
    LED_OFF
}led_state_def;

#ifdef  __cplusplus
extern "C"{
#endif

bool led_init(led_def* obj);
bool led_register_interface(led_def* obj, led_interface_def type, void(*led_write)(bool state));
bool led_register_state_on_level(led_def* obj, led_interface_def type, uint32_t level);
void led_write(led_def* obj, led_state_def state);
void led_deint(led_def* obj);

#ifdef  __cplusplus
}
#endif

#endif