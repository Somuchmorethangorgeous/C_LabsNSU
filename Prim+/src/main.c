#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#define HEAP struct list
#define HeapNode struct list1
#define MAX_NUM_VERTICES 5000


typedef enum CheckData{
    success,
    badVertex,
    badLength,
    badNumOfVertices,
    badNumOfEdges,
    badNumberOfLines,
    noSpanningTree
} workingResult;


HeapNode{
    short int vertex;
    int key;
};


HEAP{
    short int heap_size;
    short int* pos; // array of positions vertices in heap
    HeapNode* array;
};




int parent(int i){
    return (i - 1)/2;
}


int left(int i){
    return (2*i + 1);
}


int right(int i){
    return (2*i + 2);
}


int isInQueue(HEAP*ptr, int vertex){
    return (ptr->pos[vertex] < ptr -> heap_size);
}


int isQueueEmpty(HEAP* ptr){
    return (ptr -> heap_size == 0);
}


void swap(HeapNode*x , HeapNode*y){
    HeapNode tmp = *x;
    *x = *y;
    *y = tmp;
}


void siftUp(HEAP*ptr, int idx){
    while (idx && ptr->array[idx].key < ptr->array[parent(idx)].key){
        HeapNode child = ptr -> array[idx];
        HeapNode upNode = ptr ->array[parent(idx)];

        ptr->pos[child.vertex] =(short) parent(idx);
        ptr->pos[upNode.vertex] = (short) idx;

        swap(&ptr -> array[idx], &ptr ->array[parent(idx)]);
        idx = parent(idx);
    }
}


void minHeapify(HEAP*ptr, int idx){
    int l = left(idx);
    int r = right(idx);
    int smallest = idx;
    if (l < ptr->heap_size && ptr->array[l].key < ptr -> array[idx].key)
        smallest = l;
    if (r < ptr->heap_size && ptr->array[r].key < ptr -> array[smallest].key)
        smallest = r;
    if (smallest != idx){
        HeapNode smallestNode = ptr -> array[smallest];
        HeapNode idxNode = ptr -> array[idx];

        ptr -> pos [smallestNode.vertex] = (short) idx;
        ptr -> pos [idxNode.vertex] = (short) smallest;

        swap(&ptr -> array[smallest], &ptr -> array[idx]);
        minHeapify(ptr, smallest);
    }
}



int extractMin(HEAP* ptr){
    if (isQueueEmpty(ptr))
        return -1;
    if (ptr -> heap_size == 1){
        --ptr -> heap_size;
        return ptr -> array[0].vertex;
    }

    HeapNode root = ptr->array[0];
    int vertex = root.vertex;
    HeapNode lastNode = ptr -> array[ptr->heap_size - 1];
    ptr -> array[0] = lastNode;

    ptr -> pos[lastNode.vertex] = 0;
    ptr -> pos[root.vertex] = (short) (ptr->heap_size -  1);
    --ptr->heap_size;
    minHeapify(ptr, 0);
    return vertex;
}



void decreaseKey(HEAP* ptr, int vertex, int key){
    int i = ptr -> pos[vertex];
    ptr->array[i].key = key;
    siftUp(ptr,i);
}


HeapNode* newNode(int vertex, int key){
    HeapNode* tmp = (HeapNode*)malloc(sizeof(HeapNode));
    tmp -> vertex = (short) vertex;
    tmp -> key = key;
    return tmp;
}


HEAP* initMinHeap(int cap){
    HEAP* ptr = (HEAP*)malloc(sizeof(HEAP));
    ptr -> heap_size = 0;
    ptr -> pos = (short*)malloc(sizeof(short) * cap);
    ptr -> array = (HeapNode*)malloc(sizeof(HeapNode) * cap);
    return ptr;
}


void checkParents(const short* parents, const int numOfVertices, workingResult* status){
    for (int i = 0; i < numOfVertices; ++i) {
        if (parents[i] == -1) {
            *status = noSpanningTree;
            return;
        }
    }
}



void printPath (short* parents, const int numOfVertices){
    for (int i = 1; i < numOfVertices; ++i)
        printf("%d %d\n", i + 1,  parents[i] + 1);
    free(parents);
}


void clearMatrix(int** matrix, int numOfVertices){
    for (int i = 0; i < numOfVertices; ++i)
        free(matrix[i]);
    free(matrix);
}


int isInBoundaries(const long long int value, const int lowestValue, const int highestValue){
    return (value >= lowestValue && value <= highestValue);
}


