#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct node{
    int value;
    struct node* left;
    struct node* right;
} Node;

Node* createNode(int value);

bool insertNode(Node** pRoot, int value);
bool deleteNode(Node** pRoot, int value);
bool findNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level);

int main(){
    Node* root = NULL;

    insertNode(&root, 5);
    insertNode(&root, 3);
    insertNode(&root, 8);
    insertNode(&root, 4);
    insertNode(&root, 1);
    // insertNode(&root, 10);
    // insertNode(&root, 6);

    printTree(root, 0);
    printf("\n");

    deleteNode(&root, 8);

    printTree(root, 0);

    // int numToFind = 9;
    // printf("found %d = %s\n", numToFind, findNode(root, numToFind) ? "true" : "false");

    free(root);
}

Node* createNode(int value){
    Node* result = malloc(sizeof(Node));
    if(result != NULL){
        result->left = NULL;
        result->right = NULL;
        result->value = value;
    }
    return result;
}

bool insertNode(Node** pRoot, int value){
    // add node
    if((*pRoot) == NULL){
        (*pRoot) = createNode(value);
        return true;
    }

    // duplicate value == invalid
    if(value == (*pRoot)->value){
        printf("!!! error: No duplicate values allowed !!!\n");
        return false;
    }

    // go deeper
    if(value < (*pRoot)->value){
        return insertNode(&((*pRoot)->left), value);
    }
    else{
        return insertNode(&((*pRoot)->right), value);
    }
}

bool deleteNode(Node** pRoot, int value){
    // value not found
    if((*pRoot) == NULL){
        return false;
    }

    // value found
    if((*pRoot)->value == value){

        // no child
        if((*pRoot)->left == NULL && (*pRoot)->right == NULL){
            (*pRoot) = NULL;
            return true;
        }

        // has child
        if((*pRoot)->left != NULL){ // left
            Node* swapNode = (*pRoot);
            
            // go LRRR...
            pRoot = &((*pRoot)->left);
            while((*pRoot)->right != NULL){
                pRoot = &((*pRoot)->right);
            }

            // swap value, repeat on new value
            swapNode->value = (*pRoot)->value;
            return deleteNode(&(*pRoot), (*pRoot)->value);
        }
        else{ // right
            Node* swapNode = (*pRoot);

            // go RLLL...
            pRoot = &((*pRoot)->right);
            while((*pRoot)->left != NULL){
                pRoot = &((*pRoot)->left);
            }

            // swap value, repeat on new value
            swapNode->value = (*pRoot)->value;
            return deleteNode(&(*pRoot), (*pRoot)->value);
        }
    }

    // search deeper
    if(value < (*pRoot)->value){
        return deleteNode(&((*pRoot)->left), value);
    }
    else{
        return deleteNode(&((*pRoot)->right), value);
    }
}

bool findNode(Node* root, int value){
    // value not found
    if(root == NULL){
        return false;
    }

    // value found
    if(root->value == value){
        return true;
    }

    // search deeper
    if(value < root->value){
        return findNode(root->left, value);
    }
    else{
        return findNode(root->right, value);
    }
}

void indent(int tabCount){
    for (size_t i = 0; i < tabCount; i++)
    {
        printf("|  ");
    }
}

void printTree(Node* root, int level){
    if(root == NULL){
        indent(level);
        printf("NULL\n");
        return;
    }

    indent(level);
    printf("value = %d\n", root->value);

    indent(level);
    printf("left\n");
    printTree(root->left, level+1);

    indent(level);
    printf("right\n");
    printTree(root->right, level+1);
}