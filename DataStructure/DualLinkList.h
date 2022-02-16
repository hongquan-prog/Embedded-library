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
    list_vtable_def* m_vtable;
    struct dual_link_list_node* m_current;
    struct dual_link_list_node m_head;
    int m_length;
    bool m_is_static;
} dual_link_list_def;

DualLinkList *dual_link_list_create(list_create_t type, DualLinkList* list);
bool dual_link_list_insert(DualLinkList *list, int i, const DualLinkListNode node);
DualLinkListNode dual_link_list_remove(DualLinkList *list, int i);
int dual_link_list_length(DualLinkList *list);
int dual_link_list_find(DualLinkList *list, const DualLinkListNode node);
bool dual_link_list_get(DualLinkList *list, int i, DualLinkListNode *node);
void dual_link_list_destroy(DualLinkList *list);
DualLinkListNode dual_link_list_position(DualLinkList *list, int position);
int dual_link_list_find(DualLinkList *list, const DualLinkListNode node);
bool dual_link_list_get(DualLinkList *list, int i, DualLinkListNode *node);

void dual_link_list_begin(DualLinkList *list);
void dual_link_list_next(DualLinkList *list);
void dual_link_list_pre(DualLinkList *list);
bool dual_link_list_end(DualLinkList *list);
DualLinkListNode dual_link_list_current(DualLinkList *list);

#endif //DATA_STRUCTURE_DUALLINKLIST_H
