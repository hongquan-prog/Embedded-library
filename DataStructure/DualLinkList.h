//
// Created by lhq on 2022/2/16.
//

#ifndef DATA_STRUCTURE_DUALLINKLIST_H
#define DATA_STRUCTURE_DUALLINKLIST_H

#include "List.h"

typedef void* DualLinkListNode;
typedef void  DualLinkList;

struct dual_link_list_node
{
    struct dual_link_list_node *next;
    struct dual_link_list_node *pre;
};

typedef struct
{
    // 虚函数表指针必须放在最前面四个字节
    list_vtable_def* vtable;
    struct dual_link_list_node* current;
    struct dual_link_list_node head;
    int length;
    list_type_t list_type;
} dual_link_list_def;

DualLinkList *dual_link_list_create(list_type_t type, DualLinkList* list);

#endif //DATA_STRUCTURE_DUALLINKLIST_H
