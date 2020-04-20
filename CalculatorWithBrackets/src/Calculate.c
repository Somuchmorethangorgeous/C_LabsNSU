//
// Created by rus10 on 24.11.2019.
//

#include "Calculate.h"


int signs (char c) { //operation priority
    if (c == '+' || c == '-')
        return 1;
    if (c == '*'||c == '/')
        return 2;
    return 0;
}


int find_sign(char s) {
    if (s =='+'||s =='-'||s =='*'||s =='/' || s =='(' || s ==')')
        return 1;
    return 0;
}


void search_left_p(OPERATIONS* S, char s) { // checking for left bracket
    OPERATIONS* K;
    K = S->next1;
    while (K && K->sign !='(') {
        K = K->next1;
    }
    if (K == NULL||s =='(') {
        printf("syntax error");
        exit(0);
    }
}


void check_operands(NUMBERS* N) {
    if (N->next != NULL && N->next->next != NULL)
        return;
    printf("syntax error");
    exit(0);
}


int check_num(char num) {
    num = num-'0';
    if (num >= 0 && num <= 9)
        return 1;
    else
        return 0;
}


int is_empty_s(OPERATIONS* S) {
    return !(S->next1);
}


int is_empty(NUMBERS* N) {
    return !(N->next);
}


char pop_s(OPERATIONS* S) {
    char c = 0;
    OPERATIONS* del = (OPERATIONS*)malloc(sizeof(OPERATIONS));
    if (S->next1) {
        c = S->next1->sign;
        del = S->next1;
        S->next1 = S->next1->next1;
    }
    free(del);
    return c;
}


int pop (NUMBERS* N) {
    int value;
    NUMBERS* del = (NUMBERS*)malloc(sizeof(NUMBERS));
    if (N->next) {
        value = N->next->value;
        del = N->next;
        N->next = N->next->next;
    }
    free(del);
    return value;
}


void push_sign (OPERATIONS* S, OPERATIONS* ptr_s) {
    OPERATIONS*first = S->next1;
    ptr_s->next1 = first;
    S->next1 = ptr_s;
}


void push_num (NUMBERS* N, NUMBERS* ptr) {
    NUMBERS* first = N->next;
    ptr->next = first;
    N->next = ptr;
}


OPERATIONS* new_sign(char s) {
    OPERATIONS* tmp_s = (OPERATIONS*)malloc(sizeof(OPERATIONS));
    tmp_s->sign = s;
    return tmp_s;
}


NUMBERS* new_num(int value) {
    NUMBERS* tmp = (NUMBERS*)malloc(sizeof(NUMBERS));
    tmp->value = value;
    return tmp;
}


int operation(NUMBERS* N, char c) {
    switch (c){
        case '+': return pop(N->next) + pop(N);
        case '-': return pop(N->next) - pop(N);
        case '*': return pop(N->next) * pop(N);
        case '/':
            if (N->next->value == 0){
                printf("division by zero");
                exit(0);
            }
            return pop(N->next) / pop(N);
        default:
            printf("syntax error");
            exit(0);
    }
}