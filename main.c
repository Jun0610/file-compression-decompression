#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "compress.h"
#include "decompress.h"
#define ASCII_SIZE 256


int main(int argc, char * argv[]) {

    #ifdef COMPRESS

    // if (argc != 2) {
    //     fprintf(stderr, "Please input the name of the file to be compressed.\n");
    //     return EXIT_FAILURE;
    // }

    int ascii_count[ASCII_SIZE] = {0};    //array of count of ASCII characters
    long uncompressedSize; //uncompressed file size

    FILE * uncompressed = fopen(argv[1], "r");

    //error handling
    if (uncompressed == NULL) {
        fprintf(stderr, "Could not open uncompressed file.\n");
        return EXIT_FAILURE;
    }

    //creating the ASCII count array and determining the size (in bytes) of the uncompressed file
    uncompressedSize = countChar(uncompressed, ascii_count);

    //error handling
    if (uncompressedSize == EXIT_FAILURE) {
        fprintf(stderr, "Failure in counting ASCII characters in file.\n");
        return EXIT_FAILURE;
    }

    //reset pointer to start of file
    fseek(uncompressed, 0, SEEK_SET);

    //builds the ordered linked list
    ListNode * head = initializeOrderedList(ascii_count);
    //printList(head);
    //builds the huffman encoding tree
    TreeNode * huffmanRoot = buildHuffmanTree(head);

    //uses the huffman tree to build a table of ascii values and their associated binary values
    //eg - g:00, e:01
    char ** encodedTable = buildEncodedTable(huffmanRoot);

    char compressedFile[100];
    createCompressedFile(argv[1], compressedFile);

    FILE * compressed = fopen(compressedFile, "w+");

    if (compressed == NULL) {
        fprintf(stderr, "Could not open compressed file.\n");
        return EXIT_FAILURE;
    }


    int byte_index = 0;
    int cur_byte = 0;
    long topologySize = 0;

    //first write the topoloySize and uncompressed file size to the file
    fwrite(&topologySize, sizeof(long), 1, compressed);
    
    fwrite(&uncompressedSize, sizeof(long), 1, compressed);



    //writes the binary Huffman tree topology to the file
    writeBinaryHuffmanTopology(huffmanRoot, compressed, &cur_byte, &byte_index, &topologySize);

    //there is a high likelihood that the final byte of the huffman topology is not written cuz of the way
    //writeBinaryHuffmanTopology is written

    //hence we need to check for that
    if (byte_index != 0) {
        int check = fputc(cur_byte, compressed);

        if (check == EOF) {
            fprintf(stderr, "fputc error in main.\n");
            return EXIT_FAILURE;
        }

        topologySize += 1;
        cur_byte = 0;
    }

    //writes the binary encoded version of the original file to the file
    writeCompressedBits(encodedTable, uncompressed, compressed);

    long compressedSize = ftell(compressed);

    //reset pointer to start of file so we can write the actual topologySize
    fseek(compressed, 0, SEEK_SET);
    fwrite(&topologySize, sizeof(long), 1, compressed);

    //display results of compression
    displayCompression(compressedSize, uncompressedSize);   



    fclose(compressed);
    fclose(uncompressed);
    delEncodedTable(encodedTable);
    delTree(huffmanRoot);

    return EXIT_SUCCESS;

    #endif
 

    #ifdef DECOMPRESS


    if (verifyCompressedFile(argv[1]) != 0) {
        fprintf(stderr, "File must be of type .cmp!\n");
        return EXIT_FAILURE;
    }


    FILE * compressed = fopen(argv[1], "r");
    
    if (compressed == NULL) {
        fprintf(stderr, "Could not open compressed file.\n");
        return EXIT_FAILURE;
    }
    
    long topologySize;  //size of the huffman tree topology in bytes
    long uncompressedSize; //size of the uncompressed file in bytes
    
    if (fread(&topologySize, sizeof(long), 1, compressed) != 1) {
        fprintf(stderr, "fread failure in main.\n");
        return EXIT_FAILURE;
    }

    if (fread(&uncompressedSize, sizeof(long), 1, compressed) != 1) {
        fprintf(stderr, "fread failure in main.\n");
        return EXIT_FAILURE;
    }

    int curByte = fgetc(compressed); //get the next byte to be passed into the function to build the huffman tree
    int byteIndex = 0;              //index of the byte


    //build huffman tree using tree topology in compressed file
    TreeNode * huffmanRoot = buildDecompressHuffmanTree(compressed, &curByte, &byteIndex);
    char decompressedFile[100];
    createDecompressedFile(argv[1], decompressedFile);

    FILE * restore = fopen(decompressedFile, "w");
    if (restore == NULL) {
        fprintf(stderr, "Could not open file for restoration.\n");
        return EXIT_FAILURE;
    }


    //decompress compressed bits by traversing huffman tree
    decompress(huffmanRoot, compressed, restore, uncompressedSize);
    long compressedSize = ftell(compressed);

    //display results of decompression
    displayDecompression(uncompressedSize, compressedSize);

    fclose(compressed);
    fclose(restore);
    delTree(huffmanRoot);

    #endif

    
    
}
