#include <stdio.h>
#include <stdlib.h>
#include "udaan.h"

struct Node* createNode(struct Flight f) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = f;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insertBST(struct Node* root, struct Flight f) {
    if (root == NULL)
        return createNode(f);
    if (f.flightID < root->data.flightID)
        root->left = insertBST(root->left, f);
    else if (f.flightID > root->data.flightID)
        root->right = insertBST(root->right, f);
    return root;
}

struct Flight* searchBST(struct Node* root, int id) {
    if (root == NULL)
        return NULL;
    if (root->data.flightID == id)
        return &root->data;
    if (id < root->data.flightID)
        return searchBST(root->left, id);
    else
        return searchBST(root->right, id);
}

void printBST(struct Node* root) {
    if (root != NULL) {
        printBST(root->left);
        printf("Flight %d -> %s\n", root->data.flightID, root->data.destination);
        printBST(root->right);
    }
}
