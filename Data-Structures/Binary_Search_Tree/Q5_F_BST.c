//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section F - Binary Search Trees Questions
Purpose: Implementing the required functions for Question 5
                 Implementing 'remove node' operation for BST*/
//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _bstnode {
    int item;
    struct _bstnode *left;
    struct _bstnode *right;
} BSTNode; // You should not change the definition of BSTNode

typedef struct _stackNode {
    BSTNode *data;
    struct _stackNode *next;
} StackNode; // You should not change the definition of StackNode

typedef struct _stack {
    StackNode *top;
} Stack; // You should not change the definition of Stack

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
void postOrderIterativeS2(BSTNode *root);

void insertBSTNode(BSTNode **node, int value);

void push(Stack *stack, BSTNode *node);
BSTNode *pop(Stack *s);
BSTNode *peek(Stack *s);
int isEmpty(Stack *s);
void removeAll(BSTNode **node);
BSTNode *removeNodeFromTree(BSTNode *root, int value);

///////////////////////////// main() /////////////////////////////////////////////

int main() {
    int c, i;
    c = 1;

    // Initialize the Binary Search Tree as an empty Binary Search Tree
    BSTNode *root;
    root = NULL;

    printf("1: Insert an integer into the binary search tree;\n");
    printf("2: Print the post-order traversal of the binary search tree;\n");
    printf("3 : 삭제할 정수 입력\n");
    printf("0: Quit;\n");

    while (c != 0) {
        printf("Please input your choice(1/2/0): ");
        scanf("%d", &c);

        switch (c) {
        case 1:
            printf("Input an integer that you want to insert into the Binary Search Tree: ");
            scanf("%d", &i);
            insertBSTNode(&root, i);
            break;
        case 2:
            printf("The resulting post-order traversal of the binary search tree is: ");
            postOrderIterativeS2(root); // You need to code this function
            printf("\n");
            break;
        case 3:
            printf("삭제할 정수를 입력하세요: ");
            scanf("%d", &i);
            root = removeNodeFromTree(root, i); // 여기서 삭제
            postOrderIterativeS2(root);
            break;
        case 0:
            removeAll(&root);
            break;
        default:
            printf("Choice unknown;\n");
            break;
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

// 스택 2개를 사용하여 후위 순회를 구현
void postOrderIterativeS2(BSTNode *root) {
    if (root == NULL) {
        return;
    }

    Stack s1;
    s1.top = NULL;
    Stack s2;
    s2.top = NULL;

    // 1) 루트부터 시작
    push(&s1, root);

    // 2) S1을 비울 때까지: S1에서 pop -> S2에 push, 자식은 S1에 push
    while (!isEmpty(&s1)) {
        BSTNode *node = pop(&s1);
        push(&s2, node);

        if (node->left) {
            push(&s1, node->left);
        }
        if (node->right) {
            push(&s1, node->right);
        }
    }

    // 3) S2를 pop하며 출력
    while (!isEmpty(&s2)) {
        BSTNode *node = pop(&s2);
        printf("%d ", node->item);
    }
}

/* Given a binary search tree and a key, this function
   deletes the key and returns the new root. Make recursive function. */
// value가 현재 item보다 작다면
// 왼쪽으로 재귀
// 아니라면 오른쪽으로 재귀
// 삭제 대상을 찾았을 때 자식을 검사
BSTNode *removeNodeFromTree(BSTNode *root, int value) {
    if (root == NULL)
        return NULL;

    if (value < root->item) {
        root->left = removeNodeFromTree(root->left, value);
    } else if (value > root->item) {
        root->right = removeNodeFromTree(root->right, value);
    } else {
        // 삭제 대상 발견
        if (root->left == NULL && root->right == NULL) {
            // 자식 0개 (리프)
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            // 자식 1개 (오른쪽만)
            BSTNode *r = root->right;
            free(root);
            return r;
        } else if (root->right == NULL) {
            // 자식 1개 (왼쪽만)
            BSTNode *l = root->left;
            free(root);
            return l;
        } else {
            // 자식 2개 : 오른쪽 서브트리의 최소값 찾기 (후계자)
            BSTNode *succ = root->right;
            while (succ->left != NULL) {
                succ = succ->left;
            }
            // 현재 노드 값을 successor 값으로 교체
            root->item = succ->item;
            // 교체한 값(succ->item)을 오른쪽 서브트리에서 제거
            root->right = removeNodeFromTree(root->right, succ->item);
        }
    }
    return root;
}
///////////////////////////////////////////////////////////////////////////////

void insertBSTNode(BSTNode **node, int value) {
    if (*node == NULL) {
        *node = malloc(sizeof(BSTNode));

        if (*node != NULL) {
            (*node)->item = value;
            (*node)->left = NULL;
            (*node)->right = NULL;
        }
    } else {
        if (value < (*node)->item) {
            insertBSTNode(&((*node)->left), value);
        } else if (value > (*node)->item) {
            insertBSTNode(&((*node)->right), value);
        } else
            return;
    }
}

//////////////////////////////////////////////////////////////////////////////////

void push(Stack *stack, BSTNode *node) {
    StackNode *temp;

    temp = malloc(sizeof(StackNode));

    if (temp == NULL)
        return;
    temp->data = node;

    if (stack->top == NULL) {
        stack->top = temp;
        temp->next = NULL;
    } else {
        temp->next = stack->top;
        stack->top = temp;
    }
}

BSTNode *pop(Stack *s) {
    StackNode *temp, *t;
    BSTNode *ptr;
    ptr = NULL;

    t = s->top;
    if (t != NULL) {
        temp = t->next;
        ptr = t->data;

        s->top = temp;
        free(t);
        t = NULL;
    }

    return ptr;
}

BSTNode *peek(Stack *s) {
    StackNode *temp;
    temp = s->top;
    if (temp != NULL)
        return temp->data;
    else
        return NULL;
}

int isEmpty(Stack *s) {
    if (s->top == NULL)
        return 1;
    else
        return 0;
}

void removeAll(BSTNode **node) {
    if (*node != NULL) {
        removeAll(&((*node)->left));
        removeAll(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
