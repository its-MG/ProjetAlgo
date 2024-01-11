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

// Function to enqueue a tree node into the queue
void Enqueue(Queue* queue, TreeNode* data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->data = data;
    newNode->next = NULL;

    if (IsQueueEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue a tree node from the queue
TreeNode* Dequeue(Queue* queue) {
    if (IsQueueEmpty(queue)) {
        return NULL;
    }

    QueueNode* temp = queue->front;
    TreeNode* data = temp->data;

    queue->front = temp->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);

    return data;
}

// Function to free the queue
void FreeQueue(Queue* queue) {
    while (!IsQueueEmpty(queue)) {
        Dequeue(queue);
    }

    free(queue);
}