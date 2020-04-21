#include <stdio.h>
#include "lib.h"



int main() {
    FILE*fp = fopen("in.txt", "r");
    int numOfVertices, numOfEdges;
    GRAPH* AdjacencyList = NULL;
    short* visited = NULL;
    workingResult status;
    status = success;

    int checkInput = fscanf(fp, "%d %d", &numOfVertices, &numOfEdges);
    if (checkInput != 2){
        status = badNumberOfLines;
        PrintStatusResult(status);
    }

    int maxNumOfEdges = numOfVertices * (numOfVertices - 1) / 2;
    if  (!checkConditions(0, numOfVertices, MAX_NUM_VERTICES))
        status = badNumberOfVertices;

    if  (!checkConditions(0, numOfEdges, maxNumOfEdges))
        status = badNumberOfEdges;

    if  (status == success)
        AdjacencyList = initAdjacencyList(fp, &visited, numOfVertices, numOfEdges, &status);

    if  (status == success) {
        for (short vertex = 1; vertex <= (short) numOfVertices; ++vertex) {  // starting dfs from all vertex (from smallest to highest),
                                                                             // which haven't been visited
            if (visited[vertex] == white)
                dfs(AdjacencyList, visited, vertex, &status);
        }
    }

    if (status == success)
        printStack(&AdjacencyList[0]);
    else
        PrintStatusResult(status);

    if (AdjacencyList != NULL)
        free(AdjacencyList);
    if (visited != NULL)
        free(visited);
    fclose(fp);
    return 0;
}
