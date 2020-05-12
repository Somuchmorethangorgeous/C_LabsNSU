//
// Created by roxxa on 12.05.2020.
//

#ifndef HUFFMAN_LIB_H
#define HUFFMAN_LIB_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>

#define CODING 'c'
#define DECODING 'd'
#define uc unsigned char
#define BYTE_SIZE 8
#define CODES_ASCII 256
#define MAX_CODE_SIZE 24
#define MAX_BUF_SIZE (int)1e5
#define HEAP struct heap
#define NODE struct list
#define BUFFER struct buffer
#define TREE struct stack



NODE{
    int freq;
    uc symbol;
    NODE* left;
    NODE* right;
};


HEAP{
    int heapSize;
    NODE** array;
};


TREE{
    NODE* node;
    TREE* next;
};


BUFFER{
    int bits;
    int bytes;
    uc* buffer;
};


/// Heap

int parent(int i);
int leftChild(int i);
int rightChild(int i);
int hasTreeBuilt(HEAP* ptr);
void swap(NODE* x, NODE* y);
void siftUp(HEAP* ptr, int idx);
void minHeapify(HEAP* ptr, int idx);
NODE* extractMin(HEAP* ptr);
NODE* newNode(int frequency, uc symbol);
void createHeap(HEAP**ptr, unsigned short cap);


/// recover tree

NODE* popFromStack(TREE* ptr);
void pushInStack(TREE* ptr, NODE* node);
void unionNodes(TREE* ptr);
void initStack(TREE** ptr);

/// byte processing

uc pushChar(const uc arr[]);
void readByte(uc ptr [], uc byte);
void writeBit(BUFFER* ptr, uc bit, FILE* fout);
void writeChar(BUFFER* ptr, uc tmp, FILE* fout);

/// unzip

NODE* recoverTree(FILE* fin, unsigned int* offset, uc* byte);
void decode(FILE* fin, FILE* fout, NODE* root, long int fSize, unsigned int offset);

/// zip

void initBuf(BUFFER** ptr);
void pushingInTree(FILE* fin, FILE* fout, HEAP** ptr);
HEAP* creatingBigTree(HEAP* ptr);
void preOrder(FILE* fout, BUFFER* ptr, NODE* tmp, uc** codesOfSymbols, uc* value, int* bit);
void codeMessage(FILE* fin, FILE* fout, uc** codesOfSymbols, BUFFER* ptr);


long int sizeOfFile(FILE* fin);
void freeMem(uc** codesOfSymbols, uc* code, BUFFER* ptr);



#endif //HUFFMAN_LIB_H
