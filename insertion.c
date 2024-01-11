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

// Function to initialize an empty queue
Queue* InitQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to check if the queue is empty
int IsQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Function to get the number of nodes in the queue (used for level tracking)
int QueueSize(Queue* queue) {
    int count = 0;
    QueueNode* temp = queue->front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}