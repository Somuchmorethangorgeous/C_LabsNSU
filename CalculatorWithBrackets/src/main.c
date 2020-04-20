#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Calculate.h"

int main() {
    NUMBERS* N;
    OPERATIONS* S;
    N = (NUMBERS*)malloc(sizeof(NUMBERS));
    S = (OPERATIONS*)malloc(sizeof(OPERATIONS));
    S->next1 = NULL;
    N->next = NULL;
    char string[1001];
    gets(string);
    for (int i = 0; i < strlen(string); i++) {
        if (find_sign((string[i]))){ // check for character or number
            if (is_empty_s(S)){
                push_sign(S, new_sign(string[i]));
            }
            else {
                if (signs(string[i]) > signs(S->next1->sign))
                    push_sign(S, new_sign(string[i]));
                else if (string[i] == ')' || string[i] == '(') {
                    if (string[i] == '(')
                        push_sign(S, new_sign(string[i]));
                    else {
                        search_left_p(S,string[--i]);
                        i++;
                        while (S->next1->sign != '(') {
                            check_operands(N);
                            push_num(N, new_num(operation(N, S->next1->sign)));
                            pop_s(S);
                        }
                        pop_s(S); // remove left bracket
                    }
                }
                else {
                    if (!is_empty(N)) {
                        check_operands(N);
                        push_num(N, new_num(operation(N, S->next1->sign)));
                        pop_s(S);
                        push_sign(S, new_sign(string[i]));
                    }
                    else {
                        printf("syntax error");
                        exit(0);
                    }
                }
            }
        }
        else if (check_num(string[i])) {
            int num = string[i] - '0';
            while (i < strlen(string) && check_num(string[++i]))
                num = num * 10 + (string[i] - '0');
            i--;
            push_num(N, new_num(num));
        }
        else {
            printf("syntax error");
            exit(0);
        }
    }
    if (is_empty(N))
        printf("syntax error");
    else {
        while (!(is_empty_s(S))) {
            check_operands(N);
            push_num(N, new_num(operation(N, S->next1->sign)));
            pop_s(S);
        }
        printf("%d", N->next->value);
    }
    return 0;
}

