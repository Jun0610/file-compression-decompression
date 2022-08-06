#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#define ASCII_SIZE 256


int isLeafNode(TreeNode * node) {
    if(node -> left == NULL && node -> right == NULL) {
        return 1;
    }
    
    return 0;
}

void preOrderPrintEncoding(TreeNode * root, char ** encodedTable) {

    if(root == NULL) {
        return;
    }

    if (isLeafNode(root)) {
        printf("%d : %s\n", root -> ascii, encodedTable[root -> ascii]);
    }
    
    preOrderPrintEncoding(root -> left, encodedTable);
    preOrderPrintEncoding(root -> right, encodedTable);
}

void preOrderPrint(TreeNode * root, FILE * out) {
    if (root == NULL) {
        return;
    }

    if (isLeafNode(root)) {
        fputc('1', out);
        fputc(root -> ascii, out);
    } else {
        fputc('0', out);
    }

    preOrderPrint(root -> left, out);
    preOrderPrint(root -> right, out);
    
}


//completely frees a tree
void delTree(TreeNode * root) {
    if(root == NULL) {
        return;
    }

    delTree(root -> left);
    delTree(root -> right);
    free(root);

}
