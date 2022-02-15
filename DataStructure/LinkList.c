//
// Created by lhq on 2022/2/15.
//

#include "LinkList.h"

typedef struct
{
    // 虚函数表指针必须放在最前面四个字节
    const list_vtable_def *m_vtable;
    // 用于保存遍历当前位置
    struct link_list_node *m_current;
    // 头节点
    struct link_list_node m_head;
    // 链表长度
    int m_length;
} link_list_def;

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

LinkList *link_list_create()
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

    link_list_def *list = malloc(sizeof(link_list_def));
    if (list)
    {
        list->m_current = NULL;
        list->m_length = 0;
        list->m_vtable = &s_link_list_vtable;
        list->m_head.next = NULL;
    }
    return (LinkList *)list;
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
    free(list);
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
