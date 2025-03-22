#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct node {
    int value;
    int color; // 1-red, 0-black
    struct node* parent;
    struct node* right;
    struct node* left;
} Node;

Node* createNode(int value);
Node* insertNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level);

int main()
{
    Node* root = createNode(5);

    root = insertNode(root, 3);
    root = insertNode(root, 8);
    root = insertNode(root, 4);
    root = insertNode(root, 1);
    // root = insertNode(root, 10);
    // root = insertNode(root, 6);

    printTree(root, 0);
    printf("\n");

    // root = deleteNode(root, 5);

    // printTree(root, 0);

    // int numToFind = 9;
    // printf("found %d = %s\n", numToFind, findNode(root, numToFind) ? "true" : "false");

    free(root);
    return 0;
}

Node* createNode(int value){
    Node* result = (Node*)malloc(sizeof(Node));
    if(result != NULL){
        result->parent = NULL;
        result->left = NULL;
        result->right = NULL;
        result->value = value;
        result->color = RED;
    }
    return result;
}

Node* insertNode(Node* root, int value){
    // add node
    if(root == NULL){
        root = createNode(value);
        return root;
    }

    // duplicate value == invalid
    if(value == root->value){
        printf("!!! error: No duplicate values allowed !!!\n");
        return root;
    }

    // go deeper
    if(value < root->value){
        root->left = insertNode((root->left), value);
        root->left->parent = root;
    }
    else{
        root->right = insertNode((root->right), value);
        root->right->parent = root;
    }

    // return without a change
    return root;
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
