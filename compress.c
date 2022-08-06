#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"
#include "compress.h"
#define ASCII_SIZE 256

void createCompressedFile(char * fileName, char * compressedFile) {

    char extension[] = ".cmp";
    int strLength = (int) strlen(fileName);
    int i;
    for (i = 0; i < strLength; i++) {
        if (fileName[i] == '.') {
            compressedFile[i] = '\0';
            break;
        }
        compressedFile[i] = fileName[i];
        if (i == strLength - 1) {
            compressedFile[i + 1] = '\0';
        }
    }

    strcat(compressedFile, extension);
}


//counts all ASCII character occurences in a file and stores them in an array of length 256
//returns size of the file in bytes as a long integer
long countChar(FILE * in, int * count){

    int cur_char = fgetc(in);
    long size = 0; //size of the file in bytes

    if (cur_char == EOF) {
        printf("File is empty.\n");
        return EXIT_SUCCESS;
    }

    while (cur_char != EOF) {
        size += 1;
        count[cur_char] += 1;
        cur_char = fgetc(in);
    }

    return size;
}

//builds a huffman coding tree from the sorted linked list
TreeNode * buildHuffmanTree(ListNode * head) {

    if (head == NULL) {
        fprintf(stderr, "Linked list is empty.\n");
        return NULL;
    }
    ListNode * temp = NULL;

    while (head -> next != NULL) {
        ListNode * newNode = malloc(sizeof(ListNode));
        newNode -> root = malloc(sizeof(TreeNode));

        if (newNode == NULL) {
            fprintf(stderr, "Malloc Failure in buildHuffmanTree.\n");
            return NULL;
        }

        //the newNode's tree is made up by combining the trees of the first 2 elements in the linked list
        newNode -> root -> left = head -> root;
        newNode -> root -> right = head -> next -> root;
        newNode -> ascii_count = head -> ascii_count + head -> next -> ascii_count;
        newNode -> root -> ascii = -1;
        newNode -> next = NULL;

        if (head -> next -> next == NULL) {
            temp = newNode;
        } else {
            temp = head -> next -> next;    //move the temp to point to the new head which is the 3rd node of the linked list
        }
        

        //free the first two nodes;
        free(head -> next);
        free(head);

        head = temp;
        head = addSorted(head, newNode);
        
    }

    //if linked list only has 1 element
    TreeNode * returnRootNode = head -> root;
    
    //free the last linked list node but not the root within the node
    free(head);
    return returnRootNode;
    
}


//returns an array of Strings (char *) that represent the encoded version of characters
/*
* example output:
* g : 00
* o : 01
*/
char ** buildEncodedTable(TreeNode * root) {

    char ** encodedTable = malloc(sizeof(char *) * ASCII_SIZE); //only 256 possible ASCII characters
    char encodedChar[500] = {'\0'}; //initialize the encoded string

    int index = 0; //the true length of the string is 1 since '\0' is counted

    buildEncodedTableHelper(root, encodedChar, '\0', encodedTable, 0, &index);

    return encodedTable;

}

//helper function to buildEncodedTable
//RECURSIVE FUNCTION
void buildEncodedTableHelper(TreeNode * root, char * encodedChar, char charToAdd, char ** encodedTable, int encoded_length, int * index) {


    if (root == NULL) {
        return;
    }


    //once we know that the destination (encodedChar) is big enough, we append the next coded character
    //if we go left, append 0, if we go right append 1
    encodedChar[*index] = charToAdd;

    if (encoded_length != 0) {
        *index += 1;
    }
    


    //once we reach a leaf node, allocate memory for that ASCII value in the table and copy the string into the allocated memory
    //must use strcpy for deep copy
    if (isLeafNode(root)) {
        encodedTable[root -> ascii] = malloc((sizeof(char) * encoded_length) + 1);
        
        if (encodedTable[root -> ascii] == NULL) {
            fprintf(stderr, "Malloc failure in buildEncodedTableHelper.\n");
            return;
        }

        strcpy(encodedTable[root -> ascii], encodedChar);


        //the latest value of encodedChar must be removed (sort of like backtracking)
        //eg: if g : 00, we need to remove the second 0 before returning to parent ndoe in tree
        encodedChar[encoded_length - 1] = '\0';
        
        //the index must also be decremented
        *index -= 1;

        return;
    }

    buildEncodedTableHelper(root -> left, encodedChar, '0', encodedTable, encoded_length + 1, index);
    buildEncodedTableHelper(root -> right, encodedChar, '1', encodedTable, encoded_length + 1, index);


    //the latest value of encodedChar must be removed (sort of like backtracking)
    //must be done for leaf nodes and non leaf nodes
    encodedChar[encoded_length - 1] = '\0';

    //the index must also be decremented
    *index -= 1;


}


