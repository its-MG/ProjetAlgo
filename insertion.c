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
// Function to create a new binary tree node
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to insert a new node into the binary tree level by level
TreeNode* insertNode(TreeNode* root, int data, int* errorOccurred) {
    if (root == NULL) {
        return createNode(data);
    }

    Queue* queue = InitQueue(); // Implement a simple queue for level-order traversal
    Enqueue(queue, root);

    int level = 1; // Track the level of the current node

    while (!IsQueueEmpty(queue)) {
        int nodesAtCurrentLevel = QueueSize(queue);

        while (nodesAtCurrentLevel > 0) {
            TreeNode* current = Dequeue(queue);

            if (level < 5) {
                if (current->left == NULL) {
                    current->left = createNode(data);
                    FreeQueue(queue);
                    return root;
                } else {
                    Enqueue(queue, current->left);
                }

                if (current->right == NULL) {
                    current->right = createNode(data);
                    FreeQueue(queue);
                    return root;
                } else {
                    Enqueue(queue, current->right);
                }
            } else {
                // Display error message and stop insertion
                *errorOccurred = 1;
                FreeQueue(queue);
                return root;
            }

            nodesAtCurrentLevel--;
        }

        level++;
    }

    FreeQueue(queue);
    return root;
}

// Function to draw a binary tree recursively
void drawTree(TreeNode* root, int x, int y, int hSpacing, int vSpacing) {
    if (root != NULL) {
        // Draw branches
        if (root->left != NULL) {
            DrawLine(x, y, x - hSpacing, y + vSpacing, LIGHTGRAY);
            drawTree(root->left, x - hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }

        if (root->right != NULL) {
            DrawLine(x, y, x + hSpacing, y + vSpacing, LIGHTGRAY);
            drawTree(root->right, x + hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }
        Color customColor ={ 0x1F, 0x5E, 0x91, 255 }; // couleur bleu pour les autres noeuds
        // Draw circle (node)
        DrawCircle(x, y, 28,customColor);

        // Calculate the position to center the text within the circle
        int textWidth = MeasureText(TextFormat("%d", root->data), 20);
        int textHeight = 20;  // Assuming font size is 20
        int textX = x - textWidth / 2;
        int textY = y - textHeight / 2;

        // Draw current node value
        DrawText(TextFormat("%d", root->data), textX, textY, 20, WHITE);
    }
}
// Function to draw a rounded rectangle button
void DrawRoundedButton(Rectangle bounds, const char* text, Color bgColor, Color textColor) {
    DrawRectangleRounded(bounds, 0.1, 0, bgColor);
    DrawRectangleRoundedLines(bounds, 0.1, 0, 2, BLACK);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
    Vector2 textPos = {(float)(bounds.x + bounds.width / 2 - textSize.x / 2), (float)(bounds.y + bounds.height / 2 - textSize.y / 2)};
    
    DrawTextEx(GetFontDefault(), text, textPos, 20, 1, textColor);
}

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Visualisation de l'insertion");

    SetTargetFPS(60);
    
    TreeNode* root = NULL;
    char inputValue[4] = "";
    int inputLength = 0;
    int errorOccurred = 0; // Variable to track whether an error has occurred
    Color backgroundColor = (Color){ 15, 16, 53, 255 };
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(backgroundColor);

        // Display instructions
        DrawText("Insertion dans un arbre binaire ordonnée par niveau", 180, 10, 23, (Color){0x4F, 0x97, 0xA9, 0xFF});

        // Draw the binary tree
        drawTree(root, screenWidth / 2,100, 200, 100);

        // Draw the input text box
        Rectangle textBoxBounds = {10, screenHeight - 40, 100, 30};

        // Draw the grey rectangle (text box)
        DrawRectangleRec(textBoxBounds, LIGHTGRAY);

        // Draw the black border for the text box
        DrawRectangleLinesEx(textBoxBounds, 2, BLACK);
        DrawText(inputValue, textBoxBounds.x + 10 , textBoxBounds.y + 7, 20, BLACK);

        // Draw the insert button
        Rectangle buttonBounds = {120, screenHeight - 40, 200, 30};
        DrawRoundedButton(buttonBounds, "Insérer",(Color){0x4F, 0x97, 0xA9, 0xFF}, WHITE);

        // Check if the button is pressed
        if (CheckCollisionPointRec(GetMousePosition(), buttonBounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (inputLength > 0) {
                int value = atoi(inputValue);
                root = insertNode(root, value, &errorOccurred);
                // Reset input buffer when the button is pressed and a number is entered
                inputLength = 0;
                memset(inputValue, 0, sizeof(inputValue));
            }
        }
        
        // add a descriptive message on how it works 
        DrawRectangleRounded((Rectangle){170, screenHeight - 150, 670, 60}, 0.2, 0, (Color) { 0x29, 0x32, 0x41, 200 });
        DrawText("Entrer la valeur du noeud et appuyer sur insérer pour insérer\n\n la valeur désirée d'une manière ordonnée par niveau",180, screenHeight - 150 + 5, 20, (Color){ 0x98, 0xC1, 0xD9, 255 });


        // Display error message
        if (errorOccurred) {
            DrawText("Error: Tree is full (reached level 5)", 10, screenHeight -80, 20, (Color){124, 10, 2, 255});
            // Do not reset errorOccurred after displaying the error message
            // The message will remain on the screen until the program is closed
        }

        // Handle text input
        int key = GetKeyPressed();
        if (key >= KEY_ZERO && key <= KEY_NINE) {  // Only allow numeric input
            if (inputLength < 3) {
                inputValue[inputLength++] = '0' + (key - KEY_ZERO); // Convert key to digit character
            }
        } else if (key == KEY_BACKSPACE) {  // Handle backspace
            if (inputLength > 0) {
                inputValue[--inputLength] = '\0'; // Remove the last entered digit
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}