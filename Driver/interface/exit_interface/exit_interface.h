#pragma once

#include "err.h"

typedef void exit_interface_t;
typedef void (*exit_callbck_t)(void *param);

typedef enum
{
    Exit_Falling_Trigger,
    Exit_Rising_Trigger,
    Exit_Both_Trigger
}exit_trig_edge_def;

typedef struct
{
    void (*init)(exit_trig_edge_def edge);
    void (*register_callback)(exit_callbck_t callback, void *param);
    void (*enable)(void);
    void (*disable)(void);
    exit_trig_edge_def (*read_trigger_edge)(void);
}exit_drv_t;

void exit_init(exit_interface_t *obj, exit_trig_edge_def edge);
void exit_register_callback(exit_interface_t *obj, exit_callbck_t callback, void *param);
void exit_enable(exit_interface_t *obj);
void exit_disable(exit_interface_t *obj);
exit_trig_edge_def exit_read_trigger_edge(exit_interface_t *obj);