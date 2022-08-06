typedef struct TreeNode {
    int ascii;
    struct TreeNode * left;
    struct TreeNode * right;
} TreeNode;

int isLeafNode(TreeNode * node);
void preOrderPrintEncoding(TreeNode * root, char ** encodedTable);
void delTree(TreeNode * root);
void preOrderPrint(TreeNode * root, FILE * out);
