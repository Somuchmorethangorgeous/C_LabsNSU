#include <stdio.h>
#include <malloc.h>
#include <io.h>
#include <string.h>
#include <math.h>



#define SYMB 257
#define MAX_CODE_SIZE 24
#define MAX_BUF_SIZE (int)1e5
#define HEAP struct heap
#define NODE struct list
#define BUFFER struct class
#define TREE struct stack


NODE{
    int freq;
    unsigned char symb;
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
    unsigned char* buffer;
};



int parent(int i){
    return (i-1)/2;
}


int leftChild(int i){
    return (2*i+1);
}


int rightChild(int i){
    return (2*i+2);
}


int hasTreeBuilt(HEAP* ptr){
    return ptr -> heapSize == 1;
}


void swap(NODE* x, NODE* y){
    NODE tmp = *x;
    *x = *y;
    *y = tmp;
}


void siftUp(HEAP* ptr, int idx){
    while (idx && ptr->array[idx] -> freq < ptr->array[parent(idx)] -> freq) {
        swap(ptr->array[idx], ptr->array[parent(idx)]);
        idx = parent(idx);
    }
}


void minHeapify(HEAP* ptr, int idx){
    int l = leftChild(idx);
    int r = rightChild(idx);
    int smallest = idx;
    if (l < ptr -> heapSize && ptr -> array[l] -> freq < ptr -> array[smallest] -> freq)
        smallest = l;
    if (r < ptr -> heapSize && ptr -> array[r] -> freq < ptr -> array[smallest] -> freq)
        smallest = r;
    if (smallest != idx) {
        swap(ptr->array[smallest], ptr->array[idx]);
        minHeapify(ptr, smallest);
    }
}


NODE* extractMin(HEAP* ptr){
    NODE* root = ptr -> array[0];
    NODE* lastNode = ptr -> array[ptr -> heapSize - 1];
    ptr -> array[0] = lastNode;
    --ptr -> heapSize;
    minHeapify(ptr, 0);
    return root;
}


NODE* newNode(int frequency, unsigned char symb){
    NODE* tmp = (NODE*)malloc(sizeof(NODE));
    tmp -> freq = frequency;
    tmp -> symb = symb;
    tmp -> left = tmp -> right = NULL;
    return tmp;
}


void createHeap(HEAP**ptr, unsigned short cap){
    *ptr = (HEAP*)malloc(sizeof(HEAP));
    (*ptr) -> heapSize = 0;
    (*ptr) -> array = (NODE**)malloc(sizeof(NODE*) * cap);
}


NODE* popFromStack(TREE* ptr){
    TREE* delElem = ptr -> next;
    NODE* branch = delElem -> node;
    ptr -> next = ptr -> next -> next;
    free(delElem);
    return branch;
}


void pushInStack(TREE* ptr, NODE* node){
    TREE* first = ptr -> next;
    TREE* tmp = (TREE*)malloc(sizeof(TREE));
    tmp -> node = node;
    ptr -> next = tmp;
    tmp -> next = first;
}


void unionNodes(TREE* ptr){
    NODE* voidNode = newNode(0, '\0');
    NODE* rightBranch = popFromStack(ptr);
    NODE* leftBranch = popFromStack(ptr);
    voidNode -> left = leftBranch;
    voidNode -> right = rightBranch;
    pushInStack(ptr, voidNode);
}


void initStack(TREE** ptr){
    *ptr = (TREE*)malloc(sizeof(TREE));
    (*ptr) -> next = NULL;
    (*ptr) -> node = NULL;
}


unsigned char pushChar(unsigned char arr[]){
    unsigned res = 0;
    for (int i = 0; i < 8; ++i)
        res += (arr[i] - '0') * (int)pow(2, 7- i);
     return (unsigned char) res;
}


void readByte(unsigned char ptr [], unsigned char byte){
    for (int i = 0; i < 8; ++i){
        if ((1 << (7 - i)) & byte)
            ptr[i] = '1';
        else
            ptr [i] = '0';
    }
}


void writeBit(BUFFER*ptr, unsigned char bit, FILE* fout){
    unsigned res = 0;
    ptr->buffer[ptr -> bytes + ptr -> bits++] = bit;
    if (ptr->bits  == 8){
       for (int i = 0; i < 8; ++i) {
           res += (ptr->buffer[ptr->bytes + i] - '0') * (int)pow(2, 7 - i);
       }
       ptr -> buffer[ptr->bytes++] = (unsigned char) res;
       ptr -> bits = 0;
       if (ptr -> bytes == 65536) {
           fwrite(ptr->buffer, sizeof(unsigned char), ptr->bytes, fout);
           memset(ptr->buffer, '\0', 65536);
           ptr->bytes = 0;
       }
    }
}


void writeChar(BUFFER*ptr, unsigned char tmp, FILE* fout) {
    for(int i = 0; i < 8; ++i){
        if ((1 << (7 - i)) & tmp)
            writeBit(ptr, '1', fout);
        else
            writeBit(ptr, '0', fout);
    }
}


