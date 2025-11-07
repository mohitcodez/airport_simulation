#ifndef TREE_H
#define TREE_H

#include "flight.h"

typedef struct TreeNode {
    FlightNode *fnode; /* pointer to flight linked-list node */
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* Build and manage BST index by flight ID */
TreeNode* createTreeNode(FlightNode *fnode);
TreeNode* insertTreeNode(TreeNode *root, FlightNode *fnode);
TreeNode* buildTreeFromList(FlightNode *head);
FlightNode* treeSearchByID(TreeNode *root, int id);
void inorderPrintTree(TreeNode *root);
void freeTree(TreeNode *root);

#endif