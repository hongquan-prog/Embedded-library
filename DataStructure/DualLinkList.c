//
// Created by lhq on 2022/2/16.
//

#include "DualLinkList.h"

DualLinkListNode dual_link_list_position(DualLinkList *list, int position)
{
    dual_link_list_def *obj = (dual_link_list_def *)list;
    struct dual_link_list_node *current = (struct dual_link_list_node *)(&(obj->m_head));

    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }
    return current;
}

DualLinkList *dual_link_list_create(list_create_t type, DualLinkList* list)
{
    static list_vtable_def s_dual_link_list_vtable = {
            .insert = dual_link_list_insert,
            .remove = dual_link_list_remove,
            .find = dual_link_list_find,
            .get = dual_link_list_get,
            .set = NULL,
            .length = dual_link_list_length,
            .destroy = dual_link_list_destroy,
            .begin = dual_link_list_begin,
            .end = dual_link_list_end,
            .next = dual_link_list_next,
            .pre = dual_link_list_pre,
            .current = dual_link_list_current};

    dual_link_list_def *ret = NULL;

    if(type == LIST_CREATE_DYNAMIC)
    {
        ret = malloc(sizeof(dual_link_list_def));
        if (ret)
        {
            ret->m_length = 0;
            ret->m_vtable = &s_dual_link_list_vtable;
            ret->m_head.next = NULL;
            ret->m_head.pre = NULL;
            ret->m_is_static = false;
        }
    }
    else if(list)
    {
        ret = (dual_link_list_def *)list;
        ret->m_length = 0;
        ret->m_vtable = &s_dual_link_list_vtable;
        ret->m_head.next = NULL;
        ret->m_head.pre = NULL;
        ret->m_is_static = true;
    }

    return (DualLinkList *)ret;
}

bool dual_link_list_insert(DualLinkList *list, int i, const DualLinkListNode node)
{
    bool ret = true;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i <= obj->m_length) && node)
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        if (current)
        {
            struct dual_link_list_node *next = current->next;
            ((struct dual_link_list_node *)node)->next = next;
            ((struct dual_link_list_node *)node)->pre = (current == &(obj->m_head)) ? (NULL) : (current);
            current->next = node;
            ++obj->m_length;
            if (next)
            {
                next->pre = node;
            }
        }
    }
    else
    {
        ret = false;
    }
    return ret;
}

DualLinkListNode dual_link_list_remove(DualLinkList *list, int i)
{
    DualLinkListNode ret = NULL;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->m_length))
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        struct dual_link_list_node *toDel = (current) ? (current->next) : (NULL);
        struct dual_link_list_node *next = (toDel) ? (toDel->next) : (NULL);
        if (current && toDel)
        {
            ret = toDel;
            current->next = next;
            --obj->m_length;
            if (next)
            {
                next->pre = toDel->pre;
            }
        }
    }
    return ret;
}

void dual_link_list_destroy(DualLinkList *list)
{
    if(((dual_link_list_def *)list)->m_is_static == LIST_CREATE_DYNAMIC)
    {
        free(list);
    }
}

int dual_link_list_length(DualLinkList *list)
{
    return ((dual_link_list_def *)list)->m_length;
}

void dual_link_list_begin(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->m_current = ((dual_link_list_def *)list)->m_head.next;
    }
}

void dual_link_list_next(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->m_current = ((dual_link_list_def *)list)->m_current->next;
    }
}

void dual_link_list_pre(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->m_current = ((dual_link_list_def *)list)->m_current->pre;
    }
}

bool dual_link_list_end(DualLinkList *list)
{
    bool ret = false;

    if (list)
    {
        dual_link_list_def *obj = list;
        ret = (obj->m_current == NULL);
    }

    return ret;
}

DualLinkListNode dual_link_list_current(DualLinkList *list)
{
    return (list) ? (((dual_link_list_def *)list)->m_current) : (NULL);
}

int dual_link_list_find(DualLinkList *list, const DualLinkListNode node)
{
    int ret = -1;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && node)
    {
        struct dual_link_list_node *current = &(obj->m_head);

        for (int i = 0; i < obj->m_length; i++)
        {
            current = current->next;
            if (current == node)
            {
                ret = i;
                break;
            }
        }
    }

    return ret;
}

bool dual_link_list_get(DualLinkList *list, int i, DualLinkListNode *node)
{
    bool ret = true;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->m_length) && node)
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        *((struct dual_link_list_node **)node) = current->next;
    }
    else
    {
        ret = false;
    }

    return ret;
}