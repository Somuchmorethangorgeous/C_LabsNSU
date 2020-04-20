//
// Created by rus10 on 24.11.2019.
//

#ifndef SS_COMPUTATION_H
#define SS_COMPUTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#define OPERATIONS struct list1
#define NUMBERS struct list2

NUMBERS {
    int value;
    NUMBERS* next;
};

OPERATIONS {
    char sign;
    OPERATIONS* next1;
};


int signs (char c);
int find_sign(char s);
void search_left_p(OPERATIONS*S, char s);
void check_operands(NUMBERS*N);
int check_num(char num);
int is_empty_s(OPERATIONS*S);
int is_empty(NUMBERS*N);
char pop_s(OPERATIONS*S);
int pop (NUMBERS*N);
void push_sign (OPERATIONS*S, OPERATIONS*ptr_s);
void push_num (NUMBERS*N, NUMBERS*ptr);
OPERATIONS* new_sign(char s);
NUMBERS* new_num(int value);
int operation(NUMBERS*N, char c);


#endif //SS_COMPUTATION_H
