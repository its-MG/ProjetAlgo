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
// Function to draw a binary tree recursively
void drawTree(TreeNode* root, int x, int y, int hSpacing, int vSpacing) {
    // Define a lighter green color
    Color lightYel = (Color){147, 197, 114, 255}; // Adjust the green component to make it lighter
    if (root != NULL) {
        // Draw left subtree
        if (root->left != NULL) {
            // Adjust the position of the line to avoid crossing the number
            DrawLine(x - 3, y + 3, x - hSpacing + 3, y + vSpacing - 3, BLACK);
            drawTree(root->left, x - hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }

        // Draw right subtree
        if (root->right != NULL) {
            // Adjust the position of the line to avoid crossing the number
            DrawLine(x + 3, y + 3, x + hSpacing - 3, y + vSpacing - 3, BLACK);
            drawTree(root->right, x + hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }

        // Determine node color
        Color nodeColor = (root->isNewlyInserted) ? lightYel : BLACK;

        // Draw filled circle without an outline
        DrawCircleV((Vector2){(float)x, (float)y}, NODE_RADIUS, nodeColor);

        // Calculate the position to center the text within the circle
        int textWidth = MeasureText(TextFormat("%d", root->data), 20);
        int textHeight = 20;  // Assuming font size is 20
        int textX = x - textWidth / 2;
        int textY = y - textHeight / 2;

        // Draw current node value
        DrawText(TextFormat("%d", root->data), textX, textY, 20, WHITE);
    }
}

// Update the tree, reset the color of newly inserted nodes after a certain duration
void updateTree(TreeNode* root, double frameTime) {
    if (root != NULL) {
        if (root->isNewlyInserted) {
            root->colorTimer += frameTime;

            // If the color change duration has passed, reset the color and timer
            if (root->colorTimer >= 0.5) {  // Change the duration as needed
                root->isNewlyInserted = false;
                root->colorTimer = 0.0;
            }
        }

        // Update left subtree
        updateTree(root->left, frameTime);

        // Update right subtree
        updateTree(root->right, frameTime);
    }
}