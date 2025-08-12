//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section C - Stack and Queue Questions
Purpose: Implementing the required functions for Question 3 */

//////////////////////////////////////////////////////////////////////////////////

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////   linked list /////////////////////////////////

// 단일 연결 리스트의 노드
typedef struct _listnode {
    int item;
    struct _listnode *next;
} ListNode;

// 단일 연결 리스트 전체를 관리
typedef struct _linkedlist {
    int size;
    ListNode *head;
    ListNode *tail;
} LinkedList;

////////////////////////////////// stack //////////////////////////////////////////

// 연결 리스트 기반 스택 구조체
typedef struct stack {
    LinkedList ll;
} Stack;

////////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
// pairwise consicutive : 데이터를 두 개씩 묶었을 때, 각 묶음의 두 값이 연속된 수인지
int isStackPairwiseConsecutive(Stack *s);

void push(Stack *s, int item);
int pop(Stack *s);
int peek(Stack *s);
int isEmptyStack(Stack *s);

void printList(LinkedList *ll);
ListNode *findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);
void removeAllItems(LinkedList *ll);

//////////////////////////////////////////////////////////////////////////////////////

int main() {
    int c, value;

    Stack s;

    s.ll.head = NULL;
    s.ll.size = 0;
    s.ll.tail = NULL;

    c = 1;

    printf("1: Insert an integer into the stack:\n");
    printf("2: Check the stack is pairwise consecutive:\n");
    printf("0: Quit:\n");

    while (c != 0) {
        printf("Please input your choice(1/2/0): ");
        scanf("%d", &c);

        switch (c) {
        case 1:
            // 스택에 정수 삽입
            printf("Input an integer that you want to insert into the stack: ");
            scanf("%d", &value);
            push(&s, value);
            printf("The stack is: ");
            printList(&(s.ll));
            break;
        case 2:
            // stack pairwise consecutive 검사
            if (isStackPairwiseConsecutive(&s)) {
                printf("The stack is pairwise consecutive.\n");
            } else {
                printf("The stack is not pairwise consecutive.\n");
            }
            // 검사 후 스택 비우기
            removeAllItems(&(s.ll));
            break;
        case 0:
            // 종료 전 스택 비우기
            removeAllItems(&(s.ll));
            break;
        default:
            printf("Choice unknown;\n");
            break;
        }
    }

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////

int isStackPairwiseConsecutive(Stack *s) {
    /**
     * 스택의 원소를 위에서부터 두 개씩 pop
     * 각 쌍의 절댓값 차이가 1인지 검사
     * 크기가 홀수이면 즉이 0 반환
     * push/pop만 사용하여 원소를 추가 혹은 제거
     */

    // 올바른 스택 상태 확인
    if (s == NULL)
        return 0;
    // 스택의 크기가 홀수인지 확인
    if (s->ll.size % 2 != 0)
        return 0;

    while (!isEmptyStack(s)) {
        int num_1 = pop(s);
        int num_2 = pop(s);
        // 절댓값으로 차이 계산
        int diff = abs(num_1 - num_2);
        // 차이가 1이 아니라면 실패
        if (diff != 1) {
            return 0;
        }
    }
    // 모든 쌍이 절댓값 차이 1 검증을 통과 -> True
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////

// head(인덱스 0)에 item 삽입
void push(Stack *s, int item) { insertNode(&(s->ll), 0, item); }

// head(인덱스 0) 노드 제거 후 값 반환
int pop(Stack *s) {
    int item;
    if (!isEmptyStack(s)) {
        item = ((s->ll).head)->item;
        removeNode(&(s->ll), 0);
        return item;
    }
    return INT_MIN; // 스택이 비어있을 때 반환값
}

// head 값만 반환. 제거하지는 않음
int peek(Stack *s) { return ((s->ll).head)->item; }

// 스택이 비어있다면 1, 아니라면 0 반환
int isEmptyStack(Stack *s) {
    if ((s->ll).size == 0)
        return 1;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void printList(LinkedList *ll) {

    ListNode *cur;
    if (ll == NULL)
        return;
    cur = ll->head;

    if (cur == NULL)
        printf("Empty");
    while (cur != NULL) {
        printf("%d ", cur->item);
        cur = cur->next;
    }
    printf("\n");
}

ListNode *findNode(LinkedList *ll, int index) {

    ListNode *temp;

    if (ll == NULL || index < 0 || index >= ll->size)
        return NULL;

    temp = ll->head;

    if (temp == NULL || index < 0)
        return NULL;

    while (index > 0) {
        temp = temp->next;
        if (temp == NULL)
            return NULL;
        index--;
    }

    return temp;
}

int insertNode(LinkedList *ll, int index, int value) {

    ListNode *pre, *cur;

    if (ll == NULL || index < 0 || index > ll->size + 1)
        return -1;

    // If empty list or inserting first node, need to update head pointer
    if (ll->head == NULL || index == 0) {
        cur = ll->head;
        ll->head = malloc(sizeof(ListNode));
        ll->head->item = value;
        ll->head->next = cur;
        ll->size++;
        return 0;
    }

    // Find the nodes before and at the target position
    // Create a new node and reconnect the links
    if ((pre = findNode(ll, index - 1)) != NULL) {
        cur = pre->next;
        pre->next = malloc(sizeof(ListNode));
        pre->next->item = value;
        pre->next->next = cur;
        ll->size++;
        return 0;
    }

    return -1;
}

int removeNode(LinkedList *ll, int index) {

    ListNode *pre, *cur;

    // Highest index we can remove is size-1
    if (ll == NULL || index < 0 || index >= ll->size)
        return -1;

    // If removing first node, need to update head pointer
    if (index == 0) {
        cur = ll->head->next;
        free(ll->head);
        ll->head = cur;
        ll->size--;

        return 0;
    }

    // Find the nodes before and after the target position
    // Free the target node and reconnect the links
    if ((pre = findNode(ll, index - 1)) != NULL) {

        if (pre->next == NULL)
            return -1;

        cur = pre->next;
        pre->next = cur->next;
        free(cur);
        ll->size--;
        return 0;
    }

    return -1;
}

void removeAllItems(LinkedList *ll) {
    ListNode *cur = ll->head;
    ListNode *tmp;

    while (cur != NULL) {
        tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    ll->head = NULL;
    ll->size = 0;
}
