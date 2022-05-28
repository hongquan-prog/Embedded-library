#include "led.h"

bool led_init(led_def* obj)
{
    return true;
}

bool led_register_interface(led_def* obj, led_interface_def type, void(*led_write)(bool state))
{
    bool ret = true;

    if (obj)
    {
        switch (type)
        {
        case LED_WRITE:
            obj->led_write = led_write;
            break;
        default:
            ret = false;
            break;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool led_register_state_on_level(led_def* obj, led_interface_def type, uint32_t level)
{
    bool ret = true;

    if (obj)
    {
        switch (type)
        {
        case LED_STATE_ON_LEVEL:
            obj->state_on_level = level;
            break;
        default:
            // to do
            ret = false;
            break;
        }
    }
    else
    {
        // to do
        ret = false;
    }

    return ret;
}

void led_write(led_def* obj, led_state_def state)
{
    if(obj)
    {
        if(state == LED_OFF)
        {
            obj->led_write(state);
        }
        else
        {
            obj->led_write(state);
        }
    }
}

void led_deint(led_def* obj)
{
    return;
}