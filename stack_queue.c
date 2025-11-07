#include <stdio.h>
#include <stdlib.h>
#include "stack_queue.h"

void initStack(Stack *stack) {
    stack->top = -1;
}

int pushStack(Stack *stack, int flightID) {
    if (stack->top >= STACK_SIZE - 1) return 0;
    stack->data[++stack->top] = flightID;
    return 1;
}

int popStack(Stack *stack) {
    if (stack->top < 0) return -1;
    return stack->data[stack->top--];
}

void printStack(const Stack *stack) {
    if (stack->top < 0) {
        printf("Emergency stack empty.\n");
        return;
    }
    printf("Emergency stack (top -> bottom):\n");
    for (int i = stack->top; i >= 0; --i) {
        printf("Flight ID: %d\n", stack->data[i]);
    }
}

/* Queue implementation */

void initQueue(Queue *q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

int enqueue(Queue *q, int flightID) {
    QueueNode *n = (QueueNode*)malloc(sizeof(QueueNode));
    if (!n) return 0;
    n->flightID = flightID;
    n->next = NULL;
    if (!q->rear) {
        q->front = q->rear = n;
    } else {
        q->rear->next = n;
        q->rear = n;
    }
    q->size++;
    return 1;
}

int dequeue(Queue *q) {
    if (!q->front) return -1;
    QueueNode *tmp = q->front;
    int id = tmp->flightID;
    q->front = tmp->next;
    if (!q->front) q->rear = NULL;
    free(tmp);
    q->size--;
    return id;
}

void printQueue(const Queue *q) {
    if (!q->front) {
        printf("Boarding queue is empty.\n");
        return;
    }
    printf("Boarding queue (front -> rear):\n");
    QueueNode *cur = q->front;
    while (cur) {
        printf("Flight ID: %d\n", cur->flightID);
        cur = cur->next;
    }
}

void freeQueue(Queue *q) {
    while (q->front) {
        QueueNode *tmp = q->front;
        q->front = q->front->next;
        free(tmp);
    }
    q->rear = NULL;
    q->size = 0;
}