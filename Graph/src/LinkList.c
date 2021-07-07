#include "Graph.h"

Head* listCreate()
{
    Head * header = (Head *)MALLOC_FUNC(sizeof(Head));

    if(header) 
    {
        header->length = 0;
        header->next = NULL;
    }

    return header;
}

bool listInsert(Head* head, int i, void* value)
{
    bool ret = ((head) && (i >= 0) && (i <= head->length));
    
    if(ret)
    {
        Node* slider = (Node*)head;
        Node* node = (Node*)MALLOC_FUNC(sizeof(Node));
        if(node)
        {
            int j = 0;
            node->value = value;
            for( j = 0; j <= head->length; j++)
            {
                if(j == i)
                {
                    node->next = slider->next;
                    slider->next = node;
                    head->length++;
                    break;
                }
                slider = slider->next;
            }

            if(i != j)
            {
                FREE_FUNC(node);
                ret = false;
            }
        }
        else
            ret = false;
    }
    return ret;
}
       
bool listRemove(Head* head, int i)
{
    bool ret = (head) && (i >= 0) && (i < head->length);

    if(ret)
    {
        int j = 0;
        Node* pre = (Node *)head;
        Node* next = pre->next;
        
        for(j = 0; j < head->length; j++)
        {
            if(j == i)
            {
                Node* toDel = next;
                pre->next = toDel->next;
                FREE_FUNC(toDel);
                head->length--;
                break;
            }
            pre = next;
            next = next->next;
        }

        if(j != i)
        {
            ret = false;
        }
    }
    return ret;
}

int listFind(Head* head, void* value)
{
    int ret = 0;
    Node* slider = (head) ? (head->next) : (NULL);

    while (slider != NULL)
    {
        if(slider->value == value)
        {
            break;
        }
        slider = slider->next;
        ret ++;
    }
    
    return ret;
}

bool listGet(Head* head, int i, void** value)
{
    bool ret = false;
    Node* slider = (head) ? (head->next) : (NULL);

    for(int j = 0; j < head->length; j++)
    {
        if(j == i)
        {
            ret = true;
            *value = slider->value;
            ret = true;
            break;
        }
        slider = slider->next;
    }
    
    return ret;
}

bool listSet(Head* head, int i, void* value)
{
    bool ret = false;
    Node* slider = (head) ? (head->next) : (NULL);

    for(int j = 0; j < head->length; j++)
    {
        if(j == i)
        {
            ret = true;
            slider->value = value;
            break;
        }
        slider = slider->next;
    }
    
    return ret;
}

int listLength(Head* head)
{
    return (head) ? (head->length) : (0);
}

void listClear(Head* head)
{
    Node* slider = (head) ? (head->next) : (NULL);

    while (slider != NULL)
    {
        Node* toDel = slider;
        slider = slider->next;
        FREE_FUNC(toDel);
    }
    FREE_FUNC(head);
}