#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#define R 5000
#define GRAPH struct list


GRAPH{
    int finish;
    long long int weight;
    GRAPH *next;
};


void PrintPath(const GRAPH* list,  const int* ways, int destination){
    if (!*(ways + destination))
        printf("no path");
    else if ((list + destination) -> weight > INT_MAX && *(ways + destination) != 1)
        printf("overflow");
    else {
        printf("%d ", destination);
        while ((list + destination) -> weight){
            printf("%d ", (list + destination) -> finish);
            destination = (list + destination) -> finish;
        }
    }
}


void printDistance(const GRAPH* list, int numVertexes){
    for (int i = 1; i <= numVertexes; ++i){
        if ((list + i) -> weight > INT_MAX && (list + i) -> finish == -1)
            printf("oo ");
        else if ((list + i) -> weight > INT_MAX)
            printf("INT_MAX+ ");
        else
            printf("%lld ", (list + i) -> weight);
    }
    printf("\n");
}


int popElem(GRAPH*top, int* neighbourWeight){
    int x;
    GRAPH* delElem = top -> next;
    x = top -> next -> finish;
    if (neighbourWeight)
        *neighbourWeight = top -> next -> weight;
    top -> next = top -> next -> next;
    free(delElem);
    return x;
}


int findMin(const GRAPH* list, const int* used, int numVertexes){
    long long int min = LONG_LONG_MAX;
    int index = 0;
    for (int i = 1; i <= numVertexes; ++i){
        if (!*(used + i)  && (list + i)-> weight < min){
            min = (list + i) -> weight;
            index = i;
        }
    }
    return index;
}


void dejkstra(GRAPH* list, int departure, int destination, int numVertexes){
    int* ways = (int*)malloc(sizeof(int) * (numVertexes+1));
    int* used = (int*)malloc(sizeof(int) * (numVertexes+1));
    for (int i = 1; i <= numVertexes; ++i){
        (list + i) -> weight = LONG_LONG_MAX;
        (list + i) -> finish = -1;
        *(ways + i) = 0;
        *(used + i) = 0;
    }
    (list + departure) -> weight = 0;
    *(ways + departure) = 1;
    while (1){
        int curVertex = findMin(list, used, numVertexes);
        if (!curVertex)
            break;
        *(used+curVertex) = 1;
        while ((list + curVertex) -> next){
            int neighbourWeight;
            int neighbour = popElem(list + curVertex, &neighbourWeight);
            if (!*(used + neighbour) && (list + neighbour) -> weight > (list + curVertex) -> weight + neighbourWeight){
                (list + neighbour) -> weight = (list + curVertex) -> weight + neighbourWeight;
                (list + neighbour)->finish = curVertex;
            }
            *(ways+neighbour) += 1;
        }
    }
    printDistance(list, numVertexes);
    PrintPath(list, ways, destination);
    free(used);
    free(ways);
}


void ClearStack(GRAPH *top){
    while (top -> next)
        popElem(top, NULL);
}


void pushElementQueue(GRAPH* top, int finish, int weight){
    GRAPH* edge = (GRAPH*)malloc(sizeof(GRAPH));
    edge -> finish = finish;
    edge -> weight = weight;
    edge -> next = top -> next;
    top -> next = edge;
}


void checkGraph(int vert, int edg){
    if (vert > R || vert < 0){
        printf("bad number of vertices");
        exit (0);
    }
    else if (edg > (vert * (vert - 1)/2) || edg < 0){
        printf("bad number of edges");
        exit(0);
    }
}


void checkInput(int start, int end, int len){
    if (start + end > 2*R || end + start < 2){
        printf("bad vertex");
        exit(0);
    }
    else if (len < 0){
        printf("bad length");
        exit(0);
    }
}


int main() {
    int vert, edg, start, end;
    FILE *fp;
    fp = fopen("in.txt","r");
    fscanf(fp,"%d",&vert);
    fscanf(fp,"%d",&start);
    fscanf(fp, "%d", &end);
    fscanf(fp, "%d", &edg);
    GRAPH* vertexes = (GRAPH*)malloc(sizeof(GRAPH)*(vert+1));
    for (int i = 0; i <= vert; ++i)
        (vertexes + i) -> next = NULL;
    checkGraph(vert, edg);
    checkInput(start, end, 0);
    for(int i = 0; i < edg; ++i){

        int firstPoint, secondPoint, weight;
        int checkEdge = fscanf(fp, "%d %d %d", &firstPoint, &secondPoint, &weight);

        if (checkEdge != 3){
            printf("bad number of lines");
            return 0;
        }

        checkInput(firstPoint, secondPoint, weight);
        pushElementQueue(vertexes + firstPoint, secondPoint, weight);
        pushElementQueue(vertexes + secondPoint, firstPoint, weight);
    }
    dejkstra(vertexes, start, end, vert);
    for (int i = 0; i <= vert; ++i)
        ClearStack(vertexes+i);
    free(vertexes);
    fclose(fp);
    return 0;
}
