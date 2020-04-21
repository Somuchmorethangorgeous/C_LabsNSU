#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#define NODE struct list1
#define HEAP struct list2
#define MAX_NUM_VERTICES 5000


typedef  enum CheckData{
    success,
    badVertex,
    badLength,
    badNumberOfVertices,
    badNumberOfEdges,
    badNumberOfLines,
    noPath,
    overflow
} workingResult;


NODE{
    int vertex;
    unsigned int key;
};


HEAP{
    int heapSize;
    int* pos;
    NODE* array;
};





int parent(int i){
    return (i-1)/2;
}


int leftChild(int i){
    return 2*i+1;
}


int rightChild(int i){
    return 2*i+2;
}


int isInQueue(HEAP* ptr, int vertex){
    return ptr -> pos[vertex] < ptr -> heapSize;
}


int isQueueEmpty(HEAP* ptr){
    return ptr->heapSize == 0;
}


void swap(NODE* x, NODE* y){
    NODE tmp = *x;
    *x = *y;
    *y = tmp;
}


void siftUp(HEAP* ptr, int idx){
    while (idx && ptr->array[idx].key < ptr->array[parent(idx)].key) {
        int parentVrtx = ptr->array[parent(idx)].vertex;
        int childVrtx = ptr->array[idx].vertex;

        ptr->pos[parentVrtx] = idx;
        ptr->pos[childVrtx] = parent(idx);

        swap(&ptr->array[idx], &ptr->array[parent(idx)]);
        idx = parent(idx);
    }
}


void minHeapify(HEAP* ptr, int idx){
    int l = leftChild(idx);
    int r = rightChild(idx);
    int smallest = idx;
    if (l < ptr -> heapSize && ptr -> array[l].key < ptr -> array[smallest].key)
        smallest = l;
    if (r < ptr -> heapSize && ptr -> array[r].key < ptr -> array[smallest].key)
        smallest = r;
    if(smallest != idx) {

        NODE smallestNode = ptr->array[smallest];
        NODE idxNode = ptr -> array[idx];

        ptr -> pos[smallestNode.vertex] = idx;
        ptr -> pos[idxNode.vertex] = smallest;

        swap(&ptr -> array[smallest], &ptr -> array[idx]);
        minHeapify(ptr, smallest);
    }
}


int extractMin(HEAP* ptr){
   if (isQueueEmpty(ptr))
       return -1;
   if (ptr -> heapSize == 1){
       --ptr->heapSize;
       return ptr->array[0].vertex;
   }

   NODE root = ptr -> array[0];
   int minVertex = root.vertex;
   NODE lastNode = ptr -> array[ptr -> heapSize - 1];

   ptr -> array[0] = lastNode;
   ptr -> pos[lastNode.vertex] = 0;
   ptr -> pos[root.vertex] = ptr -> heapSize - 1;
   --ptr -> heapSize;
   minHeapify(ptr, 0);

   return minVertex;
}


void decreaseKey(HEAP* ptr, int vertex, unsigned int key){
    int pos = ptr -> pos[vertex];
    ptr->array[pos].key = key;
    siftUp(ptr, pos);
}


NODE* newNode(int vertex, unsigned int key){
    NODE* tmp = (NODE*)malloc(sizeof(NODE));
    tmp -> vertex = vertex;
    tmp -> key = key;
    return tmp;
}


HEAP* createHeap(int cap){
    HEAP*ptr = (HEAP*)malloc(sizeof(HEAP));
    ptr -> heapSize = 0;
    ptr -> pos = (int*)malloc(sizeof(int) * cap);
    ptr -> array = (NODE*)malloc(sizeof(NODE) * cap);
    return ptr;
}


int checkConditions(int inf, unsigned int value, int sup){
    return (value >= inf && value <= sup);
}


int counter(int** matrix, int vertex, const int start, int* numOfPath, bool* visited, const int numOfVertices) {
    if (vertex == start)
        return numOfPath[vertex];
    else{
        int sum = 0;
        visited[vertex] = true;
        for(int i = 0; i < numOfVertices; ++i){
            if (matrix[vertex][i] && (!visited[i] || i == start))
                sum += counter(matrix, i, start, numOfPath, visited, numOfVertices);
            numOfPath[vertex] = sum;
        }
        return sum;
    }
}


int countPath(int** matrix, const int numOfVertices, int start, int end){
    int value;
    int* numOfPath = (int*)malloc(sizeof(int) * numOfVertices);
    bool* visited = (bool*)malloc(numOfVertices);
    memset(numOfPath, 0, sizeof(int) * numOfVertices);
    memset(visited, false, numOfVertices);
    numOfPath[start] = 1;
    counter(matrix, end, start, numOfPath, visited, numOfVertices);
    value = numOfPath[end];
    free(numOfPath);
    free(visited);
    return value;
}


void printPath(int* parents, int start, int end){
    printf("%d ", end + 1);
    for(int i = end; i != start; i=parents[i])
        printf("%d ", parents[i] + 1);
    free(parents);
}


void printDistance(unsigned int* key, int numOfVertices){
    for (int i = 0; i < numOfVertices; ++i) {
        if (checkConditions(0, key[i], INT_MAX))
            printf("%d ", key[i]);
        else if (key[i] == UINT_MAX)
            printf("oo ");
        else
            printf("INT_MAX+ ");
    }
    printf("\n");
}


