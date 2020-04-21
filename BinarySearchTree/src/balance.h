//
// Created by roxxa on 23.12.2019.
//

#ifndef INC_1_BALANCE_H
#define INC_1_BALANCE_H
    #include <stdio.h>
    #include <malloc.h>
    #define TREE struct tree


    TREE {
        int lvl;
        int node;
        TREE* left;
        TREE* right;
    };


int height(TREE*T);
int fixheight (TREE*T);
TREE* rotR(TREE*T);
TREE* rotL(TREE*T);
int difference(TREE*T);
TREE* rebalance(TREE*T);
void freemem(TREE*T);
void in_order(TREE* T);
TREE* push_int_tree (TREE*T, int value);


#endif //INC_1_BALANCE_H
