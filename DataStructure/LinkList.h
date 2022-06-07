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
    const list_vtable_def *vtable;
    struct link_list_node *current;
    struct link_list_node head;
    int length;
    list_type_t list_type;
} link_list_def;

LinkList *link_list_create(list_type_t type, LinkList* list);

#endif //EMBEDDED_LINKLIST_H