void decode(FILE* fin, FILE* fout, BUFFER* ptr, NODE* root, long int fsize, int offset) {
    unsigned int decodedSymb = 0;
    int numOfBytes = 0;
    int curPos = ftell(fin);
    int sizeOfMessage = fsize- curPos;
    NODE* curr = root;
    unsigned char usefulBits;
    unsigned char byte[8] ={0};
    unsigned char tmp;
    if (offset != 0 && sizeOfMessage != 1) {
        fseek(fin, -1, SEEK_CUR);
        fread(&tmp, sizeof(unsigned char), 1, fin);
        readByte(byte, tmp);
        for (int i = offset; i < 8; ++i) {
            if (byte[i] == '1' && curr->right) {
                curr = curr->right;
            } else if (byte[i] == '0' && curr->left) {
                curr = curr->left;
            }
            if (!curr->left && !curr->right) {
                writeChar(ptr, curr->symb, fout);
                ++decodedSymb;
                curr = root;
            }
        }
        offset = 0;
    }
    while (numOfBytes < sizeOfMessage - 2){
        fread(&tmp, sizeof(unsigned char), 1, fin);
        int usedBits = 0;
        readByte(byte, tmp);
        for(; usedBits < 8; ++usedBits){
            if (byte[usedBits] == '1' && curr -> right) {
                curr = curr->right;
            }
            else if (byte[usedBits] == '0' && curr -> left) {
                curr = curr->left;
            }
            if (!curr -> left && !curr -> right){
                writeChar(ptr, curr->symb, fout);
                curr = root;
            }
        }
        ++numOfBytes;
    }
    fseek(fin, -1L, SEEK_END);
    fread(&usefulBits, sizeof(unsigned char), 1, fin);
    fseek(fin, -2L, SEEK_CUR);
    fread(&tmp, sizeof(unsigned char), 1, fin);
    readByte(byte, tmp);
    for (int i = offset; i <= (usefulBits - '0') ; ++i) {
        if (byte[i] == '1' && curr->right) {
            curr = curr->right;
        } else if (byte[i] == '0' && curr->left) {
            curr = curr->left;
        }
        if (!curr->left && !curr->right) {
            writeChar(ptr, curr->symb, fout);
            curr = root;
        }
    }
}


NODE* recoverTree(FILE* fin, int* offset, unsigned char* byte){
    unsigned short numOfSymb;
    unsigned char tmp;
    int decodedSymb = 0;
    int sizeOfStack = 0;
    fseek(fin, 3L, SEEK_SET);
    TREE* ptr;
    initStack(&ptr);
    fread(&numOfSymb, sizeof(unsigned short), 1, fin);
    if (numOfSymb == 1){
        pushInStack(ptr, NULL);
        ++sizeOfStack;
    }
    while (decodedSymb < numOfSymb || sizeOfStack != 1){
        if (*offset == 0) {
            fread(&tmp, sizeof(unsigned char), 1, fin);
            readByte(byte, tmp);
        }
        if (byte[0] == '1'){
            tmp = tmp << 1;
            readByte(byte, tmp);
            fread(&tmp, sizeof(unsigned char), 1, fin);
            for (int i = 7 - *offset , j = 7; i < 8; ++i, --j) {
                if ((1 << j) & tmp)
                    byte[i] = '1';
                else
                    byte[i] = '0';
            }
            pushInStack(ptr, newNode(0,pushChar(byte)));
            sizeOfStack++;
            decodedSymb++;
            (*offset) = ((*offset) + 1) % 8;
            tmp = tmp << *offset;
            readByte(byte, tmp);
        }
        else {
            unionNodes(ptr);
            --sizeOfStack;
            tmp = tmp << 1;
            (*offset) = ((*offset) + 1) % 8;
            readByte(byte, tmp);
        }
    }
    return ptr -> next -> node;
}


void codeMessage(FILE*fin, FILE*fout, unsigned char** symbols, BUFFER* ptr) {
    fseek(fin, 3, SEEK_SET);
    unsigned char curSymb;
    while (fread(&curSymb, sizeof(unsigned char), 1, fin)) {
        unsigned int lenOfSymb = strlen(symbols[curSymb]);
        for (int i = 0; i < lenOfSymb; i++){
            writeBit(ptr, symbols[curSymb][i], fout);
        }
    }
    if (ptr -> bits % 8) {
        int bitsCount = ptr -> bits % 8;
        unsigned int usefulBits = bitsCount - 1;
        for (; bitsCount < 8 ; ++bitsCount)
            writeBit(ptr, '0', fout);
        writeChar(ptr, '0' + usefulBits, fout);
    } else
        writeChar(ptr, '7', fout);
}


