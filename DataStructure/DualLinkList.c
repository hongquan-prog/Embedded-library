//
// Created by lhq on 2022/2/16.
//

#include "DualLinkList.h"

static void dual_link_list_destroy(DualLinkList *list);
static bool dual_link_list_insert(DualLinkList *list, int i, const DualLinkListNode node);
static DualLinkListNode dual_link_list_remove(DualLinkList *list, int i);
static int dual_link_list_length(DualLinkList *list);
static int dual_link_list_find(DualLinkList *list, const DualLinkListNode node);
static bool dual_link_list_get(DualLinkList *list, int i, DualLinkListNode *node);
static void dual_link_list_begin(DualLinkList *list);
static void dual_link_list_next(DualLinkList *list);
static void dual_link_list_pre(DualLinkList *list);
static bool dual_link_list_end(DualLinkList *list);
static DualLinkListNode dual_link_list_current(DualLinkList *list);

DualLinkListNode dual_link_list_position(DualLinkList *list, int position)
{
    dual_link_list_def *obj = (dual_link_list_def *)list;
    struct dual_link_list_node *current = (struct dual_link_list_node *)(&(obj->head));

    for (int i = 0; i < position; i++)
    {
        current = current->next;
    }
    return current;
}

DualLinkList *dual_link_list_create(list_type_t type, DualLinkList* list)
{
    static list_vtable_def s_dual_link_list_vtable = {
            .insert = dual_link_list_insert,
            .remove = dual_link_list_remove,
            .find = dual_link_list_find,
            .get = dual_link_list_get,
            .length = dual_link_list_length,
            .destroy = dual_link_list_destroy,
            .begin = dual_link_list_begin,
            .end = dual_link_list_end,
            .next = dual_link_list_next,
            .pre = dual_link_list_pre,
            .current = dual_link_list_current};

    dual_link_list_def *ret = NULL;

    if(type == LIST_DYNAMIC)
    {
        ret = malloc(sizeof(dual_link_list_def));
        if (ret)
        {
            ret->length = 0;
            ret->vtable = &s_dual_link_list_vtable;
            ret->head.next = NULL;
            ret->head.pre = NULL;
            ret->list_type = type;
        }
    }
    else if(list)
    {
        ret = (dual_link_list_def *)list;
        ret->length = 0;
        ret->vtable = &s_dual_link_list_vtable;
        ret->head.next = NULL;
        ret->head.pre = NULL;
        ret->list_type = type;
    }

    return (DualLinkList *)ret;
}

static bool dual_link_list_insert(DualLinkList *list, int i, const DualLinkListNode node)
{
    bool ret = true;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i <= obj->length) && node)
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        if (current)
        {
            struct dual_link_list_node *next = current->next;
            ((struct dual_link_list_node *)node)->next = next;
            ((struct dual_link_list_node *)node)->pre = (current == &(obj->head)) ? (NULL) : (current);
            current->next = node;
            ++obj->length;
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

static DualLinkListNode dual_link_list_remove(DualLinkList *list, int i)
{
    DualLinkListNode ret = NULL;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->length))
    {
        struct dual_link_list_node *current = dual_link_list_position(obj, i);
        struct dual_link_list_node *toDel = (current) ? (current->next) : (NULL);
        struct dual_link_list_node *next = (toDel) ? (toDel->next) : (NULL);
        if (current && toDel)
        {
            ret = toDel;
            current->next = next;
            --obj->length;
            if (next)
            {
                next->pre = toDel->pre;
            }
        }
    }
    return ret;
}

static void dual_link_list_destroy(DualLinkList *list)
{
    if(((dual_link_list_def *)list)->list_type == LIST_DYNAMIC)
    {
        free(list);
    }
}

static int dual_link_list_length(DualLinkList *list)
{
    return ((dual_link_list_def *)list)->length;
}

static void dual_link_list_begin(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->current = ((dual_link_list_def *)list)->head.next;
    }
}

static void dual_link_list_next(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->current = ((dual_link_list_def *)list)->current->next;
    }
}

static void dual_link_list_pre(DualLinkList *list)
{
    if (list)
    {
        ((dual_link_list_def *)list)->current = ((dual_link_list_def *)list)->current->pre;
    }
}

static bool dual_link_list_end(DualLinkList *list)
{
    bool ret = false;

    if (list)
    {
        dual_link_list_def *obj = list;
        ret = (obj->current == NULL);
    }

    return ret;
}

static DualLinkListNode dual_link_list_current(DualLinkList *list)
{
    return (list) ? (((dual_link_list_def *)list)->current) : (NULL);
}

static int dual_link_list_find(DualLinkList *list, const DualLinkListNode node)
{
    int ret = -1;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && node)
    {
        struct dual_link_list_node *current = &(obj->head);

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

static bool dual_link_list_get(DualLinkList *list, int i, DualLinkListNode *node)
{
    bool ret = true;
    dual_link_list_def *obj = (dual_link_list_def *)list;

    if (obj && (i >= 0) && (i < obj->length) && node)
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