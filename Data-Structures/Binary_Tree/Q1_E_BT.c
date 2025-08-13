//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section E - Binary Trees Questions
Purpose: Implementing the required functions for Question 1 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

/**
 * 이진 트리 노드
 * item : 노드에 저장되는 정수 값
 * left : 왼쪽 자식 노드를 가리키는 포인터
 * right : 오른쪽 자식 노드를 가리키는 포인터
 */
typedef struct _btnode {
    int item;
    struct _btnode *left;
    struct _btnode *right;
} BTNode; // You should not change the definition of BTNode

/////////////////////////////////////////////////////////////////////////////////

/**
 * 스택 노드 및 스택 구조체 - 트리 생성 시, 노드 방문 순서를 제어
 */
typedef struct _stackNode {
    BTNode *btnode;
    struct _stackNode *next;
} StackNode;

/**
 * 단순 연결 리스트 기반의 top 포인터만을 제어
 */
typedef struct _stack {
    StackNode *top;
} Stack;

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
int identical(BTNode *tree1, BTNode *tree2);

BTNode *createBTNode(int item);

BTNode *createTree();
void push(Stack *stk, BTNode *node);
BTNode *pop(Stack *stk);

void printTree(BTNode *node);
void removeAll(BTNode **node);

///////////////////////////// main() /////////////////////////////////////////////

int main() {
    int c, s;
    char e;
    BTNode *root1, *root2;

    root1 = NULL;
    root2 = NULL;
    c = 1;

    // 1. 트리 1 생성
    printf("1: Create a binary tree1.\n");
    // 2. 트리 2 생성
    printf("2: Create a binary tree2.\n");
    // 3. identical 함수를 통해 두 트리 비교
    printf("3: Check whether two trees are structurally identical.\n");
    // 4. 종료
    printf("0: Quit;\n");

    while (c != 0) {
        printf("Please input your choice(1/2/3/0): ");
        if (scanf("%d", &c) > 0)

        {

            switch (c) {
            case 1:
                // root1 트리 메모리 해제 후 새로 생성
                removeAll(&root1);
                printf("Creating tree1:\n");
                root1 = createTree();
                printf("The resulting tree1 is: ");
                printTree(root1);
                printf("\n");
                break;
            case 2:
                // root2 트리 메모리 해제 후 새로 생성
                removeAll(&root2);
                printf("Creating tree2:\n");
                root2 = createTree();
                printf("The resulting tree2 is: ");
                printTree(root2);
                printf("\n");
                break;
            case 3:
                // 두 트리 동일 여부 검사 후 매모리 해제
                s = identical(root1, root2);
                if (s) {
                    printf("Both trees are structurally identical.\n");
                } else {
                    printf("Both trees are different.\n");
                }
                removeAll(&root1);
                removeAll(&root2);
                break;
            case 0:
                removeAll(&root1);
                removeAll(&root2);
                break;
            default:
                printf("Choice unknown;\n");
                break;
            }
        } else {
            scanf("%c", &e);
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

// 재귀를 통해 두 트리가 구조 및 값이 모두 같은지를 확인
int identical(BTNode *tree1, BTNode *tree2) {
    if (tree1 == NULL && tree2 == NULL) {
        return 1;
    }
    if (tree1 == NULL || tree2 == NULL) {
        return 0;
    }
    if (tree1->item != tree2->item) {
        return 0;
    }

    return identical(tree1->left, tree2->left) && identical(tree1->right, tree2->right);
}

/////////////////////////////////////////////////////////////////////////////////

// 새 BTNode를 동적 할당하고, 값을 설정
// 자식 포인터는 NULL로 초기화
BTNode *createBTNode(int item) {
    BTNode *newNode = malloc(sizeof(BTNode));
    newNode->item = item;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//////////////////////////////////////////////////////////////////////////////////

// 사용자에게 입력을 받아 트리를 만듦
// 정수를 입력받으면 해당 자식의 노드를 생성
// 숫자가 아닌 다른 값이면 해당 자식은 NULL
BTNode *createTree() {
    Stack stk;
    BTNode *root, *temp;
    char s;
    int item;

    stk.top = NULL;
    root = NULL;

    printf("Input an integer that you want to add to the binary tree. Any Alpha value will be treated as NULL.\n");
    printf("Enter an integer value for the root: ");
    if (scanf("%d", &item) > 0) {
        root = createBTNode(item);
        push(&stk, root);
    } else {
        // 문자 입력 시 빈 트리 생성
        scanf("%c", &s);
    }

    // 모든 생성 가능한 자식을 처리할 때까지 반복
    while ((temp = pop(&stk)) != NULL) {

        printf("Enter an integer value for the Left child of %d: ", temp->item);

        if (scanf("%d", &item) > 0) {
            temp->left = createBTNode(item); // 왼쪽 자식 입력
        } else {
            scanf("%c", &s);
        }

        printf("Enter an integer value for the Right child of %d: ", temp->item);
        if (scanf("%d", &item) > 0) {
            temp->right = createBTNode(item); // 오른쪽 자식 입력
        } else {
            scanf("%c", &s);
        }

        // stack에 오른쪽에 먼저 넣어야 왼쪽부터 나오는 전위 순회가 가능해짐
        if (temp->right != NULL)
            push(&stk, temp->right);
        if (temp->left != NULL)
            push(&stk, temp->left);
    }
    return root;
}

void push(Stack *stk, BTNode *node) {
    StackNode *temp;

    temp = malloc(sizeof(StackNode));
    if (temp == NULL) // 메모리 부족 처리
        return;
    temp->btnode = node;
    if (stk->top == NULL) {
        stk->top = temp;
        temp->next = NULL;
    } else {
        temp->next = stk->top;
        stk->top = temp;
    }
}

// 스택 top을 하나 꺼내 BTNode를 반환
// 비어있다면 NULL 반환
BTNode *pop(Stack *stk) {
    StackNode *temp, *top;
    BTNode *ptr;
    ptr = NULL;

    top = stk->top;
    if (top != NULL) {
        temp = top->next;
        ptr = top->btnode;

        stk->top = temp;
        free(top);
        top = NULL;
    }
    return ptr;
}

// 중위 순회(왼->루트->오른쪽)으로 트리를 출력
void printTree(BTNode *node) {
    if (node == NULL)
        return;

    printTree(node->left);
    printf("%d ", node->item);
    printTree(node->right);
}

// 후위 순회로 모든 노드의 메모리를 해제
void removeAll(BTNode **node) {
    if (*node != NULL) {
        removeAll(&((*node)->left));
        removeAll(&((*node)->right));
        free(*node);
        *node = NULL;
    }
}
