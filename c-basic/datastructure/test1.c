#include <stdio.h>
#include <stdlib.h>

typedef struct ListNode {
    int val;               // 节点值
    struct ListNode *next; // 指向下一节点的指针
} ListNode;

ListNode *newListNode(int val) {
    ListNode *node;
    node = (ListNode *) malloc(sizeof(ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}


ListNode *access(ListNode *head, int index) {
    for (int i = 0; i < index; i++) {
        if (head == NULL)
            return NULL;
        head = head->next;
    }
    return head;
}
int main() {
    ListNode *node1  = newListNode(111);
    ListNode *node2  = newListNode(222);
    ListNode *node3  = newListNode(333);
    ListNode *node4  = newListNode(444);
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    
    // for (size_t i = 0; i < 4; i++)
    // {
    //     printf("%d\n", access(node1, i)->val);        
    // }

    ListNode *current = node1; // 从头节点开始
    while (current != NULL) {
        printf("Value: %d\n", current->val);
        current = current->next; // 移动到下一个节点
    }

    return 0;
    

}