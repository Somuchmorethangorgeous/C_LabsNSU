//
// Created by roxxa on 21.04.2020.
//

#ifndef TOPOLOGICSORT_LIB_H
#define TOPOLOGICSORT_LIB_H

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define white 0
#define grey 1
#define black 2
#define MAX_NUM_VERTICES 1000
#define GRAPH struct list1


GRAPH{
    short vertex;
    GRAPH* next;
};


typedef enum CheckData{
    success,
    badVertex,
    badNumberOfVertices,
    badNumberOfEdges,
    badNumberOfLines,
    impossibleToSort
} workingResult;


void push(GRAPH* top, short vertex);
int pop(GRAPH*top);
int isNotEmpty(GRAPH* vertex);
int checkConditions(const int inf, const int value, const int sup);
void printStack(GRAPH* top);
void dfs(GRAPH* AdjacencyList, short* visited, short curVertex, workingResult* status);
GRAPH* initAdjacencyList(FILE*fp, short** visited, const int numOfVertices, const int numOfEdges, workingResult* status);
void PrintStatusResult(const workingResult status);




#endif //TOPOLOGICSORT_LIB_H
