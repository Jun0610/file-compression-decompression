typedef struct ListNode {
    struct ListNode * next;
    int ascii_count;
    TreeNode * root;
} ListNode;

ListNode * initializeOrderedList(int * ascii_count);
int determineSize(ListNode * to_add, ListNode * cur);
ListNode * addSorted(ListNode * head, ListNode * to_add);
void delListNode(ListNode * node);
void printList(ListNode * head);
int getSize(ListNode * head);
