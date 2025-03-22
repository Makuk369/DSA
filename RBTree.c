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

Node* insertFix(Node* root);

Node* rotateLeft(Node* root);
Node* rotateRight(Node* root);

void indent(int tabCount);
void printTree(Node* root, int level);

int main()
{
    Node* treeRoot = NULL;
    treeRoot = insertNode(treeRoot, 10);

    // lava strana
    treeRoot = insertNode(treeRoot, 5);
    // treeRoot = insertNode(treeRoot, 1);
    // treeRoot = insertNode(treeRoot, 7);
    // treeRoot = insertNode(treeRoot, 6);
    // treeRoot = insertNode(treeRoot, 8);

    // prava strana
    treeRoot = insertNode(treeRoot, 20);
    // treeRoot = insertNode(treeRoot, 25);
    // treeRoot = insertNode(treeRoot, 15);
    // treeRoot = insertNode(treeRoot, 12);
    // treeRoot = insertNode(treeRoot, 17);

    printTree(treeRoot, 0);
    printf("\n");

    // treeRoot->left = rotateLeft(treeRoot->left);
    // treeRoot = rotateRight(treeRoot);
    // treeRoot = deleteNode(treeRoot, 5);

    // printTree(treeRoot, 0);

    // int numToFind = 9;
    // printf("found %d = %s\n", numToFind, findNode(treeRoot, numToFind) ? "true" : "false");

    free(treeRoot);
    return 0;
}

// returns a new node, with black color
Node* createNode(int value){
    Node* result = (Node*)malloc(sizeof(Node));
    if(result != NULL){
        result->parent = NULL;
        result->left = NULL;
        result->right = NULL;
        result->value = value;
        result->color = BLACK;
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
        root->left = insertFix(root->left);
    }
    else{
        root->right = insertNode((root->right), value);
        root->right->parent = root;
        root->right = insertFix(root->right);
    }

    // return without a change
    return root;
}

Node* insertFix(Node* root){
    // CASE 1 = root has a black parent
    if(root->parent->color == BLACK){
        root->color = RED;
        // printf("%d - case 1\n", root->value);
        return root;
    }

    // CASE 5 = no fix needed
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

Node* rotateRight(Node* root){
    Node* rotNode = root->left;

    root->left = rotNode->right;

    if(rotNode->right != NULL){
        rotNode->right->parent = root;
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

    rotNode->right = root;

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
    printf("value = %d - %s\n", root->value, (root->color == RED) ? "RED" : "BLACK");

    indent(level);
    printf("left\n");
    printTree(root->left, level+1);

    indent(level);
    printf("right\n");
    printTree(root->right, level+1);
}
