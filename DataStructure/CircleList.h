#ifndef CIRCLE_LIST_H
#define CIRCLE_LIST_H

#include "LinkList.h"

typedef void *CircleListNode;
typedef void CircleList;

CircleList *circle_list_create(list_type_t type, LinkList *list);

#endif