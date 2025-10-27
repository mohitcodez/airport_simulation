#include <stdio.h>
#include <stdlib.h>
#include "stack_queue.h"

void initStack(Stack *stack) {
    stack->top = -1;
}

int pushStack(Stack *stack, int flightID) {
    if (stack->top >= STACK_SIZE - 1)
        return 0;
    stack->data[++stack->top] = flightID;
    return 1;
}

int popStack(Stack *stack) {
    if (stack->top < 0)
        return -1;
    return stack->data[stack->top--];
}

void printStack(const Stack *stack) {
    if (stack->top < 0) {
        printf("Emergency stack empty.\n");
        return;
    }
    for (int i = stack->top; i >= 0; --i)
        printf("Flight ID: %d\n", stack->data[i]);
}