#include "exit_interface.h"

void exit_init(exit_interface_t *obj, exit_trig_edge_def edge)
{
    DEBUG_ASSERT(obj && (*(exit_drv_t **)obj)->init);
    (*(exit_drv_t **)obj)->init(edge);
}

void exit_register_callback(exit_interface_t *obj, exit_callbck_t callback, void *param)
{
    DEBUG_ASSERT(obj && (*(exit_drv_t **)obj)->register_callback);
    (*(exit_drv_t **)obj)->register_callback(callback, param);
}

void exit_enable(exit_interface_t *obj)
{
    DEBUG_ASSERT(obj && (*(exit_drv_t **)obj)->enable);
    (*(exit_drv_t **)obj)->enable();
}

void exit_disable(exit_interface_t *obj)
{
    DEBUG_ASSERT(obj && (*(exit_drv_t **)obj)->disable);
    (*(exit_drv_t **)obj)->disable();
}

exit_trig_edge_def exit_read_trigger_edge(exit_interface_t *obj)
{
    DEBUG_ASSERT(obj && (*(exit_drv_t **)obj)->read_trigger_edge);
    return (*(exit_drv_t **)obj)->read_trigger_edge();
}