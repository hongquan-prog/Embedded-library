//
// Created by lhq on 2022/6/7.
//

#include "CircleList.h"

LinkListNode link_list_position(LinkList *list, int position);
bool link_list_insert(LinkList *list, int i, const LinkListNode node);
LinkListNode link_list_remove(LinkList *list, int i);
bool link_list_get(LinkList *list, int i, LinkListNode *node);

static bool circle_list_insert(CircleList *list, int i, const CircleListNode node);
static CircleListNode circle_list_remove(CircleList *list, int i);
static bool circle_list_get(CircleList *list, int i, CircleListNode *node);
static bool circle_list_end(CircleList *list);

static void last_to_first(link_list_def *obj)
{
    struct link_list_node *ret = link_list_position(obj, obj->length - 1);
    if (ret)
    {
        ret->next->next = obj->head.next;
    }
}

CircleList *circle_list_create(list_type_t type, LinkList *list)
{
    static bool vtable_is_init = false;
    static list_vtable_def s_circle_list_vtable = {0};

    link_list_def *ret = link_list_create(type, list);
    if (ret)
    {
        if (!vtable_is_init)
        {
            vtable_is_init = true;
            s_circle_list_vtable.insert = circle_list_insert;
            s_circle_list_vtable.remove = circle_list_remove;
            s_circle_list_vtable.find = ret->vtable->find;
            s_circle_list_vtable.get = circle_list_get;
            s_circle_list_vtable.length = ret->vtable->length;
            s_circle_list_vtable.destroy = ret->vtable->destroy;
            s_circle_list_vtable.begin = ret->vtable->begin;
            s_circle_list_vtable.end = circle_list_end;
            s_circle_list_vtable.next = ret->vtable->next;
            s_circle_list_vtable.current = ret->vtable->current;
        }
        ret->vtable = &s_circle_list_vtable;
    }
    return ret;
}

static bool circle_list_insert(CircleList *list, int i, const CircleListNode node)
{
    bool ret = true;

    link_list_def *obj = (link_list_def *)list;
    i = (i % (obj->length + 1));
    ret = link_list_insert(list, i, node);
    if (ret && (i == 0))
    {
        last_to_first(obj);
    }

    return ret;
}

static CircleListNode circle_list_remove(CircleList *list, int i)
{
    CircleListNode ret = NULL;
    link_list_def *obj = (link_list_def *)list;
    i = (i % obj->length);
    if (i == 0)
    {
        struct link_list_node *toDel = obj->head.next;
        obj->head.next = toDel->next;
        obj->length--;
        ret = toDel;
        if (obj->length != 0)
        {
            last_to_first(obj);
            if(obj->current == toDel)
            {
                obj->current = toDel->next;
            }
        }
        else
        {
            obj->head.next = NULL;
        }
    }
    else
    {
        ret = link_list_remove(list, i);
    }
    return ret;
}

static bool circle_list_get(CircleList *list, int i, CircleListNode *node)
{
    link_list_def *obj = (link_list_def *)list;
    i = (i % obj->length);
    return link_list_get(list, i, node);
}



static bool circle_list_end(CircleList *list)
{
    bool ret = false;
    if (list)
    {
        link_list_def *obj = list;
        ret = (obj->current == NULL || obj->length == 0);
    }
    return ret;
}