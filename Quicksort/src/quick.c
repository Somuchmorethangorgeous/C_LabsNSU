//
// Created by roxxa on 30.11.2019.
//

#include "quick.h"

void quicksort(long int* num, int low, int top) {
    long int pivot;
    long int swap;
    long int i = low, j = top;
    pivot = num[(low+top)/2]; // select the element in the middle
    while (i<=j) {
        for(i; num[i]<pivot; i++); // search for an element of a larger than pivot
        for(j; num[j]>pivot; j--); // search for an element of a smaller than pivot
        if(i <= j) {
            swap = num[i]; // swap elements
            num[i] = num[j];
            num[j] = swap;
            i++;
            j--;
        }
    }
    if (i < top) { //recursion to sort the right half of the array
        quicksort(num,i,top);
    }
    if (j > low) { //recursion to sort the left half of the array
        quicksort(num,low,j);
    }
}