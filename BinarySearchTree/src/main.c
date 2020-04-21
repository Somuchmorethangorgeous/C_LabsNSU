#include <stdio.h>
#include <malloc.h>
#include "balance.h"





int main(){
    TREE*T=NULL;
    int num;
    scanf("%d",&num);
    int* leaves;
    leaves = (int*)malloc(num * sizeof(int));
    for (long int i=0; i<num; ++i){
        scanf("%d", leaves+i);
        T = push_int_tree(T, *(leaves+i));
    }
    if (T) {
        in_order(T);
    }
    freemem(T);
    return 0;
}
