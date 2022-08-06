#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "decompress.h"
#define ASCII_SIZE 256

void createDecompressedFile(char * fileName, char * decompressedFile) {
    char extension[] = ".dcmp";
    int strLength = (int) strlen(fileName);
    int i;
    for (i = 0; i < strLength; i++) {
        if (fileName[i] == '.') {
            decompressedFile[i] = '\0';
            break;
        }
        decompressedFile[i] = fileName[i];
        if (i == strLength - 1) {
            decompressedFile[i + 1] = '\0';
        }
    }

    strcat(decompressedFile, extension);
    
}

//verifies that the compressed file is of type .cmp
int verifyCompressedFile(char * fileName) {
    int stringLength = (int) strlen(fileName);
    char lastFour[5]; 
    int count = 0;
    
    for (int i = 0; i < stringLength; i++) {
        count++;
        
        if (i >= stringLength - 4) {
            lastFour[i - (stringLength - 4)] = fileName[i];
        }
    }

    lastFour[4] = '\0';
    char cmp[] = ".cmp";

    return strcmp(lastFour, cmp);

}

TreeNode * buildDecompressHuffmanTree(FILE * in, int * curByte, int * byteIndex) {
    if (*byteIndex >= 8) {
        *curByte = fgetc(in);
        *byteIndex = 0;
    }
    int mask = 0xFF >> 7 << *byteIndex;
    int curBit = (mask & *curByte) >> *byteIndex;

    *byteIndex += 1;
    TreeNode * root = NULL;

    if (curBit == 1) {

        root = malloc(sizeof(TreeNode));
        root -> left = NULL;
        root -> right = NULL;
        root -> ascii = readAscii(in, curByte, byteIndex);
        return root;
    } 
    
    else if (curBit == 0) {
        root = malloc(sizeof(TreeNode));
        root -> left = NULL;
        root -> right = NULL;
        root -> ascii = 0;
    }

    root -> left = buildDecompressHuffmanTree(in, curByte, byteIndex);
    root -> right = buildDecompressHuffmanTree(in, curByte, byteIndex);
    return root;
}

int readAscii(FILE * in, int * curByte, int * readByteIndex) {
    if (*readByteIndex % 8 == 0) {
        int toReturn = fgetc(in);
        *curByte = fgetc(in);
        *readByteIndex = 0;
        return toReturn;
    }
    int nextByte = fgetc(in);
    int toReturn = 0;
    int writeByteIndex = 0;
    int mask;

    while (writeByteIndex < 8) {
        mask = 0XFF >> 7 << (*readByteIndex % 8);
        mask = mask & *curByte;
        mask = mask >> (*readByteIndex % 8);

        toReturn = toReturn | (mask << writeByteIndex);
        *readByteIndex += 1;
        writeByteIndex += 1;

        if (*readByteIndex == 8) {

            *curByte = nextByte;
        }


    }
    *readByteIndex = *readByteIndex % 8;
    return toReturn;

}

void decompress(TreeNode * huffmanRoot, FILE * compressed, FILE * restored, long uncompressedSize) {
    long bytesRead = 0;
    TreeNode * traverse = huffmanRoot;
    int byteIndex = 0;
    int curByte = fgetc(compressed);

    while (bytesRead < uncompressedSize) {
        if (isLeafNode(traverse)) {
            fputc(traverse -> ascii, restored);
            traverse = huffmanRoot;
            bytesRead += 1;
            continue;
        }

        if (byteIndex >= 8) {
            curByte = fgetc(compressed);
            byteIndex = 0;
        }
        
        int mask = 0xFF >> 7 << byteIndex;
        int curBit = (mask & curByte) >> byteIndex;
        byteIndex++;

        if (curBit == 0) {
            traverse = traverse -> left;
        } 
        else if (curBit == 1) {
            traverse = traverse -> right;
        }
        else {
            fprintf(stderr, "Error in decompress function.\n");
            return;
        }

    }
}

void displayDecompression(long uncompressedSize, long compressedSize) {
    printf("====================\n");
    printf("Compressed File Size = %ld bytes\n", compressedSize);
    printf("Restored File Size = %ld bytes\n", uncompressedSize);
    printf("Successfully restored file!\n");
    printf("====================\n");
}
