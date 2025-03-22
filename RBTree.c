#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    int color; // 1-red, 0-black
    struct node* parent;
    struct node* right;
    struct node* left;
} Node;

Node* createNode(int value);

void indent(int tabCount);
void printTree(Node* root, int level);

int main()
{
    return 0;
}

Node* createNode(int value){
    Node* result = (Node*)malloc(sizeof(Node));
    if(result != NULL){
        result->parent = NULL;
        result->left = NULL;
        result->right = NULL;
        result->value = value;
    }
    return result;
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