void preOrder(FILE* fout, BUFFER*ptr, NODE* tmp, unsigned char** codeSymb, unsigned char* value, int* bit){
    if (!tmp -> left && !tmp -> right) {
        value[*bit] = '\0';
        writeBit(ptr, '1', fout);
        writeChar(ptr, tmp->symb, fout);
        codeSymb[tmp->symb] = (unsigned char*)malloc(sizeof(unsigned char) * (*(bit) + 1));
        for (int i = 0; i < *bit; ++i)
            codeSymb[tmp->symb][i] = value[i];
        codeSymb[tmp->symb][*bit] = '\0';
        return;
    }
    if (tmp -> left){
        value[(*bit)++] = '0';
        preOrder(fout, ptr, tmp -> left, codeSymb, value, bit);
        --(*bit);
    }
    if (tmp -> right) {
        value[(*bit)++] = '1';
        preOrder(fout, ptr, tmp -> right, codeSymb, value, bit);
        --(*bit);
    }
    writeBit(ptr, '0', fout);
}


HEAP* creatingBigTree(HEAP* ptr){
    if (hasTreeBuilt(ptr)){
        NODE* tmp = newNode(0, '\0');
        swap(ptr->array[0], tmp);
        ptr -> array[0] -> right = tmp;
    }
    else {
        while (!hasTreeBuilt(ptr)) {
            NODE *firstNode = extractMin(ptr);
            NODE *secondNode = extractMin(ptr);
            int newFrequency = firstNode->freq + secondNode->freq;
            NODE *tmp = newNode(newFrequency, '\0');
            tmp->left = firstNode;
            tmp->right = secondNode;
            ptr->array[ptr->heapSize] = tmp;
            siftUp(ptr, ptr->heapSize);
            ++ptr->heapSize;
        }
    }
    return ptr;
}


void pushingInTree(FILE*fin, FILE* fout, HEAP** ptr){
    int* frequency = (int*)calloc(SYMB, sizeof(int));
    unsigned short numOfSymb = 0;
    unsigned char readingChar;
    fseek(fin, 3, SEEK_SET);
    while (fread(&readingChar, sizeof(unsigned char), 1, fin)){
        if (!(frequency[readingChar]))
            ++numOfSymb;
        ++frequency[readingChar];
    }
    createHeap(ptr, numOfSymb);
    for (int i = 0; (*ptr) -> heapSize < numOfSymb; ++i){
        if (frequency[i]){
            (*ptr) -> array[(*ptr)->heapSize] = newNode(frequency[i], (unsigned char)i);
            siftUp(*ptr, (*ptr)->heapSize);
            ++(*ptr) -> heapSize;
        }
    }
    fwrite(&numOfSymb, sizeof(unsigned short), 1, fout);
    free(frequency);
}


long int sizeOfFile(FILE* fin){
    fseek(fin, 0L, SEEK_END);
    long int size = ftell(fin);
    fseek(fin, 0L, SEEK_SET);
    return size;
}


void initBuf(BUFFER** ptr) {
    *ptr = (BUFFER*) malloc(sizeof(BUFFER));
    (*ptr)->buffer = (unsigned char *) malloc(sizeof(unsigned char) * MAX_BUF_SIZE);
    (*ptr) -> bits = 0;
    (*ptr) -> bytes = 0;
}


int main() {
    FILE* fin = fopen("in.txt", "rb");
    FILE* fout = fopen("out.txt", "wb");
    HEAP* minHeap;
    BUFFER* outBuffer;
    unsigned char lineOfWork;
    long int fsize = sizeOfFile(fin);
    fread(&lineOfWork, sizeof(unsigned char), 1, fin);
    if (lineOfWork == 'c') {
        int bitInCode = 0;
        unsigned char* code = (unsigned char*)malloc(sizeof(unsigned char) * MAX_CODE_SIZE);
        unsigned char** codeSymb = (unsigned char**)malloc(sizeof(unsigned char*) * SYMB);
        if (fsize == 3)
            return 0;
        initBuf(&outBuffer);
        pushingInTree(fin, fout, &minHeap);
        creatingBigTree(minHeap);
        preOrder(fout, outBuffer, *minHeap->array, codeSymb, code, &bitInCode);
        codeMessage(fin, fout, codeSymb, outBuffer);
        fwrite(outBuffer->buffer, sizeof(unsigned char), outBuffer->bytes , fout);
        free(code);
        free(outBuffer->buffer);
       // free(codeSymb); - надо сделать полный
    }
    else if (lineOfWork == 'd'){
        unsigned char* byte = (unsigned char*)malloc(sizeof(unsigned char) * 8);
        int offset = 0;
        if (fsize == 3)
            return 0;
        initBuf(&outBuffer);
        NODE* root = recoverTree(fin, &offset, byte);
        byte[offset] = '\0';
        decode(fin, fout, outBuffer, root, fsize, offset);
        fwrite(outBuffer -> buffer, sizeof(unsigned char),  outBuffer->bytes, fout);
    }
    fclose(fin);
    return 0;
}

/// 12.05 32/36. не вывозит по времени (~в два раза больше)