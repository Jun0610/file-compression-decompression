#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "list.h"
#define ASCII_SIZE 256


//initialize orderd linked list from ASCII character count
ListNode * initializeOrderedList(int * ascii_count) {
    ListNode * head = NULL;

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (ascii_count[i] == 0) {
            continue;
        }

        ListNode * to_add = malloc(sizeof(ListNode));

        if (to_add == NULL) {
            fprintf(stderr, "Malloc failure in initializing list.\n");
            return NULL;
        }

        to_add -> ascii_count = ascii_count[i];      //number of occurences for a particular ASCII character
        to_add -> root = malloc(sizeof(TreeNode)); //create a TreeNode storing the ascii character
        to_add -> root -> left = NULL; //initialize the tree node to be a leaf node
        to_add -> root -> right = NULL;
        to_add -> root -> ascii = i;   //setting the ascii value of the treenode
        to_add -> next = NULL; //important to initialize every node's next to NULL

        head = addSorted(head, to_add);
    }
    
    if(head == NULL) {
        fprintf(stderr, "Array with ASCII counts was empty.\n");
        return NULL;
    }

    return head;
}

//returns 1 if to_add needs to be added before current node, else return 0
int determineSize(ListNode * to_add, ListNode * cur) {
    if (to_add -> ascii_count < cur -> ascii_count) {
        return 1;
    }

    else if (to_add -> ascii_count == cur -> ascii_count) {
        
        if (isLeafNode(to_add -> root) && !isLeafNode(cur -> root)) {
            return 1;
        }

        else if (isLeafNode(to_add -> root) && isLeafNode(cur -> root)) {
            
            if (to_add -> root -> ascii < cur -> root -> ascii) {
                return 1;
            }

            else {
                return 0;
            }
        }

        //if cur -> root is a leaf node and to_add -> root is a leaf node, then add the node after cur
        //if cur -> root is created first, then add to_add -> root after as well.
        else {
            return 0;
        }

    }

    else {
        return 0;
    }
}


//adds linked list node to existing linked list in sorted fashion
//returns head of sorted linked list
ListNode * addSorted(ListNode * head, ListNode * to_add) {
    if (head == NULL || head == to_add){
        return to_add;
    }

    ListNode * cur = head;
    ListNode dummy;
    dummy.next = head;
    ListNode * prev = &dummy;

    while (cur != NULL) {

        //if condition == 1, add node before current node, else add it after
        if (determineSize(to_add, cur)) {
            to_add -> next = cur;
            prev -> next = to_add;
            head = dummy.next;  //need this if we're adding the new node to the very front of the list
            return head;
        } 

        else {
            prev = cur;
            cur = cur -> next;
        }

        
    }

    prev -> next = to_add;
    return head;
    
}

//completely frees a list node
void delListNode(ListNode * node) {
    delTree(node -> root);
    free(node);
}

void printList(ListNode * head) {
    while (head != NULL) {
        printf("%d\n", head -> ascii_count);
        head = head -> next;
    }
}

int getSize(ListNode * head) {
    int size = 0;
    while (head != NULL) {
        size += 1;
        head = head -> next;
    }

    return size;
}
