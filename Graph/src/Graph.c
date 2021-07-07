#include "Graph.h"

static Head* s_graph_list;

void graphCreate()
{
    s_graph_list = listCreate();
}

bool vertexAdd(int n)
{
    bool ret = true;

    for(int i = 0; i < n; i++)
    {
        Vertex* vertex = MALLOC_FUNC(sizeof(Vertex));
        if(vertex)
        {
            vertex->edge_list = listCreate();
            ret = ret && listInsert(s_graph_list, listLength(s_graph_list), vertex);
        }
    }
    
    return false;
}

int vertexCount()
{
    return listLength(s_graph_list);
}

int edgeCount()
{
    int ret = 0;
    for(int i = 0; i < listLength(s_graph_list); i++)
    {
        Vertex* vertex = NULL;
        listGet(s_graph_list, i, (void **)&vertex);
        ret += listLength(vertex->edge_list);
    }
    return ret;
}

bool vertexSet(int index, VERTEX_TYPE value)
{
    bool ret = true;
    Vertex* vertex = NULL;

    if(false != listGet(s_graph_list, index, (void **)&vertex))
        vertex->value = value;
    else
        ret = false;

    return ret;
}

bool vertexGet(int index, VERTEX_TYPE* result)
{
    bool ret = true;
    Vertex* vertex = NULL;

    if(false != listGet(s_graph_list, index, (void **)&vertex))
        *result = vertex->value;
    else
        ret = false;

    return ret;
}

int vertexFind(int data, Vertex** ret)
{
    int index = -1;

    *ret = NULL;
    for(int i = 0; i < listLength(s_graph_list); i++)
    {
        Vertex* vertex = NULL;
        listGet(s_graph_list, i, (void **)&vertex);
        if(vertex && (vertex->value == data))
        {
            *ret = vertex;
            index = i;
        }
    }

    return index;
}

static int edgeFind(int begin, int end, Edge** ret)
{
    int index = -1;

    if((begin >= 0) && (begin < listLength(s_graph_list)) \
    && (end >= 0) && (end < listLength(s_graph_list)))
    {
        Vertex* vertex = NULL;
        listGet(s_graph_list, begin, (void **)&vertex);
        for(int i = 0; i < listLength(vertex->edge_list); i++)
        {
            
            Edge* edge = NULL;
            listGet(vertex->edge_list, i, (void **)&edge);
            if(edge && (begin == edge->begin) && (end == edge->end))
            {
                if(ret)
                {
                    *ret = edge;
                }
                index = i;
            }
        }
    }

    return index;
}


void edgeSet(int begin, int end, EDGE_TYPE data)
{
    Vertex* vertex = NULL;
    listGet(s_graph_list, begin, (void **)&vertex);
    if(vertex)
    {
        Edge* edge = NULL;
        if(-1 != edgeFind(begin, end, &edge))
        {
            edge->data = data;
        }
        else
        {
            Edge* edge = MALLOC_FUNC(sizeof(Edge));
            if(edge)
            {
                edge->begin = begin;
                edge->end = end;
                edge->data = data;
                listInsert(vertex->edge_list, listLength(vertex->edge_list), edge);
            }
        }
    }
}

void edgeRemove(int begin, int end)
{
    Vertex* vertex = NULL;
    Edge* edge = NULL;

    if((begin >= 0) && (begin < listLength(s_graph_list)) \
    && (end >= 0) && (end < listLength(s_graph_list)))
    {
        listGet(s_graph_list, begin, (void **)&vertex);
        int index = edgeFind(begin, end, &edge);
        FREE_FUNC(edge);
        listRemove(vertex->edge_list, index);
    }
}

bool edgeGet(int begin, int end, EDGE_TYPE* result)
{
    bool ret = true;
    Edge* edge = NULL;

    edgeFind(begin, end, &edge);
    if(edge && result)
    {
        *result = edge->data;
    }
    else
        ret = false;
    
    return ret;
}

void edgeClear(Vertex* vertex)
{
    Edge* edge = NULL;

    for(int i = 0; i < listLength(vertex->edge_list); i++)
    {
        listGet(vertex->edge_list, i, (void **)&edge);
        FREE_FUNC(edge);
    }
    listClear(vertex->edge_list);
}

