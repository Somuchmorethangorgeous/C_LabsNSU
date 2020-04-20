#include <stdio.h>
#include <malloc.h>
#include "quick.h"


int main() {
    int n;
    scanf("%d",&n);
    long int* a;
    a=(long int*)malloc(n* sizeof(long int));
    for (int i=0; i<n; i++) {
        scanf("%ld", a+i);
    }
    quicksort(a,0,n-1);
    for (int j=0; j<n; j++) {
        printf("%ld ", *(a+j));
    }
    free(a);
    return 0;
}