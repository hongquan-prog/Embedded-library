//
// Created by lhq on 2022/2/18.
//

#include "Queue.h"

Queue *queue_create(list_type_t type, Queue *queue)
{
    return dual_circle_list_create(type, queue);
}

bool queue_is_contained(Queue *queue, const QueueNode node)
{
    return (list_find(queue, node) != -1);
}

bool queue_add(Queue *queue, QueueNode node)
{
    return list_push_back(queue, node);
}

QueueNode queue_front(Queue *queue)
{
    return ((dual_link_list_def *)queue)->head.next;
}

QueueNode queue_remove(Queue *queue)
{
    return list_pop_front(queue);
}

int queue_length(Queue *queue)
{
    return list_length(queue);
}

void queue_rotate(Queue *queue)
{
    if (list_length(queue) > 0)
    {
        QueueNode *node = queue_remove(queue);

        queue_add(queue, node);
    }
}