//
// Created by lhq on 2022/2/15.
//

#ifndef EMBEDDED_LINKLIST_H
#define EMBEDDED_LINKLIST_H

#include "List.h"

typedef void* LinkListNode;
typedef void  LinkList;

typedef void LinkList;
typedef void *LinkListNode;

struct link_list_node
{
    struct link_list_node *next;
};

typedef struct
{
    // 虚函数表指针必须放在最前面四个字节
    const list_vtable_def *m_vtable;
    struct link_list_node *m_current;
    struct link_list_node m_head;
    int m_length;
    bool m_is_static;
} link_list_def;

LinkList *link_list_create(list_create_t type, LinkList* list);
bool link_list_insert(LinkList *list, int i, const LinkListNode node);
LinkListNode link_list_remove(LinkList *list, int i);
int link_list_length(LinkList *list);
int link_list_find(LinkList *list, const LinkListNode node);
bool link_list_get(LinkList *list, int i, LinkListNode *node);
void link_list_destroy(LinkList *list);
LinkListNode link_list_position(LinkList *list, int position);

void link_list_begin(LinkList *list);
void link_list_next(LinkList *list);
bool link_list_end(LinkList *list);
LinkListNode link_list_current(LinkList *list);

#endif //EMBEDDED_LINKLIST_H
