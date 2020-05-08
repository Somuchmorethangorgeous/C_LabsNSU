#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>


#define SYMB 256
#define MAX_BUF_SIZE 1e5
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
    int sizeOfBuffer;
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


HEAP* createHeap(unsigned int cap){
    HEAP*ptr = (HEAP*)malloc(sizeof(HEAP));
    ptr -> heapSize = 0;
    ptr -> array = (NODE**)malloc(sizeof(NODE*) * cap);
    return ptr;
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


unsigned char inChar(const unsigned char byte [], unsigned char tmp) {
    unsigned int res = 0;
    for(int i = 0; i < 8; ++i)
        res += (byte[i] - '0') * (int)pow(2, 7 - i);
    tmp = (unsigned char) res;
    return tmp;
}


void readByte(unsigned char ptr [], unsigned char byte){
    for (int i = 0; i < 8; ++i){
        if ((1 << (7 - i)) & byte)
            ptr[i] = '1';
        else
            ptr [i] = '0';
    }
}


void writeByte(BUFFER*ptr, unsigned char byte){
    ptr->buffer[ptr->sizeOfBuffer++] = byte;
}


void decode(FILE* fin, BUFFER* ptr, NODE* root) {
    int numOfBytes = 0;
    int curPos = ftell(fin);
    fseek(fin, 0, SEEK_END);
    int endOfFile = ftell(fin);
    int sizeOfMessage = endOfFile - curPos;
    fseek(fin, curPos, SEEK_SET);
    NODE* curr = root;
    unsigned char usefulBits;
    unsigned char byte[8] ={0};
    unsigned char tmp;
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
                writeByte(ptr, curr->symb);
                curr = root;
            }
        }
        ++numOfBytes;
    }
    fseek(fin, -1, SEEK_END);
    fread(&usefulBits, sizeof(unsigned char), 1, fin);
    fseek(fin, -2, SEEK_CUR);
    fread(&tmp, sizeof(unsigned char), 1, fin);
    readByte(byte, tmp);
    for (int i = 0; i <= (usefulBits - '0'); ++i) {
        if (byte[i] == '1' && curr->right) {
            curr = curr->right;
        } else if (byte[i] == '0' && curr->left) {
            curr = curr->left;
        }
        if (!curr->left && !curr->right) {
            writeByte(ptr, curr->symb);
            curr = root;
        }
    }
}


NODE* recoverTree(FILE* fin){
    fseek(fin, 3, SEEK_SET);
    unsigned char numOfSymb, binSymb, symb;
    int decodedSymb = 0;
    int sizeOfStack = 0;
    TREE* ptr;
    initStack(&ptr);
    fread(&numOfSymb, sizeof(unsigned char), 1, fin);
    if (numOfSymb == 1){
        pushInStack(ptr, newNode(0, '\0'));
        ++sizeOfStack;
    }
    for(int i = 0; decodedSymb < numOfSymb || sizeOfStack != 1; ++i){
        fread(&binSymb, sizeof(unsigned char), 1, fin);
        if (binSymb == '1'){
            fread(&symb, sizeof(unsigned char), 1, fin);
            pushInStack(ptr, newNode(0, symb));
            ++decodedSymb;
            ++sizeOfStack;
        }
        else if (binSymb == '0') {
            unionNodes(ptr);
            --sizeOfStack;
        }
    }
    return ptr -> next -> node;
}


void codeMessage(FILE*fin, unsigned char** symbols, BUFFER* ptr) {
    unsigned char curSymb;
    unsigned char tmp;
    unsigned int bitsCount = 0;
    unsigned char byte[8] = {0};
    fseek(fin, 3, SEEK_SET);
    while (fread(&curSymb, sizeof(unsigned char), 1, fin)) {
        unsigned int lenOfSymb = strlen(symbols[curSymb]);
        int j = 0;
        while (j < lenOfSymb) {
            for (; bitsCount < 8 && j < lenOfSymb; ++bitsCount) {
                byte[bitsCount] = symbols[curSymb][j++];
            }
            if (bitsCount == 8){
                writeByte(ptr, inChar(byte, tmp));
                bitsCount = 0;
            }
        }
    }
    if (bitsCount) {
        unsigned int usefulBits = bitsCount - 1;
        for (; bitsCount < 8; ++bitsCount)
            byte[bitsCount] = '0';
        writeByte(ptr, inChar(byte, tmp));
        writeByte(ptr, '0' + usefulBits);
    } else
        writeByte(ptr, '7');
}


