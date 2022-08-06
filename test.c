#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII_SIZE 256

void createCompressedFile(char * fileName, char * toReturn) {
    char compressedFile[100];
    toReturn = compressedFile;
    char extension[] = ".cmp";
    int strLength = (int) strlen(fileName);
    for (int i = 0; i < strLength; i++) {
        compressedFile[i] = fileName[i];
        if (fileName[i] == '.') {
            for (int j = 1; j <= 3; j++) {
                compressedFile[i + j] = extension[j];
            }
            break;
        }
    }
}

void findDiff(FILE * f1, FILE * f2) {
    int char1 = fgetc(f1);
    int char2 = fgetc(f2);
    int byte_count = 0;

    while (char1 != EOF) {
        byte_count += 1;
        if (char1 != char2 ) {
             printf("byte: %d - f1: %d f2: %d\n", byte_count, char1, char2);
             break;
        }
    


        char1 = fgetc(f1);
        char2 = fgetc(f2);       
    }

}

int main (int argc, char * argv[]) {

    FILE * f1 = fopen(argv[1], "r");
    FILE * f2 = fopen(argv[2], "r");

    findDiff(f1, f2);

    // char test[] = "test";
    // char * dest= NULL;
    // createCompressedFile(test, dest);
    // printf("%s\n", dest);

}
