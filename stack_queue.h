#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#define STACK_SIZE 50

typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

/* Stack operations (emergency stack) */
void initStack(Stack *stack);
int pushStack(Stack *stack, int flightID); /* returns 1 on success, 0 on overflow */
int popStack(Stack *stack);                 /* returns id or -1 if empty */
void printStack(const Stack *stack);

/* Queue (boarding queue) */
typedef struct QueueNode {
    int flightID;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

void initQueue(Queue *q);
int enqueue(Queue *q, int flightID); /* returns 1 on success, 0 on failure */
int dequeue(Queue *q);               /* returns id or -1 if empty */
void printQueue(const Queue *q);
void freeQueue(Queue *q);

#endif