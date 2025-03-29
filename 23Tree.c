#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR_PRINT 1

typedef enum traversalMethod{
    INORDER,
    PREORDER,
    POSTORDER,
    STRUCTURE,
    STRUCTURE_MINIMAL
} TraversalMethod;

typedef struct Node {
    int values[3]; // Stores up to 2 + 1 values
    struct Node *children[3]; // Stores up to 3 children (0 = left, 1 = middle, 2 = right)
    int numOfVals; // Number of values in the node (1 - 3)
    bool isLeaf;
    // short type; // -1 = root, 0 = normalNode, 1 = leaf
} Node;

Node* createNode(int value);
Node* addVal(Node* root, int value);
Node* rmVal(Node* root, int value);

Node* insert(Node* root, int value);

int findNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level, TraversalMethod traversalMethod);

int main() {
    Node* treeRoot = NULL;
    treeRoot = insert(treeRoot, 10);
    // treeRoot = insert(treeRoot, 20);
    // root = insert(root, 5);
    
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");
    
    // int value = 10;
    // printf("Search %d: %s\n", value, findNode(treeRoot, value) ? "Found" : "Not Found");
    
    return 0;
}

// creates node with one value
// isLeaf is true by defult
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->values[0] = value;
    newNode->numOfVals = 1;
    newNode->children[0] = NULL;
    newNode->children[1] = NULL;
    newNode->children[2] = NULL;
    newNode->isLeaf = true;
    return newNode;
}

Node* addVal(Node* root, int value){
    if(root->numOfVals > 2){
        #if ERROR_PRINT == 1:
        printf("addVal ERROR cannot add more - limit reached\n");
        #endif
        exit(1);
    }

    if (value < root->values[0]) { // add to beggining
        if(root->numOfVals == 2){
            root->values[2] = root->values[1];
        }
        root->values[1] = root->values[0];
        root->values[0] = value;
    } 
    else if ((root->numOfVals == 2) && (value < root->values[1])){ // add to midle
        root->values[2] = root->values[1];
        root->values[1] = value;
    }
    else {
        root->values[root->numOfVals-1] = value; // add to end
    }
    root->numOfVals++;
    return root;
}

Node* rmVal(Node* root, int value){
    if(root->numOfVals < 2){
        #if ERROR_PRINT == 1:
        printf("rmVal ERROR cannot remove - numOfVals would be 0\n");
        #endif
        exit(1);
    }

    if(value == root->values[0]){
        root->values[0] = root->values[1];
        root->values[1] = 0;
    }
    else if(value == root->values[1]){
        if(root->numOfVals == 3){
            root->values[1] = root->values[2];
            root->values[2] = 0;
        }
        else{
            root->values[1] = 0;
        }
    }
    else if(value == root->values[2]){
        root->values[2] = 0;
    }
    else{
        #if ERROR_PRINT == 1:
        printf("rmVal ERROR value to remove not found\n");
        #endif
        exit(1);
    }
    root->numOfVals--;
    return root;
}

Node* insert(Node* root, int value) {
    if (root == NULL) return createNode(value);

    if(root->isLeaf){
        root = addVal(root, value);
    }
    
    // // root has empty space
    // if (root->numOfVals < 2) {
    //     // Insert value into the correct position in the node
    //     if (value < root->values[0]) {
    //         root->values[1] = root->values[0];
    //         root->values[0] = value;
    //     } else {
    //         root->values[1] = value;
    //     }
    //     root->numOfVals++;
    // } else {
    //     // Tree split handling required (not implemented here)
    //     printf("Node is full. Splitting required!\n");
    // }
    // return root;
}

int findNode(Node* root, int value) {
    if (root == NULL) return 0;
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
        if (root == NULL) return;

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