#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_RADIUS 20
#define MAX_INPUT_LENGTH 2 // Maximum number of digits allowed

// Binary tree node structure
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
    bool isNewlyInserted;  // Variable to track the newly inserted node
    double colorTimer;     // Timer to control the color change duration
} TreeNode;

// Function to create a new binary tree node
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->isNewlyInserted = false;  // Initially, the node is not newly inserted
    newNode->colorTimer = 0.0;         // Initialize color change timer
    return newNode;
}

// Function to insert a new node into the binary tree (sorted)
TreeNode* insertNode(TreeNode* root, int data, int currentDepth, int maxDepth, int* nodesCreated) {
    if (currentDepth > maxDepth || *nodesCreated >= 31) {
        // Reached the maximum depth or the maximum number of nodes, do not insert further
        return root;
    }

    if (root == NULL) {
        *nodesCreated += 1;
        TreeNode* newNode = createNode(data);
        newNode->isNewlyInserted = true;  // Mark the node as newly inserted
        return newNode;
    }

    // Insert to the left if the value is smaller
    if (data < root->data) {
        root->left = insertNode(root->left, data, currentDepth + 1, maxDepth, nodesCreated);
    }
    // Insert to the right if the value is larger or equal (for sorted tree)
    else {
        root->right = insertNode(root->right, data, currentDepth + 1, maxDepth, nodesCreated);
    }

    return root;
}