//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section A - Linked List Questions
Purpose: Implementing the required functions for Question 2 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _listnode {
    int item;
    struct _listnode *next;
} ListNode; // You should not change the definition of ListNode

typedef struct _linkedlist {
    int size;
    ListNode *head;
} LinkedList; // You should not change the definition of LinkedList

//////////////////////// function prototypes /////////////////////////////////////

// You should not change the prototype of this function
void alternateMergeLinkedList(LinkedList *ll1, LinkedList *ll2);

void printList(LinkedList *ll);
void removeAllItems(LinkedList *ll);
ListNode *findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);

//////////////////////////// main() //////////////////////////////////////////////

int main() {
    LinkedList ll1, ll2;
    int c, i, j;
    c = 1;
    // Initialize the linked list 1 as an empty linked list
    ll1.head = NULL;
    ll1.size = 0;

    // Initialize the linked list 2 as an empty linked list
    ll2.head = NULL;
    ll2.size = 0;

    printf("1: Insert an integer to the linked list 1:\n");
    printf("2: Insert an integer to the linked list 2:\n");
    printf("3: Create the alternate merged linked list:\n");
    printf("0: Quit:\n");

    while (c != 0) {
        printf("Please input your choice(1/2/3/0): ");
        scanf("%d", &c);

        switch (c) {
        case 1:
            printf("Input an integer that you want to add to the linked list 1: ");
            scanf("%d", &i);
            j = insertNode(&ll1, ll1.size, i);
            printf("Linked list 1: ");
            printList(&ll1);
            break;
        case 2:
            printf("Input an integer that you want to add to the linked list 2: ");
            scanf("%d", &i);
            j = insertNode(&ll2, ll2.size, i);
            printf("Linked list 2: ");
            printList(&ll2);
            break;
        case 3:
            printf("The resulting linked lists after merging the given linked list are:\n");
            alternateMergeLinkedList(&ll1, &ll2); // You need to code this function
            printf("The resulting linked list 1: ");
            printList(&ll1);
            printf("The resulting linked list 2: ");
            printList(&ll2);
            removeAllItems(&ll1);
            removeAllItems(&ll2);
            break;
        case 0:
            removeAllItems(&ll1);
            removeAllItems(&ll2);
            break;
        default:
            printf("Choice unknown;\n");
            break;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////

// 1번 리스트로 2번 리스트의 노드들을 하나씩 넣어야 함
// 1번 리스트는 1번과 2번 각 노드들이 번갈아간 상태로 정렬되어야 함
// 1번 리스트의 최대 크기는 1번 리스트 size * 2를 넘을 수 없음
// 새로운 리스트를 만들거나 복사하는 것이 아닌 원본 노드의 연결을 직접 바꿔야 함
void alternateMergeLinkedList(LinkedList *ll1, LinkedList *ll2) {
    // 올바르지 않은 ll 처리
    if (ll1 == NULL || ll2 == NULL) {
        return;
    }
    // 둘 중 하나라도 비어있는 경우
    if (ll1->head == NULL || ll2->head == NULL) {
        return;
    }
    // head를 각 초기값으로 설정, 현재 노드들은 node1, node2
    ListNode *node1 = ll1->head;
    ListNode *node2 = ll2->head;
    // 모든 노드가 NULL이 아니라면 반복
    while (node1 != NULL && node2 != NULL) {
        // 각 노드의 다음 - next1, next2
        ListNode *next1 = node1->next;
        ListNode *next2 = node2->next;

        // node1의 next를 node2로 설정, node2의 next는 next1로 설정
        node1->next = node2;
        node2->next = next1;

        // ll1과 ll2의 size--
        ll1->size--;
        ll2->size--;

        // 현재 노드들을 next로 설정
        node1 = next1;
        node2 = next2;
    }
    // ll2의 head를 cur2로 설정
    ll2->head = node2;
}

///////////////////////////////////////////////////////////////////////////////////

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

    if (ll == NULL || index < 0 || index > ll->size + 1) {
        return -1;
    }

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
