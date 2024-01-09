#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_INPUT_LENGTH 2 // nombre max des digits
#define CIRCLE_RADIUS 30;
char inputBuffer[MAX_INPUT_LENGTH + 1]; // +1 for null-terminator
int inputLength = 0;
int deletedNodeCounter = 0; // Une variable qui track les noeuds suprimes

// la structure de l'arbre binaire
typedef struct TreeNode
{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
    Vector2 position; // Position to represent the node in the window
} TreeNode;
// Fonction de creation de nouveaux noeuds
TreeNode *createNode(int data, float x, float y)
{
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->position.x = x;
    newNode->position.y = y;
    return newNode;
}

// procedure insertion de noeuds

void insertNode(TreeNode *root, int index)
{
    const int screenWidth = 1200;
    if (root == NULL)
        return;

    if (index > root->data)
    {
        if (root->right == NULL)
        {
            root->right = createNode(index, screenWidth / 2, 50);
        }
        else
        {
            insertNode(root->right, index);
        }
    }
    else
    {
        if (root->left == NULL)
        {
            root->left = createNode(index, screenWidth / 2, 50);
        }
        else
        {
            insertNode(root->left, index);
        }
    }
}
// procedure dessiner l'arbre
void drawTree(TreeNode *root, int x, int y, int hSpacing, int vSpacing)
{
    if (root != NULL)
    {
        int posX = x;
        int posY = y;

        if (root->left != NULL)
        {
            DrawLine(posX, posY, posX - hSpacing, posY + vSpacing, WHITE);
            drawTree(root->left, posX - hSpacing, posY + vSpacing, hSpacing / 2, vSpacing);
        }

        if (root->right != NULL)
        {
            DrawLine(posX, posY, posX + hSpacing, posY + vSpacing, WHITE);
            drawTree(root->right, posX + hSpacing, posY + vSpacing, hSpacing / 2, vSpacing);
        }

        DrawCircle(posX, posY, CIRCLE_RADIUS, BLUE);

        int textWidth = MeasureText(TextFormat("%d", root->data), 20);
        int textHeight = 20;
        int textX = posX - textWidth / 2;
        int textY = posY - textHeight / 2;
        DrawText(TextFormat("%d", root->data), textX, textY, 20, WHITE);
    }
}
// fonction de suppression
TreeNode *deleteNode(TreeNode *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else
    {
        if (root->left == NULL)
        {
            TreeNode *temp = root->right;
            free(root);
            deletedNodeCounter++;
            return temp;
        }
        else if (root->right == NULL)
        {
            TreeNode *temp = root->left;
            free(root);
            deletedNodeCounter++;
            return temp;
        }

        TreeNode *temp = minValueNode(root->right);

        root->data = temp->data;

        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}