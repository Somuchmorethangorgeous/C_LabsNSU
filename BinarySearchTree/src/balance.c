//
// Created by roxxa on 23.12.2019.
//

#include "balance.h"

int height(TREE*T){ // subtree height
    if (T){
        if (T->lvl)
            return T->lvl;
    }
    return 0;
}


int fixheight(TREE*T){ // change of heights
    int h_left=height(T->left);
    int h_right=height(T->right);
    if (h_left>h_right)
        T->lvl=h_left+1;
    else
        T->lvl=h_right+1;
    return 0;
}


TREE* rotR(TREE*T){
    TREE*q=T->left;
    T->left=q->right;
    q->right=T;
    fixheight(T);
    fixheight(q);
    return q;
}


TREE* rotL(TREE*T){
    TREE*q=T->right;
    T->right=q->left;
    q->left=T;
    fixheight(T);
    fixheight(q);
    return q;
}


int difference(TREE*T){ //checking for rotate
    return height(T->right)-height(T->left);
}


TREE* rebalance(TREE*T){
    fixheight(T);
    if (difference(T)>1){
        if (difference(T->right)<0)
            T->right=rotR(T->right);
        return rotL(T);
    }
    if (difference(T)<-1){
        if (difference(T->left)>0)
            T->left=rotL(T->left);
        return rotR(T);
    }
    return T;
}


void freemem(TREE*T){
    if (T){
        freemem(T->left);
        freemem(T->right);
        free(T);
    }
}


void in_order(TREE*T){
    if (T->left)
        in_order(T->left);
    printf("%d ", T->node);
    if (T->right)
        in_order(T->right);
}


TREE* push_int_tree (TREE*T, int value){
    if (!T){
        T = (TREE*)malloc(sizeof(TREE));
        T->node = value;
        T->left = T->right=NULL;
    }
    else if (T->node > value)
        T->left = push_int_tree(T->left,value);
    else
        T->right = push_int_tree(T->right,value);
    return rebalance(T);
}

