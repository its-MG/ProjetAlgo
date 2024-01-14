#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define CIRCLE_RADIUS 35
#define HORIZONTAL_SPACE 250
#define VERTICAL_SPACE 150
#define MAX_INPUT_LENGTH 2 // l'utilisateur peut entrer un nombre a deux chiffres max

char inputBuffer[MAX_INPUT_LENGTH + 1]; // pour l'operation de lecture au clavier
int inputLength = 0;

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

TreeNode *createNode(int data) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

bool searchNode(TreeNode *root, int value) {
     bool foundLeft=false;
     bool foundRight =false;

    if (root == NULL) return false;
    if (root->data == value) return true;

    if(value<root->data){
        foundLeft = searchNode(root->left, value); // Rechercher dans le sous-arbre gauche
    }else{
        foundRight = searchNode(root->right, value);// Rechercher dans le sous-arbre droit 
    }

    return(foundLeft || foundRight);
}

void insertNode(TreeNode *root, int index) {
    if (root == NULL) return;

    if (index > root->data) {
        if (root->right == NULL) {
            root->right = createNode(index);
        } else {
            insertNode(root->right, index);
        }
    } else {
        if (root->left == NULL) {
            root->left = createNode(index);
        } else {
            insertNode(root->left, index);
        }
    }
}

void drawTree(TreeNode *root, int x, int y, int hSpacing, int vSpacing, int searchValue, bool *foundValue) {
    if (root != NULL) {
        int posX = x;
        int posY = y;

        if (root->left != NULL) {
            DrawLine(posX, posY, posX - hSpacing, posY + vSpacing, (Color){ 0x98, 0xC1, 0xD9, 255 });
            drawTree(root->left, posX - hSpacing, posY + vSpacing, hSpacing / 2, vSpacing, searchValue, foundValue);
        }

        if (root->right != NULL) {
        DrawLine(posX, posY, posX + hSpacing, posY + vSpacing,(Color){ 0x98, 0xC1, 0xD9, 255 });
        drawTree(root->right, posX + hSpacing, posY + vSpacing, hSpacing / 2, vSpacing, searchValue, foundValue);
        }

        bool currentNodeFound = (root->data == searchValue);

        if (currentNodeFound) {
            *foundValue = true;
          Color foundColor = (Color){100, 180, 100, 255}; // couleur vert si la valeur est trouvée
            DrawCircle(posX, posY, CIRCLE_RADIUS, foundColor);
        } else {
            Color customColor ={ 0x1F, 0x5E, 0x91, 255 }; // couleur bleu pour les autres noeuds
            DrawCircle(posX, posY, CIRCLE_RADIUS, customColor);
        }

        int textWidth = MeasureText(TextFormat("%d", root->data), 20);
        int textHeight = 20;
        int textX = posX - textWidth / 2;
        int textY = posY - textHeight / 2;
        DrawText(TextFormat("%d", root->data), textX, textY, 20,WHITE);
    }
}

