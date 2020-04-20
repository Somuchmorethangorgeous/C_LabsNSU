//
// Created by roxxa on 01.04.2020.
//

#include <stdio.h>
#include "lib.h"


GRAPH* popElem(QUEUE* q){
    GRAPH *delElem;
    delElem = q -> head;
    q -> head = q -> head -> next;
    return delElem;
}


void popElementStack(GRAPH* top){
    GRAPH* delElem = top -> next;
    top -> next = top -> next -> next;
    free(delElem);
}


void ClearQ(QUEUE* q){
    while (q -> head)
        popElem(q);
    q -> head = q -> tail = NULL;
}


void pushElementQueue(QUEUE* q, GRAPH* edge){
    edge -> next = NULL;
    if (q -> head && q -> tail){
        q -> tail -> next = edge;
        q -> tail = edge;
    }
    else
        q -> head = q -> tail = edge;
}


void pushElementStack(GRAPH* top, int firstP, int secondP, int weight){
    GRAPH* newElem =(GRAPH*)malloc(sizeof(GRAPH));
    newElem -> start = firstP;
    newElem -> end = secondP;
    newElem -> weight = weight;
    top -> next = newElem;
}


int updQueue(QUEUE* q, GRAPH* edges, int* comp,  int* edgInCarcass){
    GRAPH *previous, *current;
    previous = current = q -> head;
    while (current){

        int start = current -> start;
        int end = current -> end;

        if (*(comp + start) != *(comp + end)){
            (edges + *edgInCarcass) -> next = popElem(q);
            *(comp + start) =  *(comp + end) = in;
            *edgInCarcass += 1;
            previous -> next = current -> next;
            return updQueue(q, edges, comp, edgInCarcass);
        }
        else if (*(comp + start) && *(comp + end)){
            if (current == q -> head)
                free(popElem(q));
            else
                popElementStack(previous);
        }
        previous = current;
        current  = current -> next;
    }
    return 0;
}


void Prim(GRAPH* edges, int* comp, int numEdge, int vertex){
    int edgInCarcass = 0;
    QUEUE* qbody = (QUEUE*)malloc(sizeof(QUEUE));
    qbody -> tail = qbody -> head = NULL;
    *(comp+1) = in;

    for (int j = 0; j < numEdge; ++j){

        int start = (edges + j) -> next -> start;
        int end = (edges + j) -> next -> end;

        if (*(comp + start) != *(comp + end)){ // if one point in the tree and the other isn't - push it
            if (j != edgInCarcass)
                (edges + edgInCarcass)-> next = (edges + j) -> next;
            *(comp + start) =  *(comp + end) = in;
            ++edgInCarcass;
            if (qbody -> head)
                updQueue(qbody, edges, comp, &edgInCarcass); // after every pushing edge - update the queue
        }
        else if (!*(comp + start) && !*(comp + end)) // if both point aren't in the tree
            pushElementQueue(qbody, (edges + j) -> next);
        else  // if both points have already pushed in the tree
            free((edges + j) -> next);
    }
    ClearQ(qbody);
    if (edgInCarcass == vertex - 1) {
        for (int i = 0; i < edgInCarcass; ++i)
            printf("%d %d\n", (edges + i) -> next -> start, (edges + i) -> next -> end);
    }
    else
        printf("no spanning tree");
    free(qbody);
}


int cmp (const void *i, const int *j){
    return ((GRAPH*)i) -> next -> weight - ((GRAPH*)j) -> next -> weight;
}



GRAPH* inputData(FILE*fp, int numOfEdges, int numOfVertices){
    GRAPH* edges = (GRAPH*)malloc(sizeof(GRAPH) * numOfEdges); // array of edges
    for(int i = 0; i < numOfEdges; ++i){

        int firstPoint, secondPoint, weight;
        int checkEdge = fscanf(fp, "%d %d %d", &firstPoint, &secondPoint, &weight);

        if (checkEdge != 3){
            printf("bad number of lines");
            exit (0);
        }
        badVertex(firstPoint, secondPoint, weight, numOfVertices);
        pushElementStack(edges + i, firstPoint, secondPoint, weight);
    }
    return edges;
}


void badVertex(int start, int end, int len, int numOfVertices){
    if (start + end < 2 || start + end > 2 * MAX_NUM_VERTICES || start + end >= 2 * numOfVertices) {
        printf("bad vertex");
        exit(0);
    }
    else if (len < 0){
        printf("bad length");
        exit(0);
    }
}


void badInput(int vertex, int edge){
    if (vertex > MAX_NUM_VERTICES || vertex < 0){
        printf("bad number of vertices");
        exit(0);
    }
    else if (edge > (vertex * (vertex+1)/2) || edge < 0){
        printf("bad number of edges");
        exit(0);
    }
    else if (edge == 0 && vertex != 1){
        printf("no spanning tree");
        exit(0);
    }
}
