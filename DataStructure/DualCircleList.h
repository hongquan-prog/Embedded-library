//
// Created by lhq on 2022/2/16.
//

#ifndef EMBEDDED_DUALCIRCLELIST_H
#define EMBEDDED_DUALCIRCLELIST_H

#include "DualLinkList.h"

typedef void DualCircleList;
typedef void *DualCircleListNode;

DualCircleList *dual_circle_list_create(list_create_t type, DualCircleList* list);
void dual_circle_list_destroy(DualCircleList *list);
bool dual_circle_push_back(DualCircleList *list, DualCircleListNode node);
bool dual_circle_push_front(DualCircleList *list, DualCircleListNode node);
DualCircleListNode dual_circle_pop_front(DualCircleList *list);
DualCircleListNode dual_circle_pop_back(DualCircleList *list);
bool dual_circle_list_insert(DualCircleList *list, int i, const DualCircleListNode node);
DualCircleListNode dual_circle_list_remove(DualCircleList *list, int i);

#endif //EMBEDDED_DUALCIRCLELIST_H
