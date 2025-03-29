#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR_PRINT 1

#define LEFT 0
#define MIDDLE 1
#define RIGHT 2
#define TEMP_CHILD 3

typedef enum traversalMethod{
    INORDER,
    PREORDER,
    POSTORDER,
    STRUCTURE,
    STRUCTURE_MINIMAL
} TraversalMethod;

typedef struct Node {
    int values[3]; // Stores up to 2 + 1 values
    struct Node *children[4]; // Stores up to 3 children (0 = left, 1 = middle, 2 = right, 3 = temp)
    int numOfVals; // Number of values in the node (1 - 3)
    bool isLeaf;
    // short type; // -1 = root, 0 = normalNode, 1 = leaf
} Node;

Node* createNode(int value);
Node* addVal(Node* root, int value);
Node* rmVal(Node* root, int value);
Node* splitRootNode(Node* root);
Node* splitChildNode(Node* root, int childIndex);

Node* insertNode(Node* root, int value);

int findNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level, TraversalMethod traversalMethod);

Node* treeRoot = NULL; // base of the tree

int main() {
    treeRoot = insertNode(treeRoot, 10);
    printf("----- 10 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 2);
    printf("----- 2 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 20);
    printf("----- 20 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 5);
    printf("----- 5 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 30);
    printf("----- 30 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 40);
    printf("----- 40 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 8);
    printf("----- 8 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 50);
    printf("----- 50 -----\n");
    printTree(treeRoot, 0, STRUCTURE);
    printf("\n");

    treeRoot = insertNode(treeRoot, 60);
    printf("----- 60 -----\n");
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
    newNode->values[1] = 0;
    newNode->values[2] = 0;
    newNode->numOfVals = 1;
    newNode->children[0] = NULL;
    newNode->children[1] = NULL;
    newNode->children[2] = NULL;
    newNode->children[3] = NULL;
    newNode->isLeaf = true;
    return newNode;
}

Node* addVal(Node* root, int value){
    if(root->numOfVals > 2){
        #if ERROR_PRINT == 1
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
        root->values[root->numOfVals] = value; // add to end
    }
    root->numOfVals++;
    return root;
}

Node* rmVal(Node* root, int value){
    if(root->numOfVals < 2){
        #if ERROR_PRINT == 1
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
        #if ERROR_PRINT == 1
        printf("rmVal ERROR value to remove not found\n");
        #endif
        exit(1);
    }
    root->numOfVals--;
    return root;
}

Node* splitRootNode(Node* root){
    Node* newRoot = createNode(root->values[1]);
    newRoot->isLeaf = false;

    newRoot->children[LEFT] = createNode(root->values[0]);
    newRoot->children[LEFT]->isLeaf = root->isLeaf;
    newRoot->children[RIGHT] = createNode(root->values[2]);
    newRoot->children[RIGHT]->isLeaf = root->isLeaf;

    if((root->children[TEMP_CHILD] != NULL) && (root->children[TEMP_CHILD]->values[0] < root->values[1])){
        newRoot->children[LEFT]->children[LEFT] = root->children[LEFT];
        newRoot->children[LEFT]->children[RIGHT] = root->children[TEMP_CHILD];
        newRoot->children[RIGHT]->children[LEFT] = root->children[MIDDLE];
        newRoot->children[RIGHT]->children[RIGHT] = root->children[RIGHT];
    }
    else if((root->children[TEMP_CHILD] != NULL) && (root->children[TEMP_CHILD]->values[0] < root->values[2])){
        newRoot->children[LEFT]->children[LEFT] = root->children[LEFT];
        newRoot->children[LEFT]->children[RIGHT] = root->children[MIDDLE];
        newRoot->children[RIGHT]->children[LEFT] = root->children[TEMP_CHILD];
        newRoot->children[RIGHT]->children[RIGHT] = root->children[RIGHT];
    }
    else{
        newRoot->children[LEFT]->children[LEFT] = root->children[LEFT];
        newRoot->children[LEFT]->children[RIGHT] = root->children[MIDDLE];
        newRoot->children[RIGHT]->children[LEFT] = root->children[RIGHT];
        newRoot->children[RIGHT]->children[RIGHT] = root->children[TEMP_CHILD];
    }

    free(root);
    root = NULL;
    return newRoot;
}

Node* splitChildNode(Node* root, int childIndex){
    Node* fullNode = root->children[childIndex];

    if(root->children[MIDDLE] != NULL){
        root = addVal(root, fullNode->values[1]);
        fullNode = rmVal(fullNode, fullNode->values[1]);
    
        Node* newNode = createNode(fullNode->values[1]);
        fullNode = rmVal(fullNode, fullNode->values[1]);
        // if(root->values[0] > fullNode->values[1]){
        //     newNode = createNode(fullNode->values[1]);
        //     fullNode = rmVal(fullNode, fullNode->values[1]);
        // }
        // else{
        //     newNode = createNode(fullNode->values[0]);
        //     fullNode = rmVal(fullNode, fullNode->values[0]);
        // }
        
        root->children[TEMP_CHILD] = newNode;
    
        return root;
    }
    else{ // middle child == NULL (means root has only one val)
        root = addVal(root, fullNode->values[1]);
        fullNode = rmVal(fullNode, fullNode->values[1]);
    
        Node* newNode = NULL;
        if(childIndex == LEFT){
            newNode = createNode(fullNode->values[1]);
            fullNode = rmVal(fullNode, fullNode->values[1]);
        }
        else{ // childIndex == RIGHT
            newNode = createNode(fullNode->values[0]);
            fullNode = rmVal(fullNode, fullNode->values[0]);
        }
        
        root->children[MIDDLE] = newNode;
    
        return root;
    }
}

Node* insertNode(Node* root, int value) {
    if(treeRoot == NULL){
        return createNode(value);
    }

    if((root->values[0] == value) || (root->values[1] == value)){ // duplicate val
        #if ERROR_PRINT == 1
        printf("insert ERROR duplicate value found\n");
        #endif
        return NULL;
    }

    if(root->isLeaf){
        root = addVal(root, value);
    }
    else{ // is not leaf
        // go deeper
        if(value < root->values[0]){
            root->children[LEFT] = insertNode(root->children[LEFT], value);
        }
        else if((root->numOfVals > 1) && (value < root->values[1])){
            root->children[MIDDLE] = insertNode(root->children[MIDDLE], value);
        }
        else{ // value > root.values[1]
            root->children[RIGHT] = insertNode(root->children[RIGHT], value);
        }
    }

    //child is full
    if((root->children[LEFT] != NULL) && (root->children[LEFT]->numOfVals > 2)){
        root = splitChildNode(root, LEFT);
    }
    else if((root->children[MIDDLE] != NULL) && (root->children[MIDDLE]->numOfVals > 2)){
        root = splitChildNode(root, MIDDLE);
    }
    else if((root->children[RIGHT] != NULL) && (root->children[RIGHT]->numOfVals > 2)){
        root = splitChildNode(root, RIGHT);
    }

    //treeRoot is full
    if((root == treeRoot) && (root->numOfVals > 2)){
        root = splitRootNode(root);
    }

    return root;
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
        if(root->numOfVals == 1){
            printf("values = %d%s\n", root->values[0], root->isLeaf ? " - leaf" : "");
        }
        else if(root->numOfVals == 2){
            printf("values = %d, %d%s\n", root->values[0], root->values[1], root->isLeaf ? " - leaf" : "");
        }
        else{
            #if ERROR_PRINT == 1
            printf("print ERROR 3 values in node");
            exit(1);
            #endif
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