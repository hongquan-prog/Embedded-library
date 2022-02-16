//
// Created by lhq on 2022/2/15.
//

#include "LinkList.h"


LinkListNode link_list_position(LinkList *list, int position)
{
    link_list_def *obj = (link_list_def *)list;
    struct link_list_node *current = (struct link_list_node *)(&(obj->m_head));

    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }

    return current;
}

LinkList *link_list_create(list_create_t type, LinkList* list)
{
    static list_vtable_def s_link_list_vtable = {
            .insert = link_list_insert,
            .remove = link_list_remove,
            .find = link_list_find,
            .get = link_list_get,
            .set = NULL,
            .length = link_list_length,
            .destroy = link_list_destroy,
            .begin = link_list_begin,
            .end = link_list_end,
            .next = link_list_next,
            .pre = NULL,
            .current = link_list_current};

    link_list_def *ret = NULL;

    if(type == LIST_CREATE_DYNAMIC)
    {
        ret = malloc(sizeof(link_list_def));
        if(ret)
        {
            ret->m_current = NULL;
            ret->m_length = 0;
            ret->m_vtable = &s_link_list_vtable;
            ret->m_head.next = NULL;
            ret->m_is_static = false;
        }
    }
    else if (list)
    {
        ret = (link_list_def *)list;
        ret->m_current = NULL;
        ret->m_length = 0;
        ret->m_vtable = &s_link_list_vtable;
        ret->m_head.next = NULL;
        ret->m_is_static = true;
    }

    return (LinkList *)ret;
}

bool link_list_insert(LinkList *list, int i, const LinkListNode node)
{
    bool ret = true;
    link_list_def *obj = (link_list_def *)list;

    if (obj && (i >= 0) && (i <= obj->m_length) && node)
    {
        struct link_list_node *current = link_list_position(obj, i);
        if (current)
        {
            ((struct link_list_node *)node)->next = current->next;
            current->next = node;
            ++obj->m_length;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}

LinkListNode link_list_remove(LinkList *list, int i)
{
    LinkListNode ret = NULL;
    link_list_def *obj = (link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->m_length))
    {
        struct link_list_node *current = link_list_position(obj, i);
        struct link_list_node *next = (current) ? (current->next) : (NULL);
        if (current && next)
        {
            ret = next;
            current->next = next->next;
            --obj->m_length;
        }
    }

    return ret;
}

int link_list_length(LinkList *list)
{
    return ((link_list_def *)list)->m_length;
}

int link_list_find(LinkList *list, const LinkListNode node)
{
    int ret = -1;
    link_list_def *obj = (link_list_def *)list;

    if (obj && node)
    {
        struct link_list_node *current = &(obj->m_head);
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

bool link_list_get(LinkList *list, int i, LinkListNode *node)
{
    bool ret = true;
    link_list_def *obj = (link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->m_length) && node)
    {
        struct link_list_node *current = link_list_position(obj, i);
        *((struct link_list_node **)node) = current->next;
    }
    else
    {
        ret = false;
    }

    return ret;
}

void link_list_destroy(LinkList *list)
{
    if(((link_list_def *)list)->m_is_static == LIST_CREATE_DYNAMIC)
    {
        free(list);
    }
}

void link_list_begin(LinkList *list)
{
    if (list)
    {
        ((link_list_def *)list)->m_current = ((link_list_def *)list)->m_head.next;
    }
}

void link_list_next(LinkList *list)
{
    if (list)
    {
        ((link_list_def *)list)->m_current = ((link_list_def *)list)->m_current->next;
    }
}

bool link_list_end(LinkList *list)
{
    bool ret = false;

    if (list)
    {
        ret = (((link_list_def *)list)->m_current == NULL);
    }

    return ret;
}

LinkListNode link_list_current(LinkList *list)
{
    return (list) ? (((link_list_def *)list)->m_current) : (NULL);
}
