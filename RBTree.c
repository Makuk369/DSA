#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 0 = NO case print
// 1 = YES print cases used
#define CASE_PRINT 0

#define RED 1
#define BLACK 0

// root pos relative to uncle
typedef enum uncleRelation{
    AWAY_LEFT,
    TOWARDS_LEFT,
    AWAY_RIGHT,
    TOWARDS_RIGHT
} UncleRelation;

typedef enum traversalMethod{
    INORDER,
    PREORDER,
    POSTORDER,
    STRUCTURE,
    STRUCTURE_MINIMAL
} TraversalMethod;

typedef struct node {
    int value;
    int color; // 1-red, 0-black
    struct node* parent;
    struct node* right;
    struct node* left;
} Node;

Node* createNode(int value);
Node* insertNode(Node* root, int value);
Node* deleteNode(Node* root, int value);
Node* treeFixup(Node* root);

bool findNode(Node* root, int value);

Node* rotateLeft(Node* root);
Node* rotateRight(Node* root);

void indent(int tabCount);
void printTree(Node* root, int level, TraversalMethod traversalMethod);

int main()
{
    Node* treeRoot = NULL;
    
    // int inVal = 0;
    // unsigned int numsToAdd = 0;
    // scanf("%u", &numsToAdd);
    // for (size_t i = 0; i < numsToAdd; i++)
    // {
    //     scanf("%d", &inVal);
    //     treeRoot = insertNode(treeRoot, inVal);
    // }

    // printTree(treeRoot, 0, INORDER);
    // printf("---------- FINDING: ----------\n");

    // unsigned int numsToFind = 0;
    // scanf("%u", &numsToFind);
    // for (size_t i = 0; i < numsToFind; i++)
    // {
    //     scanf("%d", &inVal);
    //     printf("%d - %s\n", inVal, findNode(treeRoot, inVal) ? "true" : "false");
    // }



    printf("\n ---------- 10 ---------- \n");
    treeRoot = insertNode(treeRoot, 10);
    printTree(treeRoot, 0, STRUCTURE);

    printf("\n ---------- 5 ---------- \n");
    treeRoot = insertNode(treeRoot, 5);
    printTree(treeRoot, 0, STRUCTURE);

    printf("\n ---------- 20 ---------- \n");
    treeRoot = insertNode(treeRoot, 20);
    printTree(treeRoot, 0, STRUCTURE);

    printf("\n ---------- DEL 5 ---------- \n");
    treeRoot = deleteNode(treeRoot, 5);
    printTree(treeRoot, 0, STRUCTURE);

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
    static Node* newNode = NULL;

    // add node
    if(root == NULL){
        root = createNode(value);
        newNode = root;
        return root;
    }

    // duplicate value == invalid
    if(value == root->value){
        printf("!!! error: No duplicate values allowed !!!\n");
        return root;
    }

    // go deeper
    if(value < root->value){
        root->left = insertNode(root->left, value);
        root->left->parent = root;
    }
    else{
        root->right = insertNode(root->right, value);
        root->right->parent = root;
    }

    // only call on treeRoot
    if(root->parent == NULL){
        newNode = treeFixup(newNode);

        while (root->parent != NULL)
        {
            root = root->parent;
        }
        return root;
    }

    // return without a change
    return root;
}

Node* deleteNode(Node* root, int value){
    // node found
    if(value == root->value){
        // CASE 1 = root does not have children
        if((root->left == NULL) && (root->right == NULL)){
            free(root);
            root = NULL;
            return root;
        }
    }

    // didnt find = go deeper
    if(value < root->value){
        root->left = deleteNode(root->left, value);
    }
    else{
        root->right = deleteNode(root->right, value);
    }

    return root;
}

