#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <limits.h>

#define EDGE struct list
#define MAX_VERTEX_NUM 5000


typedef enum CheckData{
    success,
    noInput,
    badVertex,
    badLength,
    noSpanningTree,
    badNumOfLines,
    badNumOfVertices,
    badNumOfEdges
} workingResult;


EDGE{
    short begin;
    short end;
    int weight;
    bool isInSpanningTree;
};




int checkNumberOfEdges(const int numberOfEdges, const int numberOfVertices){
    return (numberOfEdges <= numberOfVertices * (numberOfVertices - 1) / 2);
}


int isBetweenBoundaries(const long long int checkingValue, const int leftBoundary, const int rightBoundary){
    return (checkingValue >= leftBoundary && checkingValue <= rightBoundary);
}


void printSpanningTree(const int numberOfEdges, EDGE*listOfEdges){
    for (int i = 0; i < numberOfEdges; ++i){
        if (listOfEdges[i].isInSpanningTree == true)
            printf("%d %d\n", listOfEdges[i].begin, listOfEdges[i].end);
    }
}


void makeSet(const int numberOfVertices, short* parents, short* rank){
    // each vertex is the root, so rank = 0
    for (int i = 0; i < numberOfVertices; ++i){
        parents[i] = (short) i;
        rank[i] = 0;
    }
}


short findSet(const short el, short* parents){
    if (parents[el] == el)
        return el;
    return parents[el] = findSet(parents[el], parents); // recursive search the root and updating vertices' parents
}


void setUnion(short begin, short end, short* parents, short* rank){
    begin = findSet(begin, parents);
    end = findSet(end, parents);
    if (rank[begin] < rank[end]) // attach first to the second tree, which has bigger rank (size)
        parents[begin] = end;
    else {
        parents[end] = begin;
        if (rank[begin] == rank[end])
            ++rank[begin];
    }
}



EDGE* findMinSpanningTree(const int numberOfEdges, const int numberOfVertices, workingResult *status, EDGE*listOfEdges){
    if (numberOfVertices == 0 || (numberOfEdges == 0 && numberOfVertices != 1)){
        *status = noSpanningTree;
        return listOfEdges;
    }
    short* parents = malloc(sizeof(short) * numberOfVertices);
    short* rank = malloc(sizeof(short) * numberOfVertices);
    makeSet(numberOfVertices, parents, rank);
    int counterOfVisitedVertex = 1;

    for (int idx = 0; idx < numberOfEdges; ++idx){
        short begin = (short) (listOfEdges[idx].begin - 1);
        short end = (short) (listOfEdges[idx].end - 1);

        if (findSet(begin, parents) != findSet(end, parents)){ // if ends of the edge are in different trees
            listOfEdges[idx].isInSpanningTree = true;
            ++counterOfVisitedVertex;
            setUnion(begin, end, parents, rank);
            if (counterOfVisitedVertex == numberOfVertices)
                break;
        }
    }

    if (counterOfVisitedVertex != numberOfVertices)
        *status = noSpanningTree;

    free(parents);
    free(rank);
    return listOfEdges;
}



EDGE* inputEdges(FILE*fp, const int numberOfVertices, const int numberOfEdges, workingResult* controlValue){
    EDGE* listOfEdges = malloc(sizeof(EDGE) * numberOfEdges);
    for (int idx = 0; idx < numberOfEdges; ++idx){
        EDGE currentEdge;
        int checkInput;
        long long int currentWeight;
        checkInput = fscanf(fp,"%hd %hd %lld", &currentEdge.begin, &currentEdge.end,  &currentWeight);

        if (checkInput != 3) {
            *controlValue = noInput;
            break;
        }

        if (!isBetweenBoundaries(currentWeight, 0, INT_MAX)) {
           *controlValue = badLength;
            break;
        }

        if (!isBetweenBoundaries(currentEdge.begin, 1, numberOfVertices)  || !isBetweenBoundaries(currentEdge.end, 1, numberOfVertices)){
            *controlValue = badVertex;
            break;
        }

        currentEdge.weight = (int)currentWeight;
        currentEdge.isInSpanningTree = false;
        listOfEdges[idx] = currentEdge;
    }
    return listOfEdges;
}


int comp (const int *i, const int *j){
    return ((EDGE*)i) -> weight - ((EDGE*)j) -> weight;
}


void printMessageByStatus(const workingResult status){
    switch (status){
        case noInput:
            printf("bad number of lines");
            break;
        case badVertex:
            printf("bad vertex");
            break;
        case badLength:
            printf("bad length");
            break;
        case badNumOfLines:
            printf("bad number of lines");
            break;
        case badNumOfVertices:
            printf("bad number of vertices");
            break;
        case badNumOfEdges:
            printf("bad number of edges");
            break;
        case noSpanningTree:
            printf("no spanning tree");
            break;
        default:
            break;
    }

}


int main() {
    FILE*fp = fopen("in.txt", "r");
    int numberOfVertices, numberOfEdges;
    workingResult status = success;
    int checkInput = fscanf(fp, "%d %d", &numberOfVertices, &numberOfEdges);
    if (checkInput != 2)
        status = badNumOfLines;
    else if (!isBetweenBoundaries(numberOfVertices, 0, MAX_VERTEX_NUM))
        status = badNumOfVertices;
    else if (!checkNumberOfEdges(numberOfEdges, numberOfVertices))
        status = badNumOfEdges;

    EDGE* listOfEdges = NULL;
    if (status == success){
        listOfEdges = inputEdges(fp, numberOfVertices, numberOfEdges, &status);
        qsort(listOfEdges, numberOfEdges, sizeof(EDGE), (int(*) (const void *, const void *)) comp);
    }

    if (status == success)
        listOfEdges = findMinSpanningTree(numberOfEdges, numberOfVertices, &status, listOfEdges);

    if (status == success)
        printSpanningTree(numberOfEdges, listOfEdges);
    else
        printMessageByStatus(status);

    if (listOfEdges != NULL)
        free(listOfEdges);
    fclose(fp);
    return 0;
}