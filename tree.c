#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

TreeNode* createTreeNode(FlightNode *fnode) {
    TreeNode *n = (TreeNode*)malloc(sizeof(TreeNode));
    if (!n) return NULL;
    n->fnode = fnode;
    n->left = n->right = NULL;
    return n;
}

TreeNode* insertTreeNode(TreeNode *root, FlightNode *fnode) {
    if (!fnode) return root;
    if (!root) return createTreeNode(fnode);
    if (fnode->flight.id < root->fnode->flight.id) {
        root->left = insertTreeNode(root->left, fnode);
    } else if (fnode->flight.id > root->fnode->flight.id) {
        root->right = insertTreeNode(root->right, fnode);
    } else {
        /* duplicate id: ignore (shouldn't happen with unique IDs) */
    }
    return root;
}

TreeNode* buildTreeFromList(FlightNode *head) {
    TreeNode *root = NULL;
    FlightNode *cur = head;
    while (cur) {
        root = insertTreeNode(root, cur);
        cur = cur->next;
    }
    return root;
}

FlightNode* treeSearchByID(TreeNode *root, int id) {
    while (root) {
        if (id == root->fnode->flight.id) return root->fnode;
        if (id < root->fnode->flight.id) root = root->left;
        else root = root->right;
    }
    return NULL;
}

void inorderPrintTree(TreeNode *root) {
    if (!root) return;
    inorderPrintTree(root->left);
    printFlight(&(root->fnode->flight));
    inorderPrintTree(root->right);
}

void freeTree(TreeNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}