#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#define STACK_SIZE 50

typedef struct {
    int data[STACK_SIZE];
    int top;
} Stack;

// Stack Operations
void initStack(Stack *stack);
int pushStack(Stack *stack, int flightID);
int popStack(Stack *stack);
void printStack(const Stack *stack);

#endif