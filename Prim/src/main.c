#include <malloc.h>
#include <stdio.h>
#include "lib.h"


int main() {
    int numOfVertices, numOfEdges;
    FILE* fp;
    fp = fopen("in.txt", "r");
    fscanf(fp,"%d %d", &numOfVertices, &numOfEdges);
    badInput(numOfVertices, numOfEdges);
    GRAPH* edges; // array of edges
    int* comp = (int*)malloc(sizeof(int) * (numOfVertices + 1)); // array of components
    for (int i = 0; i <= numOfVertices; ++i)
        *(comp + i) = out;

    edges = inputData(fp, numOfEdges, numOfVertices);
    qsort(edges, numOfEdges, sizeof(GRAPH), (int (*)(const void *, const void *)) cmp);
    Prim(edges, comp, numOfEdges, numOfVertices);
    free(comp);
    if (edges != NULL)
        free(edges);
    fclose(fp);
    return 0;
}
