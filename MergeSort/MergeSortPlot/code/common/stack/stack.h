/*
 * Light-weight Linked-list with a stack API
 */
#ifndef STACK_H
#define STACK_H 1
#include <stdlib.h>

typedef struct node {
    struct node * next;
    void * data;
} List;

short int empty(List * head)
{
    return head == NULL;
}

void push(List ** head, void * data)
{
    List * node = (List *) malloc(sizeof(List));
    node->next = *head;
    node->data = data;
    
    (*head) = node;
}
void * pop(List ** head)
{
    void * temp = (*head)->data;
    List * next = (*head)->next;
    free(*head);
    (*head) = next;
    return temp;
}

void clearList(List * head)
{
    while(!empty(head))
        pop(&head);
}



#endif
