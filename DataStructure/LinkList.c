//
// Created by lhq on 2022/2/15.
//

#include "LinkList.h"

static void link_list_destroy(LinkList *list);
bool link_list_insert(LinkList *list, int i, const LinkListNode node);
LinkListNode link_list_remove(LinkList *list, int i);
static int link_list_length(LinkList *list);
static int link_list_find(LinkList *list, const LinkListNode node);
bool link_list_get(LinkList *list, int i, LinkListNode *node);
static void link_list_begin(LinkList *list);
static void link_list_next(LinkList *list);
static bool link_list_end(LinkList *list);
static LinkListNode link_list_current(LinkList *list);

LinkListNode link_list_position(LinkList *list, int position)
{
    link_list_def *obj = (link_list_def *)list;
    struct link_list_node *current = (struct link_list_node *)(&(obj->head));

    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }

    return current;
}

LinkList *link_list_create(list_type_t type, LinkList* list)
{
    static list_vtable_def s_link_list_vtable = {
            .insert = link_list_insert,
            .remove = link_list_remove,
            .find = link_list_find,
            .get = link_list_get,
            .length = link_list_length,
            .destroy = link_list_destroy,
            .begin = link_list_begin,
            .end = link_list_end,
            .next = link_list_next,
            .pre = NULL,
            .current = link_list_current};

    link_list_def *ret = NULL;

    if(type == LIST_DYNAMIC)
    {
        ret = malloc(sizeof(link_list_def));
        if(ret)
        {
            ret->current = NULL;
            ret->length = 0;
            ret->vtable = &s_link_list_vtable;
            ret->head.next = NULL;
            ret->list_type = type;
        }
    }
    else if (list)
    {
        ret = (link_list_def *)list;
        ret->current = NULL;
        ret->length = 0;
        ret->vtable = &s_link_list_vtable;
        ret->head.next = NULL;
        ret->list_type = type;
    }

    return (LinkList *)ret;
}

bool link_list_insert(LinkList *list, int i, const LinkListNode node)
{
    bool ret = true;
    link_list_def *obj = (link_list_def *)list;

    if (obj && (i >= 0) && (i <= obj->length) && node)
    {
        struct link_list_node *current = link_list_position(obj, i);
        if (current)
        {
            ((struct link_list_node *)node)->next = current->next;
            current->next = node;
            ++obj->length;
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

    if (obj && (i >= 0) && (i < obj->length))
    {
        struct link_list_node *current = link_list_position(obj, i);
        struct link_list_node *toDel = (current) ? (current->next) : (NULL);
        if (current && toDel)
        {
            if(toDel == obj->current)
            {
                obj->current = toDel->next;
            }
            
            ret = toDel;
            current->next = toDel->next;
            --obj->length;
        }
    }

    return ret;
}

static int link_list_length(LinkList *list)
{
    return ((link_list_def *)list)->length;
}

static int link_list_find(LinkList *list, const LinkListNode node)
{
    int ret = -1;
    link_list_def *obj = (link_list_def *)list;

    if (obj && node)
    {
        struct link_list_node *current = &(obj->head);
        for (int i = 0; i < obj->length; i++)
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

    if (obj && (i >= 0) && (i < obj->length) && node)
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
    if(((link_list_def *)list)->list_type == LIST_DYNAMIC)
    {
        free(list);
    }
}

static void link_list_begin(LinkList *list)
{
    if (list)
    {
        ((link_list_def *)list)->current = ((link_list_def *)list)->head.next;
    }
}

static void link_list_next(LinkList *list)
{
    if (list)
    {
        ((link_list_def *)list)->current = ((link_list_def *)list)->current->next;
    }
}

static bool link_list_end(LinkList *list)
{
    bool ret = false;

    if (list)
    {
        ret = (((link_list_def *)list)->current == NULL);
    }

    return ret;
}

static LinkListNode link_list_current(LinkList *list)
{
    return (list) ? (((link_list_def *)list)->current) : (NULL);
}
