//
// Created by roxxa on 21.04.2020.
//
#include "lib.h"

void push(GRAPH* top, short vertex){
    GRAPH* tmp = (GRAPH*)malloc(sizeof(GRAPH));
    tmp -> vertex = vertex;
    tmp -> next = top -> next;
    top -> next = tmp;
}


int pop(GRAPH*top){
    GRAPH* delElem = top -> next;
    int value = delElem -> vertex;
    top -> next = top -> next -> next;
    free(delElem);
    return value;
}


int isNotEmpty(GRAPH* vertex){
    return (int)(vertex -> next);
}


int checkConditions(const int inf, const int value, const int sup){
    return (value >= inf && value <= sup);
}


void printStack(GRAPH* top){
    while(isNotEmpty(top)) {
        int vertex = pop(top);
        printf("%d ", vertex);
    }
}


void dfs(GRAPH* AdjacencyList, short* visited, short curVertex, workingResult* status){
    visited[curVertex] = grey;
    while(isNotEmpty(&AdjacencyList[curVertex]) && !visited[0]){
        short dest = AdjacencyList[curVertex].next->vertex;
        if (visited[dest] == white){
            pop(&AdjacencyList[curVertex]);
            dfs(AdjacencyList, visited, dest, status);
        }
        else if (visited[dest] == black)
            pop(&AdjacencyList[curVertex]);
        else {                                  // if we're in the vertex, which we started dfs - we have a cycle
            *status = impossibleToSort;
            ++visited[0];
        }
    }
    visited[curVertex] = black;
    push(&AdjacencyList[0], curVertex);
}



GRAPH* initAdjacencyList(FILE*fp, short** visited, const int numOfVertices, const int numOfEdges, workingResult* status){
    GRAPH* AdjacencyList = (GRAPH*)malloc(sizeof(GRAPH) * (numOfVertices + 1));
    for(int i = 0; i <= numOfVertices; ++i)
        AdjacencyList[i].next = NULL;
    *visited = (short*)malloc(sizeof(short) * (numOfVertices + 1));
    memset(*visited, 0, sizeof(short) * (numOfVertices + 1));
    for(int i = 0; i < numOfEdges; ++i){
        int start, end, checkInput;
        checkInput = fscanf(fp, "%d %d", &start, &end);

        if (checkInput != 2){
            *status = badNumberOfLines;
            break;
        }

        if (!checkConditions(0,start, MAX_NUM_VERTICES) || !checkConditions(0,start, numOfVertices)){
            *status = badVertex;
            break;
        }

        if (!checkConditions(0,end, MAX_NUM_VERTICES) || !checkConditions(0,end, numOfVertices)){
            *status = badVertex;
            break;
        }

        push(&AdjacencyList[start], (short)end);
    }
    return AdjacencyList;
}



void PrintStatusResult(const workingResult status){
    switch(status){
        case badVertex:
            printf("bad vertex");
            break;
        case badNumberOfVertices:
            printf("bad number of vertices");
            break;
        case badNumberOfEdges:
            printf("bad number of edges");
            break;
        case badNumberOfLines:
            printf("bad number of lines");
            break;
        case impossibleToSort:
            printf("impossible to sort");
            break;
        default:
            break;
    }
}