Node* treeFixup(Node* root){
    // CASE 0 = root is treeRoot and its red
    if(root->parent == NULL){
        if(root->color == RED){
            root->color = BLACK;
            #if CASE_PRINT == 1
            printf("%d - case 0\n", root->value);
            #endif
            return root;
        }
        else{
            return root;
        }
    }
    
    // CASE 1 = root has a black parent
    if(root->parent->color == BLACK){
        root->color = RED;
        #if CASE_PRINT == 1
        printf("%d - case 1\n", root->value);
        #endif
        return root;
    }

    // uncle = second child of grandparent
    // has grandparent
    if(root->parent->parent != NULL){
        Node* grandParent = root->parent->parent;
        Node* uncle = NULL;
        UncleRelation uncleRel = AWAY_LEFT;

        // left is parent, then right is uncle
        if((grandParent->left != NULL) && (grandParent->left->value == root->parent->value)){
            uncle = grandParent->right; // can be NULL

            if((root->parent->left != NULL) && (root->value == root->parent->left->value)){
                uncleRel = AWAY_LEFT;
            }
            else{
                uncleRel = TOWARDS_LEFT;
            }
        }
        else{
            uncle = grandParent->left; // can be NULL

            if((root->parent->right != NULL) && (root->value == root->parent->right->value)){
                uncleRel = AWAY_RIGHT;
            }
            else{
                uncleRel = TOWARDS_RIGHT;
            }
        }

        // CASE 2 = root has red parent and red uncle
        if((root->parent->color == RED) && ((uncle != NULL) && (uncle->color == RED))){ // NULL uncle == black uncle
            root->parent->color = BLACK;
            uncle->color = BLACK;
            grandParent->color = RED;
            root->color = RED;
            #if CASE_PRINT == 1
            printf("%d - case 2\n", root->value);
            #endif
            grandParent = treeFixup(grandParent);
            return root;
        }

        // CASE 3 = root has red parent and black uncle
        if((root->parent->color == RED) && ((uncle == NULL) || (uncle->color == BLACK))){ // NULL uncle == black uncle
            switch (uncleRel){
                case AWAY_LEFT:
                    root->parent = rotateRight(root->parent->parent); // grandparent je teraz parent
                    root->color = RED;
                    root->parent->color = BLACK;
                    root->parent->right->color = RED;
                    #if CASE_PRINT == 1
                    printf("%d - case 3 (AWAY_LEFT)\n", root->value);
                    #endif
                    return root;
                    break;
                    
                case TOWARDS_LEFT:
                    root = rotateLeft(root->parent);
                    root = rotateRight(root->parent);
                    root->color = BLACK;
                    root->right->color = RED;
                    #if CASE_PRINT == 1
                    printf("%d - case 3 (TOWARDS_LEFT)\n", root->value);
                    #endif
                    break;

                case AWAY_RIGHT:
                    root->parent = rotateLeft(root->parent->parent); // grandparent je teraz parent
                    root->color = RED;
                    root->parent->color = BLACK;
                    root->parent->left->color = RED;
                    #if CASE_PRINT == 1
                    printf("%d - case 3 (AWAY_RIGHT)\n", root->value);
                    #endif
                    return root;
                    break;

                case TOWARDS_RIGHT:
                    root = rotateRight(root->parent);
                    root = rotateLeft(root->parent);
                    root->color = BLACK;
                    root->left->color = RED;
                    #if CASE_PRINT == 1
                    printf("%d - case 3 (TOWARDS_RIGHT)\n", root->value);
                    #endif
                    break;
                
                default:
                    break;
            }
        }
    }
    else{ // parent is root
        // CASE 4 = treeRoot is red 
        if(root->parent->color == RED){
            root->parent->color = BLACK;
            #if CASE_PRINT == 1
            printf("%d - case 4\n", root->value);
            #endif
            return root;
        }
    }

    // CASE 5 = no fix needed
    return root;
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

void printTree(Node* root, int level, TraversalMethod traversalMethod){
    if(traversalMethod == INORDER){
        if(root != NULL){
            printTree(root->left, level+1, INORDER);
            printf("%d\n", root->value);
            printTree(root->right, level+1, INORDER);
        }
    }
    else if(traversalMethod == PREORDER){
        if(root != NULL){
            printf("%d\n", root->value);
            printTree(root->left, level+1, PREORDER);
            printTree(root->right, level+1, PREORDER);
        }
    }
    else if(traversalMethod == POSTORDER){
        if(root != NULL){
            printTree(root->left, level+1, POSTORDER);
            printTree(root->right, level+1, POSTORDER);
            printf("%d\n", root->value);
        }
    }
    else if(traversalMethod == STRUCTURE){
        if(root == NULL){
            indent(level);
            printf("NULL\n");
            return;
        }
    
        indent(level);
        printf("value = %d - %s\n", root->value, (root->color == RED) ? "RED" : "BLACK");
    
        indent(level);
        printf("left\n");
        printTree(root->left, level+1, STRUCTURE);
    
        indent(level);
        printf("right\n");
        printTree(root->right, level+1, STRUCTURE);
    }
    else if(traversalMethod == STRUCTURE_MINIMAL){
        if(root != NULL){
            indent(level);
            printf("%d-%s\n", root->value, (root->color == RED) ? "R" : "B");
            printTree(root->left, level+1, STRUCTURE_MINIMAL);
            printTree(root->right, level+1, STRUCTURE_MINIMAL);
        }
    }
}
