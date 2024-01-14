#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NODE_RADIUS 28
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
    // Define a lighter blue color
    Color lightYel = (Color){0x4F, 0x97, 0xA9, 0xFF}; // Adjust the blue component to make it lighter
    if (root != NULL) {
        // Draw left subtree
        if (root->left != NULL) {
            // Adjust the position of the line to avoid crossing the number
            DrawLine(x - 3, y + 3, x - hSpacing + 3, y + vSpacing - 3, LIGHTGRAY);
            drawTree(root->left, x - hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }

        // Draw right subtree
        if (root->right != NULL) {
            // Adjust the position of the line to avoid crossing the number
            DrawLine(x + 3, y + 3, x + hSpacing - 3, y + vSpacing - 3, LIGHTGRAY);
            drawTree(root->right, x + hSpacing, y + vSpacing, hSpacing / 2, vSpacing);
        }

        // Determine node color
        #define BLU CLITERAL(Color){ 0x1F, 0x5E, 0x91, 255 }
        Color nodeColor = (root->isNewlyInserted) ? lightYel : BLU;

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

int main() {
    const int screenWidth = 1000;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Création d'un Arbre binaire");
    SetTargetFPS(60);

    TreeNode* root = NULL;  // Initially an empty tree

    // Variable to store the maximum depth
    const int maxDepth = 5;

    // Variable to keep track of the number of nodes created
    int nodesCreated = 0;

    // Flag to switch between sorted and non-sorted trees
    bool isSortedTree = true;

    // User input related variables
    char inputBuffer[MAX_INPUT_LENGTH + 1];
    int inputLength = 0;
    int totalNodes = 0;
    int previousTotalNodes = 0; // Added to keep track of previous totalNodes

    Rectangle createButtonRect = {10, screenHeight - 40, 90, 30}; // Rectangle for the "Create" button
    Rectangle toggleButtonRect = {130, screenHeight - 40, 240, 30}; // Rectangle for the "Toggle Tree Type" button

    float nodeCreationDelay = 0.5f; // Adjust the delay time as needed
    float elapsedTime = 0.0f;

    bool createTree = false;  // Variable to control tree creation

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){ 15, 16, 53, 255 });

        // Display instructions
        DrawText("Création d'un arbre binaire noeud par noeud", 250, 10, 23, (Color){0x4F, 0x97, 0xA9, 0xFF});

        // Check if the "Create" button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), createButtonRect)) {
            createTree = true;  // Set the flag to start tree creation
        }

        if (createTree) {
            // Introduce a delay between node creations
            elapsedTime += GetFrameTime();

            if (elapsedTime >= nodeCreationDelay && nodesCreated < totalNodes) {
                // Generate a random value for the new node
                int randomValue = GetRandomValue(1, 100);

                // Insert the new node into the tree based on the tree type
                if (isSortedTree) {
                    root = insertNode(root, randomValue, 1, maxDepth, &nodesCreated);
                } else {
                    root = insertNode(root, randomValue, 1, maxDepth, &nodesCreated);
                }

                elapsedTime = 0.0f; // Reset the timer for the next delay
            }

            // Check if tree creation is complete
            if (nodesCreated >= totalNodes) {
                createTree = false;  // Reset the flag
            }
        }

        // Check if the "Toggle Tree Type" button is pressed
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), toggleButtonRect)) {
            isSortedTree = !isSortedTree;
        }

        
        // add a descriptive message on how it works 
        DrawRectangleRounded((Rectangle){150, screenHeight - 170, 670, 60}, 0.2, 0, (Color) { 0x29, 0x32, 0x41, 200 });
        DrawText("Saisir le nombre total et appuyer sur la touche  « entrer »\n\n puis le bouton créer pour lancer la creation de l'arbre",160, screenHeight - 170 + 5, 20,(Color){ 0x98, 0xC1, 0xD9, 255 });

        // Draw the "Create" button
        DrawRectangleRounded(createButtonRect, 0.2, 0, (Color){0x4F, 0x97, 0xA9, 0xFF});
        DrawText("Créer", createButtonRect.x + 10, createButtonRect.y + 5, 20, WHITE);

        // Draw the "Toggle Tree Type" button
        DrawRectangleRounded(toggleButtonRect, 0.2, 0, (Color){0x4F, 0x97, 0xA9, 0xFF});
        DrawText("Basculer le type ", toggleButtonRect.x + 10, toggleButtonRect.y + 5, 20, WHITE);

        // Draw the user input for the total number of nodes
        DrawText("Entrez le total de noeuds (1-31):", 10, screenHeight - 70, 20, WHITE);

        for (int i = 0; i < inputLength; ++i) {
            DrawText(TextFormat("%c", inputBuffer[i]), 270 + i * 10, screenHeight - 70, 20, WHITE);
        }


        if (inputLength > 0) {
            totalNodes = atoi(inputBuffer); // Convert string to integer

            // Check if the totalNodes value has changed
            if (totalNodes != previousTotalNodes) {
                nodesCreated = 0;
                root = NULL;  // Clear the existing tree
                elapsedTime = 0.0f; // Reset the timer
            }

            // Update previousTotalNodes
            previousTotalNodes = totalNodes;
        }

         // Update the tree and reset colors after a certain duration
        updateTree(root, GetFrameTime());

        // Draw the binary tree
        drawTree(root, screenWidth / 2,110, 240, 100);

        // Draw information about the tree
        DrawText(TextFormat("Noeuds crées: %d", nodesCreated), 10, 80, 20, WHITE);
        DrawText(TextFormat("Nombre de noeuds : %d", totalNodes), 10, 100, 20, WHITE);
        DrawText(TextFormat("Type de l'arbre : %s", isSortedTree ? "Triée" : "Non-Triée"), 10, 120, 20, WHITE);

        EndDrawing();

        // Handle user input for total nodes
        bool enterPressed = IsKeyPressed(KEY_ENTER);

        for (int key = KEY_ZERO; key <= KEY_NINE; ++key) {
            if (IsKeyPressed(key)) {
                if (inputLength < MAX_INPUT_LENGTH) {
                    inputBuffer[inputLength++] = '0' + (key - KEY_ZERO); // Convert key to digit character
                    inputBuffer[inputLength] = '\0'; // Null-terminate the string
                }
            }
        }

        if (enterPressed && inputLength > 0) {
            // Reset input buffer when Enter key is pressed and a number is entered
            inputLength = 0;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (inputLength > 0) {
                inputBuffer[--inputLength] = '\0'; // Remove the last entered digit
            }
        }

    }

    CloseWindow();

    return 0;
}


