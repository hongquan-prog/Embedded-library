//
// Created by lhq on 2022/2/18.
//

#ifndef EMBEDDED_QUEUE_H
#define EMBEDDED_QUEUE_H

#include "DualCircleList.h"

typedef void Queue;
typedef void *QueueNode;

Queue *queue_create(list_type_t type, Queue *queue);
bool queue_is_contained(Queue *queue, const QueueNode node);
bool queue_add(Queue *queue, QueueNode node);
QueueNode queue_front(Queue *queue);
QueueNode queue_remove(Queue *queue);
int queue_length(Queue *queue);
void queue_rotate(Queue *queue);

#endif // EMBEDDED_QUEUE_H
