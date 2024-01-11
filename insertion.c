#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Binary tree node structure
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Queue node structure for level-order traversal
typedef struct QueueNode {
    TreeNode* data;
    struct QueueNode* next;
} QueueNode;

// Queue structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;
