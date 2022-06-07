//
// Created by lhq on 2022/2/15.
//

#include "List.h"

void list_insert(List *list, int i, ListNode node)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->insert);
    (*((list_vtable_def **)list))->insert(list, i, node);
}

ListNode list_remove(List *list, int i)
{
    ListNode ret = NULL;
    DEBUG_ASSET((*((list_vtable_def **)list))->remove);
    ret = (*((list_vtable_def **)list))->remove(list, i);

    return ret;
}

ListNode list_get(List *list, int i)
{
    ListNode ret = NULL;

    DEBUG_ASSET((*((list_vtable_def **)list))->get);
    (*((list_vtable_def **)list))->get(list, i, &ret);

    return ret;
}

int list_length(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->length);
    return (*((list_vtable_def **)list))->length(list);
}

int list_find(List *list, const ListNode node)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->find);
    return (*((list_vtable_def **)list))->find(list, node);
}

void list_begin(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->begin);
    (*((list_vtable_def **)list))->begin(list);
}

void list_next(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->next);
    (*((list_vtable_def **)list))->next(list);
}

void list_pre(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->pre);
    (*((list_vtable_def **)list))->pre(list);
}

bool list_end(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->end);
    return (*((list_vtable_def **)list))->end(list);
}

ListNode list_current(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->current);
    return (*((list_vtable_def **)list))->current(list);
}

void list_destory(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->destroy);
    (*((list_vtable_def **)list))->destroy(list);
}

bool list_push_back(List *list, const ListNode node)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->push_back);
    return (*((list_vtable_def **)list))->push_back(list, node);
}

bool list_push_front(List *list, const ListNode node)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->push_front);
    return (*((list_vtable_def **)list))->push_front(list, node);
}

ListNode list_pop_back(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->pop_back);
    return (*((list_vtable_def **)list))->pop_back(list);
}

ListNode list_pop_front(List *list)
{
    DEBUG_ASSET((*((list_vtable_def **)list))->pop_front);
    return (*((list_vtable_def **)list))->pop_front(list);
}