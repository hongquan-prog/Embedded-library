//
// Created by lhq on 2022/2/16.
//

#include "DualCircleList.h"

DualLinkListNode dual_link_list_position(DualLinkList *list, int position);
static DualCircleListNode dual_circle_list_remove(DualCircleList *list, int i);
static bool dual_circle_list_insert(DualCircleList *list, int i, const DualCircleListNode node);
static bool dual_circle_push_back(DualCircleList *list, DualCircleListNode node);
static bool dual_circle_push_front(DualCircleList *list, DualCircleListNode node);
static DualCircleListNode dual_circle_pop_front(DualCircleList *list);
static DualCircleListNode dual_circle_pop_back(DualCircleList *list);

static void dual_circle_list_destroy(DualCircleList *list)
{
    if (((dual_link_list_def *)list)->list_type == LIST_DYNAMIC)
    {
        free(list);
    }
}

DualCircleList *dual_circle_list_create(list_type_t type, DualCircleList *list)
{
    static bool vtable_is_init = false;
    static list_vtable_def s_dual_circle_list_vtable = {0};

    dual_link_list_def *ret = dual_link_list_create(type, list);

    if (ret)
    {
        if (!vtable_is_init)
        {
            vtable_is_init = true;
            s_dual_circle_list_vtable.insert = dual_circle_list_insert;
            s_dual_circle_list_vtable.remove = dual_circle_list_remove;
            s_dual_circle_list_vtable.destroy = dual_circle_list_destroy;
            s_dual_circle_list_vtable.push_back = dual_circle_push_back;
            s_dual_circle_list_vtable.push_front = dual_circle_push_front;
            s_dual_circle_list_vtable.pop_back = dual_circle_pop_back;
            s_dual_circle_list_vtable.pop_front = dual_circle_pop_front;

            s_dual_circle_list_vtable.find = ret->vtable->find;
            s_dual_circle_list_vtable.get = ret->vtable->get;
            s_dual_circle_list_vtable.length = ret->vtable->length;
            s_dual_circle_list_vtable.begin = ret->vtable->begin;
            s_dual_circle_list_vtable.end = ret->vtable->end;
            s_dual_circle_list_vtable.next = ret->vtable->next;
            s_dual_circle_list_vtable.pre = ret->vtable->pre;
            s_dual_circle_list_vtable.current = ret->vtable->current;
        }

        ret->vtable = &s_dual_circle_list_vtable;
    }

    return ret;
}

static DualLinkListNode dual_circle_list_position(DualCircleList *list, int position)
{
    dual_link_list_def *obj = (dual_link_list_def *)list;
    struct dual_link_list_node *current = (struct dual_link_list_node *)(&(obj->head));

    if (position > obj->length / 2)
    {
        current = current->next->pre;
        for (int i = 0; i < position; i++)
        {
            current = current->pre;
        }
    }
    else
    {
        for (int i = 0; i < position; i++)
        {
            current = current->next;
        }
    }

    return current;
}

static void dual_circle_list_add(struct dual_link_list_node *node,
                                 struct dual_link_list_node *pre,
                                 struct dual_link_list_node *next)
{
    node->pre = pre;
    node->next = next;
    next->pre = node;
    pre->next = node;
}

static bool dual_circle_list_insert(DualCircleList *list, int i, const DualCircleListNode node)
{
    bool ret = true;
    dual_link_list_def *obj = list;

    if (obj && (i >= 0) && (i <= obj->length) && node)
    {
        if (0 == obj->length)
        {
            dual_circle_list_add(node, node, node);
            obj->head.next = node;
        }
        else
        {
            struct dual_link_list_node *current = dual_circle_list_position(obj, i);
            dual_circle_list_add(node, current->next->pre, current->next);
        }
        ++obj->length;
    }
    else
    {
        ret = false;
    }

    return ret;
}

static DualCircleListNode dual_circle_list_remove(DualCircleList *list, int i)
{
    DualLinkListNode ret = NULL;
    dual_link_list_def *obj = list;

    if (obj && (i >= 0) && (i < obj->length))
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        struct dual_link_list_node *toDel = current->next;
        struct dual_link_list_node *pre = toDel->pre;
        struct dual_link_list_node *next = toDel->next;
        if (current)
        {
            ret = toDel;
            current->next = next;
            next->pre = pre;
            --obj->length;
        }
    }

    return ret;
}

static bool dual_circle_push_back(DualCircleList *list, DualCircleListNode node)
{
    bool ret = false;
    dual_link_list_def *obj = list;

    if (list && node)
    {
        if (0 == obj->length)
        {
            dual_circle_list_add(node, node, node);
            obj->head.next = node;
        }
        else
        {
            dual_circle_list_add(node, obj->head.next->pre, obj->head.next);
        }
        ++obj->length;
        ret = true;
    }

    return ret;
}

static bool dual_circle_push_front(DualCircleList *list, DualCircleListNode node)
{
    bool ret = dual_circle_push_back(list, node);
    dual_link_list_def *obj = list;

    if (ret)
    {
        obj->head.next = node;
    }

    return ret;
}

static DualCircleListNode dual_circle_pop_front(DualCircleList *list)
{
    return dual_circle_list_remove(list, 0);
}

static DualCircleListNode dual_circle_pop_back(DualCircleList *list)
{
    return dual_circle_list_remove(list, ((dual_link_list_def *)list)->length - 1);
}