void preOrder(BUFFER*ptr, NODE* tmp, unsigned char** codeSymb, char* value, int* bit){
    if (!tmp -> left && !tmp -> right) {
        value[*bit] = '\0';
        writeByte(ptr, '1');
        writeByte(ptr, tmp -> symb);
        codeSymb[tmp->symb] = (unsigned char*)malloc(sizeof(unsigned char) * *(bit));
       // printf("%c %s\n", tmp-> symb, value);
        strcpy(codeSymb[tmp->symb], value);
        return;
    }
    if (tmp -> left){
        value[(*bit)++] = '0';
        preOrder(ptr, tmp -> left, codeSymb, value, bit);
        --(*bit);
    }
    if (tmp -> right) {
        value[(*bit)++] = '1';
        preOrder(ptr, tmp -> right, codeSymb, value, bit);
        --(*bit);
    }
    writeByte(ptr, '0');
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


void pushingInTree(FILE*fin, BUFFER* tmp, HEAP** ptr){
    int* frequency = (int*)calloc(SYMB, sizeof(int));
    unsigned int numOfSymb = 0;
    unsigned char readingChar;
    fseek(fin, 3, SEEK_SET);
    while (fread(&readingChar, sizeof(unsigned char), 1, fin)){
        if (!(frequency[readingChar]))
            ++numOfSymb;
        ++frequency[readingChar];
    }
    *ptr = createHeap(numOfSymb);
    for (int i = 0; (*ptr) -> heapSize < numOfSymb; ++i){
        if (frequency[i]){
            (*ptr) -> array[(*ptr)->heapSize] = newNode(frequency[i], (unsigned char)i);
            siftUp(*ptr, (*ptr)->heapSize);
            ++(*ptr) -> heapSize;
        }
    }
    writeByte(tmp, numOfSymb);
    free(frequency);
}


int checkForSymb(FILE* fin){
    fseek(fin, 0, SEEK_END);
    int size = ftell(fin);
    return size;
}


BUFFER* initBuf(BUFFER* ptr){
    ptr = (BUFFER*)malloc(sizeof(BUFFER));
    ptr -> buffer = (unsigned char*)malloc(sizeof(unsigned char) * MAX_BUF_SIZE);
    ptr -> sizeOfBuffer = 0;
    return ptr;
}


int main() {
    FILE* fin = fopen("in.txt", "rb");
    FILE* fout = fopen("out.txt", "wb");
    HEAP* minHeap;
    BUFFER* outBuffer;
    unsigned char lineOfWork;
    fread(&lineOfWork, sizeof(unsigned char), 1, fin);
    if (lineOfWork == 'c') {
        int bitInCode = 0;
        unsigned char** codeSymb = (unsigned char**)malloc(sizeof(unsigned char*) * SYMB);
        char* code = (char*)malloc(sizeof(unsigned char) * SYMB);
        if (checkForSymb(fin) == 3)
            return 0;
        outBuffer = initBuf(outBuffer);
        pushingInTree(fin, outBuffer, &minHeap);
        creatingBigTree(minHeap);
        preOrder(outBuffer, *minHeap->array, codeSymb, code, &bitInCode);
        codeMessage(fin, codeSymb, outBuffer);
        fwrite(outBuffer->buffer, sizeof(unsigned char), outBuffer->sizeOfBuffer, fout);
        free(code);
        free(outBuffer->buffer);
       // free(codeSymb); - надо сделать полный
    }
    else if (lineOfWork == 'd'){
        if (checkForSymb(fin) == 3)
            return 0;
        outBuffer = initBuf(outBuffer);
        NODE* root = recoverTree(fin);
        decode(fin, outBuffer, root);
        fwrite(outBuffer -> buffer, sizeof(unsigned char), outBuffer -> sizeOfBuffer, fout);
    }
    fclose(fin);
    return 0;
}

/// проблема с большим текстом: символы на 10 бит. неправильный код в куче.
/// воссоздать дерево с помощью стека, (перевести в бинарный вид заокдированный файл?), раскодировать сообщение