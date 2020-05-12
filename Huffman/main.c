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


NODE* newNode(int frequency, uc symbol){
    NODE* tmp = (NODE*)malloc(sizeof(NODE));
    tmp -> freq = frequency;
    tmp -> symbol = symbol;
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


uc pushChar(const uc arr[]){
    unsigned res = 0;
    for (int i = 0; i < BYTE_SIZE; ++i)
        res += (arr[i] - '0') * (int)pow(2, 7- i);
     return (uc)res;
}


void readByte(uc ptr [], uc byte){
    for (unsigned int i = 0; i < BYTE_SIZE; ++i){
        if ((1u << (7 - i)) & byte)
            ptr[i] = '1';
        else
            ptr [i] = '0';
    }
}


void writeBit(BUFFER*ptr, uc bit, FILE* fout){
    unsigned res = 0;
    ptr->buffer[ptr -> bytes + ptr -> bits++] = bit;
    if (ptr->bits  == BYTE_SIZE){
       for (int i = 0; i < BYTE_SIZE; ++i) {
           res += (ptr->buffer[ptr->bytes + i] - '0') * (int)pow(2, 7 - i);
       }
       ptr -> buffer[ptr->bytes++] = (uc) res;
       ptr -> bits = 0;
       if (ptr -> bytes == 65536) { // buffer cleaning
           fwrite(ptr->buffer, sizeof(uc), ptr->bytes, fout);
           memset(ptr->buffer, '\0', 65536);
           ptr->bytes = 0;
       }
    }
}


void writeChar(BUFFER*ptr, uc tmp, FILE* fout) {
    for(unsigned int i = 0; i < BYTE_SIZE; ++i){
        if ((1u << (7 - i)) & tmp)
            writeBit(ptr, '1', fout);
        else
            writeBit(ptr, '0', fout);
    }
}


void decode(FILE* fin, FILE* fout, NODE* root, long int fSize, unsigned int offset) {
    int numOfBytes = 0;
    int curPos = ftell(fin);
    int sizeOfMessage = fSize- curPos;
    NODE* curr = root;
    uc usefulBits;
    uc byte[BYTE_SIZE] ={0};
    uc tmp;

    if (offset != 0 && sizeOfMessage != 1){ // decoding first byte
        fseek(fin, -1L, SEEK_CUR);
        fread(&tmp, sizeof(uc), 1, fin);
        readByte(byte, tmp);
        for (unsigned int i = offset; i < BYTE_SIZE; ++i){
            if (byte[i] == '1' && curr->right){
                curr = curr->right;
            }
            else if (byte[i] == '0' && curr->left){
                curr = curr->left;
            }
            if (!curr->left && !curr->right){
                fwrite(&curr->symbol, sizeof(uc), 1, fout);
                curr = root;
            }
        }
        offset = 0;
    }

    while (numOfBytes < sizeOfMessage - 2){ // decoding message
        fread(&tmp, sizeof(uc), 1, fin);
        int usedBits = 0;
        readByte(byte, tmp);
        for(; usedBits < BYTE_SIZE; ++usedBits){
            if (byte[usedBits] == '1' && curr -> right){
                curr = curr->right;
            }
            else if (byte[usedBits] == '0' && curr -> left){
                curr = curr->left;
            }
            if (!curr -> left && !curr -> right){
                fwrite(&curr->symbol, sizeof(uc), 1, fout);
                curr = root;
            }
        }
        ++numOfBytes;
    }
    fseek(fin, -1L, SEEK_END);
    fread(&usefulBits, sizeof(uc), 1, fin);
    fseek(fin, -2L, SEEK_CUR);
    fread(&tmp, sizeof(uc), 1, fin);
    readByte(byte, tmp);

    for (unsigned int i = offset; i <= (usefulBits - '0') ; ++i) { // reading last byte
        if (byte[i] == '1' && curr->right){
            curr = curr->right;
        }
        else if (byte[i] == '0' && curr->left){
            curr = curr->left;
        }
        if (!curr->left && !curr->right){
            fwrite(&curr->symbol, sizeof(uc), 1, fout);
            curr = root;
        }
    }
}


NODE* recoverTree(FILE* fin, unsigned int* offset, uc* byte){
    TREE* ptr;
    unsigned short numOfSymbols;
    uc tmp;
    int decodedSymbols = 0;
    int sizeOfStack = 0;
    fseek(fin, 3L, SEEK_SET);
    initStack(&ptr);
    fread(&numOfSymbols, sizeof(unsigned short), 1, fin);

    if (numOfSymbols == 1){     // pushing empty node for union
        pushInStack(ptr, NULL);
        ++sizeOfStack;
    }

    while (decodedSymbols < numOfSymbols || sizeOfStack != 1){
        if (*offset == 0){
            fread(&tmp, sizeof(uc), 1, fin);
            readByte(byte, tmp);
        }
        if (byte[0] == '1'){
            tmp = tmp << 1u;
            readByte(byte, tmp);
            fread(&tmp, sizeof(uc), 1, fin);
            for (unsigned int i = 7 - *offset , j = 7; i < BYTE_SIZE; ++i, --j){
                if ((1u << j) & tmp)
                    byte[i] = '1';
                else
                    byte[i] = '0';
            }
            pushInStack(ptr, newNode(0,pushChar(byte)));
            sizeOfStack++;
            decodedSymbols++;
            (*offset) = ((*offset) + 1) % BYTE_SIZE;
            tmp = tmp << *offset;
            readByte(byte, tmp);
        }
        else {
            unionNodes(ptr);
            --sizeOfStack;
            tmp = tmp << 1u;
            (*offset) = ((*offset) + 1) % BYTE_SIZE;
            readByte(byte, tmp);
        }
    }
    return ptr -> next -> node;
}


void codeMessage(FILE* fin, FILE* fout, uc** codesOfSymbols, BUFFER* ptr) {
    fseek(fin, 3L, SEEK_SET);
    uc curSymbol;

    while (fread(&curSymbol, sizeof(uc), 1, fin)){
        unsigned int lenOfSymbol = strlen((char*)codesOfSymbols[curSymbol]);
        for (int i = 0; i < lenOfSymbol; i++){
            writeBit(ptr, codesOfSymbols[curSymbol][i], fout);
        }
    }

    if (ptr -> bits % BYTE_SIZE){ // adding zeros in last byte
        int bitsCount = ptr -> bits % BYTE_SIZE;
        unsigned int usefulBits = bitsCount - 1;
        for (; bitsCount < BYTE_SIZE ; ++bitsCount)
            writeBit(ptr, '0', fout);
        writeChar(ptr, '0' + usefulBits, fout);
    }
    else
        writeChar(ptr, '7', fout);
}


void preOrder(FILE* fout, BUFFER* ptr, NODE* tmp, uc** codesOfSymbols, uc* value, int* bit){
    if (!tmp -> left && !tmp -> right){
        value[*bit] = '\0';
        writeBit(ptr, '1', fout);
        writeChar(ptr, tmp->symbol, fout);
        codesOfSymbols[tmp->symbol] = (uc*)malloc(sizeof(uc) * (*(bit) + 1));
        for (int i = 0; i <= *bit; ++i)
            codesOfSymbols[tmp->symbol][i] = value[i];
        return;
    }
    if (tmp -> left){
        value[(*bit)++] = '0';
        preOrder(fout, ptr, tmp -> left, codesOfSymbols, value, bit);
        --(*bit);
    }
    if (tmp -> right){
        value[(*bit)++] = '1';
        preOrder(fout, ptr, tmp -> right, codesOfSymbols, value, bit);
        --(*bit);
    }
    writeBit(ptr, '0', fout);
}


HEAP* creatingBigTree(HEAP* ptr){
    if (hasTreeBuilt(ptr)){ // for one symbol
        NODE* tmp = newNode(0, '\0');
        swap(ptr->array[0], tmp);
        ptr -> array[0] -> right = tmp;
    }
    else {
        while (!hasTreeBuilt(ptr)){
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


void pushingInTree(FILE* fin, FILE* fout, HEAP** ptr){
    int* frequency = (int*)calloc(CODES_ASCII, sizeof(int));
    unsigned short numOfsymbol = 0;
    uc readingChar;
    fseek(fin, 3L, SEEK_SET);

    while (fread(&readingChar, sizeof(uc), 1, fin)){ // counting of frequency for each symbol
        if (!(frequency[readingChar]))
            ++numOfsymbol;
        ++frequency[readingChar];
    }

    createHeap(ptr, numOfsymbol);
    for (int i = 0; (*ptr) -> heapSize < numOfsymbol; ++i){ // pushing in heap
        if (frequency[i]){
            (*ptr) -> array[(*ptr)->heapSize] = newNode(frequency[i], (uc)i);
            siftUp(*ptr, (*ptr)->heapSize);
            ++(*ptr) -> heapSize;
        }
    }

    fwrite(&numOfsymbol, sizeof(unsigned short), 1, fout);
    free(frequency);
}


long int sizeOfFile(FILE* fin){
    fseek(fin, 0L, SEEK_END);
    long int size = ftell(fin);
    fseek(fin, 0L, SEEK_SET);
    return size;
}


void initBuf(BUFFER** ptr){
    *ptr = (BUFFER*) malloc(sizeof(BUFFER));
    (*ptr)->buffer = (uc *) malloc(sizeof(uc) * MAX_BUF_SIZE);
    (*ptr) -> bits = 0;
    (*ptr) -> bytes = 0;
}


void freeMem(uc** codesOfSymbols, uc* code, BUFFER* ptr){
    if (codesOfSymbols != NULL)
        free(codesOfSymbols);
    else if (code != NULL)
        free(code);
    else if (ptr -> buffer != NULL)
        free(ptr->buffer);
    else if (ptr != NULL)
        free(ptr);
}


int main(){
    FILE* fin = fopen("in.txt", "rb");
    FILE* fout = fopen("out.txt", "wb");
    HEAP* minHeap;
    BUFFER* outBuffer;
    uc function;
    long int fSize = sizeOfFile(fin);

    if (fSize == 3) // check for empty file
        return 0;

    fread(&function, sizeof(uc), 1, fin);
    if (function == CODING){

        int bitInCode = 0;
        uc** codesOfSymbols = (uc**)malloc(sizeof(uc*) * CODES_ASCII); // coding buffer
        uc* code = (uc*)malloc(sizeof(uc) * MAX_CODE_SIZE); // buffer of new code for each symbol
        initBuf(&outBuffer);

        pushingInTree(fin, fout, &minHeap);
        creatingBigTree(minHeap); // tree of codes
        preOrder(fout, outBuffer, *minHeap->array, codesOfSymbols, code, &bitInCode);
        codeMessage(fin, fout, codesOfSymbols, outBuffer);

        fwrite(outBuffer->buffer, sizeof(uc), outBuffer->bytes , fout);
        freeMem(codesOfSymbols, code, outBuffer);
    }

    else if (function == DECODING){

        uc* byte = (uc*)malloc(sizeof(uc) * BYTE_SIZE);
        unsigned int offset = 0; // offset in byte where coding of tree ends

        NODE* root = recoverTree(fin, &offset, byte);
        decode(fin, fout, root, fSize, offset);

        if (byte != NULL)
            free(byte);
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