//writes compressed bits into the new compressed file
void writeCompressedBits(char ** encodedTable, FILE * in, FILE * out) {
    int byte_index = 0; //range is from 0 to 7: 0 is least sig bit and 7 is most sig bit
    int string_index = 0;
    int cur_byte = 0;
    int curCharFile;
    do {

        
        curCharFile = fgetc(in);

        if (curCharFile == -1) {
            break;
        }
        char * curSymbol = encodedTable[curCharFile];

        while (1) {
        
            //first check if the current byte is full, if it is, write it and reset the byte to 0
            if(byte_index == 8) {
                int check = fputc(cur_byte, out);
                //printf("%d\n", (int) (char) cur_byte);

                //error handling
                if(check == EOF) {
                    fprintf(stderr, "Error writing binary to file in writeCompressedBits.\n");
                    return;
                }

                cur_byte = 0;
                byte_index = 0;
            }

            char curChar = curSymbol[string_index];

            //while not end the end of the string
            if (curChar == '\0') {
                string_index = 0;
                break;
            }

            if (curChar == '0') {
                //to add 0, just increment index, since the byte starts of at 0
                byte_index += 1;

            } else if (curChar == '1') {
                cur_byte = addOneToByte(byte_index, cur_byte);
                byte_index += 1;

            } else {
                fprintf(stderr, "Compressed Text in string version is invalid.\n");
                return;
            }


            string_index++;
        
        }

    } while (curCharFile != EOF);

    //if there are still remaining bits not written yet, just write the current byte into the file, the MSB will be padded with 0s.
    if (byte_index != 0) {
        int check = fputc(cur_byte, out);
        //printf("%d\n", (int) (char) cur_byte);
            //error handling
        if(check == EOF) {
            fprintf(stderr, "Error writing binary to file in writeCompressedBits.\n");
            return;
        }
    }

}

//returns the current byte after adding one to it
int addOneToByte (int byte_index, int cur_byte) {
    int mask =  (0xFF >> 7) << byte_index;
    cur_byte = cur_byte | mask;
    
    return cur_byte;
}


/*
RECURSIVE FUNCTION
*/
void writeBinaryHuffmanTopology(TreeNode * root, FILE * out, int * cur_byte, int * byte_index, long * topologySize) {
   
    if (root == NULL) {
        return;
    }
    //printf("%d\n", isLeafNode(root));
    
    if(isLeafNode(root)) {
        *cur_byte = addOneToByte(*byte_index, *cur_byte);
        *byte_index += 1;
        //printf("1 ");
        //printf("%c ",root->ascii);

        //if current byte is full write it to the file and reset the byte and its index
        if (*byte_index > 7) {
            int check = fputc(*cur_byte, out);
           

            if (check == EOF) {
                fprintf(stderr, "fputc Error in writeBinaryHuffmanTopology.\n");
                return;
            }
            *topologySize += 1;
            *byte_index = 0;
            *cur_byte = 0;
        }

        //must always write character to byte if its a leaf node
        *cur_byte = addCharToByte(out, *cur_byte, byte_index, root -> ascii);

        //error handling
        if (*cur_byte == EOF) {
            return;
        }

        *topologySize += 1;
    } 

    //if none leaf node, then we add 0 to the byte, that just means incrementing the index, since the byte starts of as 00000000.
    else {
        //printf("0 ");
        *byte_index += 1;

        //if current byte is full write it to the file and reset the byte and its index
        if (*byte_index > 7) {
            int check = fputc(*cur_byte, out);

            if (check == EOF) {
                fprintf(stderr, "fputc Error in writeBinaryHuffmanTopology.\n");
                return;
            }
            *topologySize += 1;
            *byte_index = 0;
            *cur_byte = 0;
        }
    }
    
    writeBinaryHuffmanTopology(root -> left, out, cur_byte, byte_index, topologySize);
    writeBinaryHuffmanTopology(root -> right, out, cur_byte, byte_index, topologySize);
    
}

/*
adds character to byte 
if byte is full after character addition, write the byte to file
returns the new byte with leftover bits or 0 depending on what was written to the file
0 <= byte_index <= 7 
*/
int addCharToByte(FILE * out, int cur_byte, int * byte_index, int charToAdd) {
    int mask = charToAdd << *byte_index;
    cur_byte = cur_byte | mask;

    //think about the case where byte_index == 7, then there will be 7 leftover bits
    int leftoverBits = *byte_index;

    //either way, we need to write the cur_byte to file
    int check = fputc(cur_byte, out);

    //error handling
    if (check == EOF) {
        fprintf(stderr, "fputc Error in addChar to Byte.\n");
        return EOF;
    }

    *byte_index =  0;
    cur_byte = 0;

    //if there are no leftover bits, just return cur_byte = 0
    if (leftoverBits == 0) {
        return cur_byte;
    }

    //else: think about it like this
    //if there are 7 bits to write, we need to right shift the charToAdd by 1
    mask = charToAdd >> (8 - leftoverBits);
    cur_byte = cur_byte | mask;
    *byte_index += leftoverBits;

    return cur_byte;
}

//frees the encoded table
void delEncodedTable(char ** encodedTable) {

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (encodedTable[i] != NULL) {
            free(encodedTable[i]);
        }
    }

    free(encodedTable);
}

void displayCompression(long compressedSize, long uncompressedSize) {
    printf("====================\n");
    printf("Uncompressed File Size = %ld Bytes\n", uncompressedSize);
    printf("Compressed File Size = %ld Bytes\n", compressedSize);

    double percentageDiff = ((double) labs(uncompressedSize - compressedSize)) / uncompressedSize * 100;

    if (compressedSize > uncompressedSize) {
        printf("The compressed file is %.2lf%% larger.\n", percentageDiff);
        printf("For better results, please try inputting a larger text file to be compressed.\n");
    } else {
        printf("The compressed file is %.2lf%% smaller!\n", percentageDiff);
    }
    printf("====================\n");
}

