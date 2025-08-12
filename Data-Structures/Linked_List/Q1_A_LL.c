//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section A - Linked List Questions
Purpose: Implementing the required functions for Question 1 */

//////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

/**
 * 단일 연결 리스트의 구조체
 * - item : 정수 데이터
 * - next : 다음 노드의 주소
 * 불변 조건 : next는 NULL(리스트 끝) 또는 유효한 ListNode*를 가리킨다
 */
typedef struct _listnode {
    int item;
    struct _listnode *next;
} ListNode; // You should not change the definition of ListNode

/**
 * 연결 리스트의 메타데이터 구조체
 * - size : 리스트의 현재 길이(노드의 수)
 * - head : 첫 번째 노드의 포인터(비어있으면 NULL)
 * 불변 조건 : size == 실제 노드의 수, head가 NULL이면 size는 0이어야 함
 */
typedef struct _linkedlist {
    int size;
    ListNode *head;
} LinkedList; // You should not change the definition of LinkedList

///////////////////////// function prototypes
///////////////////////////////////////

// You should not change the prototype of this function
// [Q1-1] 오름차순으로 정렬된 단일 연결 리스트에 중복 없이 삽입해야한다.
// 반환 : 성공 시 삽입된 인덱스(0-base), 실패 시 -1
int insertSortedLL(LinkedList *ll, int item);

void printList(LinkedList *ll);
void removeAllItems(LinkedList *ll);
ListNode *findNode(LinkedList *ll, int index);
int insertNode(LinkedList *ll, int index, int value);
int removeNode(LinkedList *ll, int index);

//////////////////////////////////////////////////////////////////////////////////

//////////////////////////// main()
/////////////////////////////////////////////////

/**
 * 콘솔 메뉴
 * 1 : 정수 하나를 정렬 리스트에 삽입
 * 2 : 방금 입력한 값의 삽입 인덱스를 출력
 * 3 : 전체 리스트 출력
 * 0 : 종료 후 메모리 해제
 *
 * 주의:
 * - 옵션 2는 가장 최근에 입력된 i, j를 사용한다
 * - 옵션 3은 리스트 출력 후 리스트의 모든 값을 지워 테스트 초기화까지 수행한다
 */
int main() {
    LinkedList ll;
    int c, i, j;
    c = 1;

    // Initialize the linked list 1 as an empty linked list
    ll.head = NULL;
    ll.size = 0;

    printf("1: Insert an integer to the sorted linked list:\n");
    printf("2: Print the index of the most recent input value:\n");
    printf("3: Print sorted linked list:\n");
    printf("0: Quit:");

    while (c != 0) {
        printf("\nPlease in put your choice(1/2/3/0): ");
        scanf("%d", &c);

        switch (c) {
        case 1:
            printf(
                "Input an integer that you want to add to the linked list: ");
            scanf("%d", &i);
            j = insertSortedLL(&ll, i);
            printf("The resulting linked list is: ");
            printList(&ll);
            break;
        case 2:
            printf("The value %d was added at index %d\n", i, j);
            break;
        case 3:
            printf("The resulting sorted linked list is: ");
            printList(&ll);
            removeAllItems(&ll);
            break;
        case 0:
            removeAllItems(&ll);
            break;
        default:
            printf("Choice unknown;\n");
            break;
        }
    }
    return 0;
}

int insertSortedLL(LinkedList *ll, int item) {
    // 잘못된 리스트 포인터 처리
    if (ll == NULL)
        return -1;

    ListNode *prev = NULL;
    ListNode *cur = ll->head;
    int index = 0;

    // 1. 삽입 위치 탐색 : 현재 값이 item보다 작다면 index ++
    // cur은 item보다 처음으로 큰 값으로 설정됨
    while (cur != NULL && cur->item < item) {
        prev = cur;
        cur = cur->next;
        index++;
    }

    // 2. 중복 검사 : cur이 존재하고 값이 동일하면 삽입하지 않음
    if (cur != NULL && cur->item == item) {
        return -1;
    }

    // 3. 새 노드 동적 할당
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) // 메모리 부족 시 실패 반환
        return -1;

    node->item = item;
    node->next = cur; // 새 노드 -> 현재 위치*cur)로 연결

    // 4. 앞/중간/뒤 삽입의 공통 처리
    if (prev == NULL) {
        ll->head = node;
    } else {
        prev->next = node;
    }

    ll->size++;

    return index;
}

///////////////////////////////////////////////////////////////////////////////////
// 리스트 내용을 순서대로 출력(공백으로 구분), 비어 있으면 "Empty"
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

// 모든 노드를 해제하여 메모리 누수 방지, 리스트를 빈 상태로 초기화
// head부터 순차적으로 free, 마지막에 head = NULL, size = 0
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

// index(0-base)에 해당하는 노드 포인터 반환
ListNode *findNode(LinkedList *ll, int index) {

    ListNode *temp;

    if (ll == NULL || index < 0 || index >= ll->size)
        return NULL;

    temp = ll->head;

    if (temp == NULL ||
        index < 0) // index < 0 조건을 불필요하게 두 번 검사하고 있는듯?
        return NULL;

    while (index > 0) {
        temp = temp->next;
        if (temp == NULL)
            return NULL;
        index--;
    }

    return temp;
}

// 지정 index 위치에 value를 삽입
int insertNode(LinkedList *ll, int index, int value) {

    ListNode *pre, *cur;

    if (ll == NULL || index < 0 || index > ll->size + 1)
        return -1;

    // If empty list or inserting first node, need to update head pointer
    if (ll->head == NULL || index == 0) {
        cur = ll->head; // NULL 혹은 첫 번째 노드의 주소가 담김
        ll->head = malloc(sizeof(ListNode)); // 새 노드를 동적 할당
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

// index의 노드를 리스트에서 제거하고 메모리 해제
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
