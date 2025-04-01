#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // only used for testing

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
Node* rmVal(Node* root, int value, bool allowZero);
Node* splitRootNode(Node* root);
Node* splitChildNode(Node* root, int childIndex);

Node* insertNode(Node* root, int value);
Node* deleteNode(Node* root, int value);

bool findNode(Node* root, int value);

void indent(int tabCount);
void printTree(Node* root, int level, TraversalMethod traversalMethod);

Node* treeRoot = NULL; // base of the tree

int main() {
    int inVal = 0;
    unsigned int numsToAdd = 0;
    unsigned int numsToDel = 0;
    unsigned int numsToFind = 0;

    struct timespec start, end;
    double cpuTimeUsed;

    FILE *timesFile;
    timesFile = fopen("Testing/preciseTimes.txt", "w");
    if (timesFile == NULL) {
        printf("Error opening file!\n");
    }

    unsigned int repeats = 0;
    unsigned int totalInsertNums = 0;
    unsigned int totalDeleteNums = 0;
    unsigned int totalFindNums = 0;
    bool found = false;
    scanf("%u", &repeats);
    
    for (size_t r = 0; r < repeats; r++){
        // printf("---------- INSERTING: ----------\n");
        scanf("%u", &numsToAdd);
        totalInsertNums += numsToAdd;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToAdd; i++)
        {
            scanf("%d", &inVal);
            treeRoot = insertNode(treeRoot, inVal);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "insert: %u %f\n", totalInsertNums, cpuTimeUsed);
        // printTree(treeRoot, 0, INORDER);
    }
    for (size_t r = 0; r < repeats; r++)
    {
        // printf("---------- DELETING: ----------\n");
        // printf("\n");
        scanf("%u", &numsToDel);
        totalDeleteNums += numsToDel;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToDel; i++)
        {
            scanf("%d", &inVal);
            treeRoot = deleteNode(treeRoot, inVal);
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "delete: %u %f\n", totalDeleteNums, cpuTimeUsed);
        // printTree(treeRoot, 0, INORDER);
    }
    for (size_t r = 0; r < repeats; r++)
    {
        // printf("---------- FINDING: ----------\n");
        printf("\n");
        scanf("%u", &numsToFind);
        totalFindNums += numsToFind;
        clock_gettime(CLOCK_MONOTONIC, &start);  // Start time
        for (size_t i = 0; i < numsToFind; i++)
        {
            scanf("%d", &inVal);
            found = findNode(treeRoot, inVal);
            // printf("%d - %s\n", inVal, findNode(treeRoot, inVal) ? "true" : "false");
        }
        clock_gettime(CLOCK_MONOTONIC, &end);    // End time
        cpuTimeUsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
        fprintf(timesFile, "find: %u %f\n", totalFindNums, cpuTimeUsed);
    }
    fclose(timesFile);

    free(treeRoot);
    
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

Node* rmVal(Node* root, int value, bool allowZero){
    if((root->numOfVals < 2) && (allowZero == false)){
        #if ERROR_PRINT == 1
        printf("rmVal ERROR cannot remove - numOfVals would be 0\n");
        #endif
        exit(1);
    }
    
    if((root->numOfVals < 2) && (allowZero == true)){
        root->values[0] = 0;
    }
    else if(value == root->values[0]){
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
        fullNode = rmVal(fullNode, fullNode->values[1], false);
    
        Node* newNode = createNode(fullNode->values[1]);
        fullNode = rmVal(fullNode, fullNode->values[1], false);
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
        fullNode = rmVal(fullNode, fullNode->values[1], false);
    
        Node* newNode = NULL;
        if(childIndex == LEFT){
            newNode = createNode(fullNode->values[1]);
            fullNode = rmVal(fullNode, fullNode->values[1], false);
        }
        else{ // childIndex == RIGHT
            newNode = createNode(fullNode->values[0]);
            fullNode = rmVal(fullNode, fullNode->values[0], false);
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

Node* deleteNode(Node* root, int value){
    // value not found
    if(root == NULL){
        #if ERROR_PRINT == 1
        printf("delete ERROR value not found\n");
        #endif
        return root;
    }

    // value found and is the first one
    if(root->values[0] == value){
        // root doesn't have child to swap with
        if((root->children[LEFT] == NULL) || (root->children[LEFT]->numOfVals == 0)){
            root = rmVal(root, value, true);
            return root;
        }

        Node* swapNode = root;

        // go LRRR...
        root = root->children[LEFT];
        while((root->children[RIGHT] != NULL) && (root->children[RIGHT]->numOfVals != 0)){
            root = root->children[RIGHT];
        }

        // copy and remove val
        if(root->numOfVals == 2){
            swapNode->values[0] = root->values[1];
            root = rmVal(root, root->values[1], true);
        }
        else{ // root->numOfVals == 1
            swapNode->values[0] = root->values[0];
            root = rmVal(root, root->values[0], true);
        }
        return swapNode;
    }
    else if(root->numOfVals == 2 && root->values[1] == value){ // value found and is the second one
        // root doesn't have child to swap with
        if((root->children[RIGHT] == NULL) || (root->children[RIGHT]->numOfVals == 0)){
            root = rmVal(root, value, true);
            return root;
        }

        Node* swapNode = root;

        // go RLLL...
        root = root->children[RIGHT];
        while((root->children[LEFT] != NULL) && (root->children[LEFT]->numOfVals != 0)){
            root = root->children[LEFT];
        }

        // copy and remove val
        swapNode->values[0] = root->values[0];
        root = rmVal(root, root->values[0], true);

        return swapNode;
    }

    // go deeper
    if(value < root->values[0]){
        root->children[LEFT] = deleteNode(root->children[LEFT], value);
    }
    else if((root->numOfVals > 1) && (value < root->values[1])){
        root->children[MIDDLE] = deleteNode(root->children[MIDDLE], value);
    }
    else{ // value > root.values[1]
        root->children[RIGHT] = deleteNode(root->children[RIGHT], value);
    }

    return root;
}

bool findNode(Node* root, int value) {
    if (root == NULL) return false;
    if (root->values[0] == value || (root->numOfVals == 2 && root->values[1] == value)) return true;
    
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
        if((root == NULL) || (root->numOfVals == 0)){
            return;
        }

        if(root->isLeaf == true){
            printf("%d\n", root->values[0]);
            if(root->numOfVals == 2){
                printf("%d\n", root->values[1]);
            }
        }
        else if(root->children[MIDDLE] == NULL){
            printTree(root->children[LEFT], level, INORDER);
            printf("%d\n", root->values[0]);
            printTree(root->children[RIGHT], level, INORDER);
        }
        else{
            printTree(root->children[LEFT], level, INORDER);
            printf("%d\n", root->values[0]);
            printTree(root->children[MIDDLE], level, INORDER);
            printf("%d\n", root->values[1]);
            printTree(root->children[RIGHT], level, INORDER);
        }
    }
    else if(traversalMethod == STRUCTURE){
        if((root == NULL) || (root->numOfVals == 0)){
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