void checkPath(const int* parents, const int end,  workingResult* status){
        if(parents[end] == -1)
            *status = noPath;
}


void clearMatrix(int** matrix, const int numOfVertices){
    for(int i = 0; i < numOfVertices; ++i)
        free(matrix[i]);
    free(matrix);
}


int* Dijkstra(int** adjMatrix, const int numOfVertices,const int start, const int end, const int numOfWays, workingResult* status){
    unsigned int* key = (unsigned int*)malloc(sizeof(unsigned int) * numOfVertices); // array of distances to each vertex from start
    int* parents = (int*)malloc(sizeof(int) * numOfVertices);

    HEAP* minHeap = createHeap(numOfVertices); // initial minimal heap
    for(int i = 0; i < numOfVertices; ++i){
        parents[i] = -1;
        key[i] = UINT_MAX;
        minHeap -> array[i] = *newNode(i, key[i]);
        minHeap -> pos [i] = i;
    }

    key[start] = 0; // starting vertex has 0 distance, so we'll extract it first
    parents[start] = start;
    minHeap -> array[0] = *newNode(start, key[start]);
    decreaseKey(minHeap, start, key[start]);
    minHeap -> heapSize = numOfVertices;

    /// extracting nearest vertex, updating distance to the adjacent vertices, putting this in heap

    while(!isQueueEmpty(minHeap)){
        int nearestVertex = extractMin(minHeap);
        for(int adjVertex = 0; adjVertex < numOfVertices; ++adjVertex){
            if(adjMatrix[nearestVertex][adjVertex] && isInQueue(minHeap, adjVertex) && key[nearestVertex] + adjMatrix[nearestVertex][adjVertex] < key[adjVertex]){

                key[adjVertex] =  key[nearestVertex] + adjMatrix[nearestVertex][adjVertex];

                parents[adjVertex] = nearestVertex;

                decreaseKey(minHeap, adjVertex, key[adjVertex]);
            }
        }
    }

    clearMatrix(adjMatrix, numOfVertices);
    printDistance(key, numOfVertices);

    if (key[end] > INT_MAX && numOfWays > 1)
        *status = overflow;

    free(key);
    free(minHeap -> array);
    free(minHeap -> pos);
    free(minHeap);
    return parents;
}


int** initMatrix(FILE*fp, const int numOfVertices, const int numOfEdges, workingResult* status){
    int** adjMatrix = (int**)malloc(sizeof(int) * numOfVertices);
    for (int i = 0; i < numOfVertices; ++i){
        adjMatrix[i] = (int*)malloc(sizeof(int) * numOfVertices);
        memset(adjMatrix[i], 0, sizeof(int) * numOfVertices);
    }
    for(int j = 0; j < numOfEdges; ++j){
        int start, end, checkInput;
        unsigned int length;
        checkInput = fscanf(fp, "%d %d %d", &start, &end, &length);
        if (checkInput != 3){
            *status = badNumberOfLines;
            break;
        }

        if(!checkConditions(1, start, MAX_NUM_VERTICES) || !checkConditions(1, start, numOfVertices)){
            *status = badVertex;
            break;
        }

        if(!checkConditions(1, end, MAX_NUM_VERTICES) || !checkConditions(1, end, numOfVertices)){
            *status = badVertex;
            break;
        }

        if(!checkConditions(0, length, INT_MAX)){
            *status = badLength;
            break;
        }

        --start;
        --end;
        adjMatrix[start][end] = adjMatrix[end][start] = length;
    }
    return adjMatrix;
}


void printResult(workingResult status){
    switch (status){
        case badVertex:
            printf("bad vertex");
            break;
        case badLength:
            printf("bad length");
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
        case noPath:
            printf("no path");
            break;
        case overflow:
            printf("overflow");
            break;
        default:
            break;
    }
}


int main() {
    FILE* fp = fopen("in.txt", "r");
    workingResult status = success;
    int** adjMatrix;
    int* parents;
    int numOfVertices, numOfEdges, start, end;
    int numOfWays = 0;
    int checkInput = fscanf(fp, "%d %d %d %d", &numOfVertices, &start, &end, &numOfEdges);
    if (checkInput != 4) {
        status = badNumberOfLines;
        printResult(status);
    }

    int maxNumOfEdges = numOfVertices*(numOfVertices - 1)/2;
    if(!checkConditions(0, numOfVertices, MAX_NUM_VERTICES))
        status = badNumberOfVertices;

    if(!checkConditions(0, numOfEdges, maxNumOfEdges))
        status = badNumberOfEdges;

    if(!checkConditions(1, start, MAX_NUM_VERTICES) || !checkConditions(1, end, MAX_NUM_VERTICES))
        status = badVertex;

    if (status == success){
        --end;
        --start;
        adjMatrix = initMatrix(fp, numOfVertices, numOfEdges, &status); // creating matrix of adjacency
    }

    if (status == success){
        numOfWays = countPath(adjMatrix, numOfVertices, start, end); // counting ways to the ending vertex
        parents = Dijkstra(adjMatrix, numOfVertices, start, end, numOfWays, &status); // shortest ways to the each vertex
        checkPath(parents, end, &status);
    }

    if(status == success){
        printPath(parents, start, end);
    }
    else
        printResult(status);
    fclose(fp);
    return 0;
}
