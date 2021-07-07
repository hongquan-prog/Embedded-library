#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MALLOC_FUNC(n) malloc(n)
#define FREE_FUNC(p) free(p)

typedef int EDGE_TYPE;
typedef int VERTEX_TYPE;

typedef struct Node
{
    void* value;
    struct Node* next;
}Node;

typedef struct 
{
    int length;
    Node* next;
}Head;

typedef struct Edge
{
    int begin;
    int end;
    EDGE_TYPE data;
}Edge;

typedef struct Vertex
{
    VERTEX_TYPE value;
    Head* edge_list;
}Vertex;

Head* listCreate();
bool listInsert(Head* head, int i, void* value);
bool listRemove(Head* head, int i);
int listFind(Head* head, void* value);
bool listGet(Head* head, int i, void** value);
bool listSet(Head* head, int i, void* value);
int listLength(Head* head);
void listClear(Head* head);

void graphCreate();
bool vertexAdd(int n);
bool vertexSet(int index, VERTEX_TYPE value);
bool vertexGet(int index, VERTEX_TYPE* result);
bool vertexRemove();
void edgeSet(int begin, int end, EDGE_TYPE data);
void edgeRemove(int begin, int end);
bool edgeGet(int begin, int end, EDGE_TYPE* result);
int vertexCount();
int edgeCount();
void graphClear();
int getAdjacent(int begin, int** result);
bool dijkstra(int begin, int end, int* len, int** result, int LIMIT);

