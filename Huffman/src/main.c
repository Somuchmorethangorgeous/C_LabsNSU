#include <stdio.h>
#include <malloc.h>
#include "lib.h"


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