int main() {

    const int screenWidth = 1000;
    const int screenHeight = 800;

    int inputNumber=-1; //Initialisé -1 au cas où aucun valeur n'est entrée par l'utilisateur
    bool showMessage = true; //controler de la visibilité du msg (Nous voulons masquer le message une fois la recherche lancée)
    bool showInfo = false; // controler si la visibilité du msg explication du fonctionement 
    Color foundGreen = (Color){100, 180, 100, 255};

    InitWindow(screenWidth, screenHeight, "Recherche dans un arbre Binaire");

    //message explicatif au utilisateurs
    const char *line1 = "Lorsque vous cliquez sur le button, un algorithme de recherche sera lancé pour";
    const char *line2 = "chercher la valeur que vous avez sélectionné.";
    // calcul pour centrer le rectangle message
    int rectangleWidth = screenWidth - 120;
    int rectangleHeight = 90;
    int rectangleX = (screenWidth - rectangleWidth) / 2;
    int rectangleY = screenHeight - 200;
    Rectangle messageRect = { rectangleX, rectangleY, rectangleWidth, rectangleHeight };

    SetTargetFPS(60);

     // ces numéros ne sont pas choisis aléatoirement c'est pour crée un arbre binaire équlibré
     TreeNode *root = createNode(16); // la racine
     insertNode(root,8);
     insertNode(root,24);
     insertNode(root,4);
     insertNode(root,12);
     insertNode(root,20);
     insertNode(root,28);
     insertNode(root,18);
     insertNode(root,22);
     insertNode(root,26);
     insertNode(root,30);
     insertNode(root,2);
     insertNode(root,6);
     insertNode(root,10);
     insertNode(root,14);

    Color backgroundColor = (Color){ 15, 16, 53, 255 };  // couleur arrière-plan

    Color customColor1 = { 0x29, 0x32, 0x41, 255 };
    Color customColor2 = { 0x98, 0xC1, 0xD9, 255 };
    Color textColor = WHITE;
    Rectangle buttonRect = {screenWidth / 2 - 100, 700, 200, 50}; // dimentions bouton recherche
    bool buttonHovered = false; // pour savoir si la souris est sur le bouton
    int searchValue = 0;
    bool found = false;  //found est un booléen qui récupére si la valeur existe dans l'arbre ou pas 
    bool searching = false; //recherche effectuée

    while (!WindowShouldClose()) {

    //Traitement de la saisie 
    // saisie du nbr sous forme de chaîne de caractères
    DrawText("Entrez un entier :", 20, 20, 20,WHITE);
     for (int i = 0; i < inputLength; ++i) {
        DrawText(TextFormat("%c", inputBuffer[i]), 20 + i * 20, 50, 20, WHITE);
         }
        if (inputLength > 0) {
            inputNumber = atoi(inputBuffer); // convertir la chaine en entier 
        }

     buttonHovered = CheckCollisionPointRec(GetMousePosition(), buttonRect);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttonHovered) {
            searchValue = inputNumber;
            found = searchNode(root, searchValue);
            searching = true;
            showMessage = false; //pour cacher le msg d'explication aprés avoir clicker le button rechercher
        }

        if (IsKeyPressed(KEY_I)) {
            showInfo = !showInfo; //pour faire appaitre et disparaitre le msg fonctionnement algo
        }
    BeginDrawing();

    ClearBackground(backgroundColor);

        bool foundValue = false; // pour avoir la position de la valeur trouvée
        drawTree(root, screenWidth / 2,80, HORIZONTAL_SPACE, VERTICAL_SPACE, searchValue, &foundValue); //afficher l'arbre

        if (buttonHovered) {
            textColor = customColor1;
        }else{
            textColor = WHITE;
        }

    DrawRectangleRounded(buttonRect, 0.2, 0, buttonHovered ? customColor2 : customColor1); //effet changement de couleur lors du survol avec la souris 
    DrawText("Rechercher", buttonRect.x +40, buttonRect.y + 15, 20, textColor);

        if (searching) {
            Color displayColor = found ? foundGreen:(Color){ 0xF2, 0x4C, 0x00, 255 };
            const char* message = TextFormat("Valeur %d %strouvée", searchValue,(found ? "" : "non "));//phrase -> val trouvée ou non
            
            // Mesure de la largeur du text pour le centrer horizontalement
            int textWidth = MeasureText(message,25);
            // Calcul coordonée x
            int textX = (screenWidth - textWidth) / 2;
            DrawText(message, textX, rectangleY + 20,25, displayColor);
        }
        
       if (showMessage) {
        DrawRectangleRounded(messageRect, 0.2, 0, (Color) { 0x29, 0x32, 0x41, 200 });
        // Afficher le message
        DrawText(line1, messageRect.x + 20, messageRect.y + 20, 20, customColor2);
        DrawText(line2, messageRect.x + 20, messageRect.y + 45, 20, customColor2);
        }

       // Dessiner le texte en haut à droite
        DrawText("Appuyez sur i pour plus d'infos",screenWidth - MeasureText("Appuyez sur i pour plus d'infos", 20) -20,20,20, customColor2);

        if (showInfo) {
            //message d'explication du fonctionement
            DrawRectangleRounded((Rectangle){ 500,0,490,600}, 0.1, 0,(Color){ 15, 16, 53, 230});
            
            DrawText("Fonctionnement de l'algorithme",screenWidth/2 +80,20, 24,customColor2);
            DrawText("if this == null",screenWidth/2 +90,80, 22,WHITE);
            DrawText("return null",screenWidth/2 +90,130, 22,WHITE);
            DrawText("else if this key == search value",screenWidth/2 +90,180, 22,WHITE);
            DrawText(" return this",screenWidth/2 +90, 230, 22,WHITE);
            DrawText("else if this key < search value",screenWidth/2 +90, 280, 22,WHITE);
            DrawText("search right",screenWidth/2 +90, 330, 22,WHITE);
            DrawText("else search left",screenWidth/2 +90, 380, 22,WHITE);
            DrawText("Pour fermer, appuyez sur i",screenWidth/2 +105,460, 22,customColor2);
        }

    EndDrawing();
    
    //Traitement pour la saisie 
        for (int key = KEY_ZERO; key <= KEY_NINE; ++key) {
            if (IsKeyPressed(key)) {
                if (inputLength < MAX_INPUT_LENGTH) {
                    inputBuffer[inputLength++] = '0' + (key - KEY_ZERO); // Convertir la touche en caractère numérique
                    inputBuffer[inputLength] = '\0'; //Insèrer un caractère nul pour terminer la chaîne
                }
            }
        }

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && buttonHovered) && inputLength > 0) {
            //Efface le nombre de l'écran lorsque 'Entrée' est pressée
            inputLength = 0;
            memset(inputBuffer, 0, sizeof(inputBuffer));
        }

        if (IsKeyPressed(KEY_BACKSPACE)) { // vérifie si la touche "Retour Arrière" est enfoncée
            if (inputLength > 0) { //Si la longueur de l'entrée est supérieure à zéro
                inputBuffer[--inputLength] = '\0'; //supprimer le dernier caractère
            }
        }
}
    CloseWindow();
    return 0;
}