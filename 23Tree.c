#include <stdio.h>
#include <stdlib.h>

typedef enum traversalMethod{
    INORDER,
    PREORDER,
    POSTORDER,
    STRUCTURE,
    STRUCTURE_MINIMAL
} TraversalMethod;

typedef struct Node {
    int values[2]; // Stores up to 2 values
    struct Node *children[3]; // Stores up to 3 children
    int numOfVals; // Number of values in the node (1 or 2)
} Node;

Node* createNode(int value);
Node* insert(Node* root, int value);

int findNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level, TraversalMethod traversalMethod);

int main() {
    Node* treeRoot = NULL;
    treeRoot = insert(treeRoot, 10);
    treeRoot = insert(treeRoot, 20);
    // root = insert(root, 5);
    
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");
    
    // int value = 10;
    // printf("Search %d: %s\n", value, findNode(treeRoot, value) ? "Found" : "Not Found");
    
    return 0;
}

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->values[0] = value;
    newNode->numOfVals = 1;
    newNode->children[0] = newNode->children[1] = newNode->children[2] = NULL;
    return newNode;
}

Node* insert(Node* root, int value) {
    if (!root) return createNode(value);
    
    if (root->numOfVals < 2) {
        // Insert value into the correct position in the node
        if (value < root->values[0]) {
            root->values[1] = root->values[0];
            root->values[0] = value;
        } else {
            root->values[1] = value;
        }
        root->numOfVals++;
    } else {
        // Tree split handling required (not implemented here)
        printf("Node is full. Splitting required!\n");
    }
    return root;
}

int findNode(Node* root, int value) {
    if (!root) return 0;
    if (root->values[0] == value || (root->numOfVals == 2 && root->values[1] == value)) return 1;
    
    if (value < root->values[0]) return findNode(root->children[0], value);
    else if (root->numOfVals == 1 || value < root->values[1]) return findNode(root->children[1], value);
    else return findNode(root->children[2], value);
}

void indent(int tabCount){
    for (size_t i = 0; i < tabCount; i++)
    {
        printf("|  ");
    }
}

void printTree(Node* root, int level, TraversalMethod traversalMethod) {
    if(traversalMethod == INORDER){
        if (!root) return;

        printTree(root->children[0], 0, INORDER);
        printf("%d ", root->values[0]);
        printTree(root->children[1], 0, INORDER);

        if (root->numOfVals == 2) {
            printf("%d ", root->values[1]);
            printTree(root->children[2], 0, INORDER);
        }
    }
    else if(traversalMethod == STRUCTURE){
        if(root == NULL){
            indent(level);
            printf("NULL\n");
            return;
        }
    
        indent(level);
        if(root->numOfVals == 2){
            printf("values = %d, %d\n", root->values[0], root->values[1]);
        }
        else{
            printf("values = %d\n", root->values[0]);
        }
        
    
        indent(level);
        printf("left\n");
        printTree(root->children[0], level+1, STRUCTURE);

        indent(level);
        printf("middle\n");
        printTree(root->children[1], level+1, STRUCTURE);
    
        indent(level);
        printf("right\n");
        printTree(root->children[2], level+1, STRUCTURE);
    }
}