bool vertexRemove()
{
    if(listLength(s_graph_list) > 0)
    {
        Vertex* toDel = NULL;
        int index = listLength(s_graph_list) - 1;
        listGet(s_graph_list, index, (void **)&toDel);
        if(toDel)
        {
            //清空当前顶点所有的边
            edgeClear(toDel);

            //删除当前顶点
            listRemove(s_graph_list, index);
            FREE_FUNC(toDel);

            //删除其他顶点到当前顶点的边
            for(int i = 0; i < listLength(s_graph_list); i++)
            {
                edgeRemove(i, index);
            }
        }
    }
    return false;
}

void graphClear()
{
    while(listLength(s_graph_list))
    {
        vertexRemove();
    }
    listClear(s_graph_list);
}

bool isAdjacent(int begin, int end)
{
    bool ret = true;
    if((begin >= 0) && (begin < listLength(s_graph_list)) \
    && (end >= 0) && (end < listLength(s_graph_list)))
    {
        ret =  (-1 != edgeFind(begin, end, NULL));
    }
    else
    {
        ret = false;
    }
    return ret;
}

int getAdjacent(int begin, int** result)
{
    int count = 0;
    if((begin >= 0) && (begin < listLength(s_graph_list)))
    {
        Vertex* vertex = NULL;
        listGet(s_graph_list, begin, (void **)&vertex);
        if(vertex)
        {
            (*result) = MALLOC_FUNC(listLength(vertex->edge_list));
            int* array = (*result);
            if(array)
            {
                count = listLength(vertex->edge_list);
                for(int i = 0; i < listLength(vertex->edge_list); i++)
                {
                    Edge* edge = NULL;
                    listGet(vertex->edge_list, i, (void **)&edge);
                    array[i] = edge->end;
                }
            }
        }
    }  
    return count;  
}

bool dijkstra(int begin, int end, int* len, int** result, int LIMIT)
{
    bool ret = true;
    int count = 0;

    if((begin >= 0) && (begin < listLength(s_graph_list)) \
    && (end >= 0) && (end < listLength(s_graph_list)))
    {
        bool* mark = MALLOC_FUNC(vertexCount() * sizeof(bool));
        int* path = MALLOC_FUNC(vertexCount() * sizeof(int)); 
        int* dist = MALLOC_FUNC(vertexCount() * sizeof(int));

        for(int i = 0; i < vertexCount(); i++)
        {
            mark[i] = false;
            path[i] = -1;
            if(isAdjacent(begin, i))
            {
                EDGE_TYPE edge = 0;
                edgeGet(begin, i, &edge);
                path[i] = begin;
                dist[i] = edge;
            }
            else
                dist[i] = LIMIT;
        }
        mark[begin] = true;

        for(int i = 0 ;i < vertexCount(); i++)
        {
            int distance = LIMIT;
            int index = -1;

            for(int j = 0; j < vertexCount(); j++)
            {
                if(!mark[j] && (dist[j] < distance))
                {   
                    distance = dist[j];
                    index = j;
                }
            }

            if(index == -1)
            {
                break;
            }
            mark[index] = true;

            for(int j = 0; j < vertexCount(); j++)
            {
                if(!mark[j] && isAdjacent(index, j))
                {
                    EDGE_TYPE edge = 0;
                    edgeGet(index, j, &edge);
                    if(edge + dist[index] < dist[j])
                    {
                        dist[j] = edge + dist[index];
                        path[j] = index;
                    }
                }
            }
        }

        int* array = MALLOC_FUNC(sizeof(int) * vertexCount());
        array[count++] = end;
        for(int i = path[end]; i != -1; i = path[i])
        {
            array[count++] = i;
        }

        for(int i = 0; i < count / 2; i++)
        {
            int temp = array[i];
            array[i] = array[count - i - 1];
            array[count - i - 1] = temp;
        }

        if(count >= 2)
        {
            if(result && len)
            {
                *result = array;
                *len = count;
            }
            else
            {
                *result = NULL;
                FREE_FUNC(array);
                ret = false;
            }
        }
        else
        {
            *result = NULL;
            FREE_FUNC(array);
            ret = false;
        }

        FREE_FUNC(mark);
        FREE_FUNC(path);
        FREE_FUNC(dist);
    }
    else
        ret = false;

    return ret;
}