//
// Created by lhq on 2022/2/16.
//

#ifndef EMBEDDED_DUALCIRCLELIST_H
#define EMBEDDED_DUALCIRCLELIST_H

#include "DualLinkList.h"

typedef void DualCircleList;
typedef void *DualCircleListNode;

DualCircleList *dual_circle_list_create(list_type_t type, DualCircleList* list);

#endif //EMBEDDED_DUALCIRCLELIST_H
