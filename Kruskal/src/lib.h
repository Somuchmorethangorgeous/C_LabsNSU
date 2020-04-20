//
// Created by roxxa on 01.04.2020.
//

#ifndef PRIM_LIB_H
#define PRIM_LIB_H

#define MAX_NUM_VERTICES 5000
#define out 0
#define in 1
#define GRAPH struct list
#define QUEUE struct list1
#include <stdlib.h>


GRAPH{
    int start, end, weight;
    GRAPH* next;
};


QUEUE{
    GRAPH* head;
    GRAPH* tail;
};


GRAPH* popElem(QUEUE* q);
void popElementStack (GRAPH* top);
void ClearQ(QUEUE* q);
void pushElementQueue(QUEUE* q, GRAPH* edge);
void pushElementStack(GRAPH* top, int firstP, int secondP, int weight);
int updQueue(QUEUE* q, GRAPH* edges, int* comp,  int* edgInCarcass);
void Prim(GRAPH* edges, int* comp, int numEdge, int numOfVertices);
int cmp (const void *i, const int *j);
GRAPH* inputData(FILE*fp, int numOfEdges, int numOfVertices);
void badVertex(int start, int end, int len, int numOfVertices);
void badInput(int vertex, int edge);

#endif //PRIM_LIB_H
