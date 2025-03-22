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

Node* rotateLeft(Node* root);

void indent(int tabCount);
void printTree(Node* root, int level);

int main()
{
    Node* treeRoot = createNode(10);

    treeRoot = insertNode(treeRoot, 5);
    treeRoot = insertNode(treeRoot, 1);
    treeRoot = insertNode(treeRoot, 7);
    treeRoot = insertNode(treeRoot, 6);
    treeRoot = insertNode(treeRoot, 8);
    // treeRoot = insertNode(treeRoot, 6);

    printTree(treeRoot, 0);
    printf("\n");

    treeRoot->left = rotateLeft(treeRoot->left);
    // treeRoot = deleteNode(treeRoot, 5);

    printTree(treeRoot, 0);

    // int numToFind = 9;
    // printf("found %d = %s\n", numToFind, findNode(treeRoot, numToFind) ? "true" : "false");

    free(treeRoot);
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

Node* rotateLeft(Node* root){
    Node* rotNode = root->right;

    root->right = rotNode->left;

    if(rotNode->left != NULL){
        rotNode->left->parent = root;
    }

    rotNode->parent = root->parent; // moze bit aj null

    if(root->parent != NULL){
        if((root->parent->left != NULL) && (root->parent->left->value == root->value)){
            root->parent->left = rotNode;
        }
        else if((root->parent->right != NULL) && (root->parent->right->value == root->value)){
            root->parent->right = rotNode;
        }
    }

    rotNode->left = root;

    root->parent = rotNode;

    return rotNode;
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