int** initMatrix(int** matrix, const int numberOfVertices){
    for (int i = 0; i < numberOfVertices; ++i){
        matrix[i] = (int*)malloc(sizeof(int) * numberOfVertices);
        for (int j = 0; j < numberOfVertices; ++j)
            matrix[i][j] = INT_MAX;
    }
    return matrix;
}


short* PrimSpanningTree(int** matrix, const int numOfVertices, const int numOfEdges, workingResult* status){
    int* key = (int*)malloc(sizeof(int) * numOfVertices); // array of shortest distances to the each vertex
    short* shortestParents = (short*)malloc(sizeof(short) * numOfVertices); // from which vertex the shortest distance

    if(numOfVertices == 0 || (numOfEdges == 0 && numOfVertices != 1))
        *status = noSpanningTree;

    HEAP* minHeap = initMinHeap(numOfVertices); // initial minimal heap
    for (int idx = 0; idx < numOfVertices; ++idx){
        shortestParents[idx] = -1;
        key[idx] = INT_MAX;
        minHeap -> array[idx] = *newNode(idx, key[idx]);
        minHeap -> pos[idx] = (short) idx;
    }

    key[0] = 0; // we always start from 0 vertex
    shortestParents[0] = 0;
    minHeap -> array[0] = *newNode(0, key[0]);
    minHeap->pos[0] = 0;
    minHeap->heap_size = (short) numOfVertices;

    /// extracting vertex with minimal distance, updating distances to the adjacent vertex, putting these keys in heap

    while (!isQueueEmpty(minHeap)){
        int minVertex = extractMin(minHeap);
        for (int column = 0; column < numOfVertices; ++column){
            if (isInQueue(minHeap, column) && matrix[minVertex][column] < key[column]){
                key[column] = matrix[minVertex][column];

                shortestParents[column] = (short) minVertex;

                decreaseKey(minHeap, column, key[column]);
            }
        }
    }

    clearMatrix(matrix, numOfVertices);
    free(key);
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
    return shortestParents;
}


int** inputEdges(FILE*fp, const int numberOfVertices, const int numberOfEdges, workingResult* status){
    int** matrix = (int**)malloc(sizeof(int*) * numberOfVertices);
    initMatrix(matrix, numberOfVertices);
    for (int i = 0; i < numberOfEdges; ++i){
        int begin, end;
        long long int weight;
        int checkInput = fscanf(fp, "%d %d %lld", &begin, &end, &weight);

        if (checkInput != 3){
            *status = badNumberOfLines;
            break;
        }

        if (!isInBoundaries(begin, 0, MAX_NUM_VERTICES) || !isInBoundaries(end, 0, MAX_NUM_VERTICES)) {
            *status = badVertex;
            break;
        }

        if (!isInBoundaries(begin, 0,numberOfVertices) || !isInBoundaries(end, 0, numberOfVertices)){
            *status = badVertex;
            break;
        }

        if (!isInBoundaries(weight, 0, INT_MAX)){
            *status = badLength;
            break;
        }

        if (weight == INT_MAX)
            --weight;

        --begin;
        --end;
        matrix[begin][end] = matrix[end][begin] = (int) weight;
    }
    return matrix;
}


void printStatusResult(const workingResult status){
    switch (status){
        case badVertex:
            printf("bad vertex");
            break;
        case badLength:
            printf("bad length");
            break;
        case badNumOfVertices:
            printf("bad number of vertices");
            break;
        case badNumOfEdges:
            printf("bad number of edges");
            break;
        case badNumberOfLines:
            printf("bad number of lines");
            break;
        case noSpanningTree:
            printf("no spanning tree");
            break;
        default:
            break;
    }
}


int main() {
    int numVertices, numEdges;
    int** matrix;
    short* parents;
    FILE* fp;
    fp = fopen("in.txt", "r");
    workingResult status = success;

    fscanf(fp,"%d %d", &numVertices, &numEdges);
    if (!isInBoundaries(numVertices, 0, MAX_NUM_VERTICES))
        status = badNumOfVertices;

    if (!isInBoundaries(numEdges, 0, (numVertices * (numVertices - 1)/ 2)))
        status = badNumOfEdges;

    if (status == success){
        matrix = inputEdges(fp, numVertices, numEdges, &status);
    }

    if (status == success){
        parents = PrimSpanningTree(matrix, numVertices, numEdges, &status);
        checkParents(parents, numVertices, &status);
    }

    if (status == success)
        printPath(parents, numVertices);
    else
        printStatusResult(status);
    fclose(fp);
    return 0;
}