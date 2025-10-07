#include <stdio.h>
#include "udaan.h"

void initStack(struct Stack *s) {
    s->top = -1;
}

int isEmpty(struct Stack *s) {
    return s->top == -1;
}

void push(struct Stack *s, struct Flight f) {
    if (s->top >= MAX_FLIGHTS - 1) {
        printf("Stack full!\n");
        return;
    }
    s->top++;
    s->flights[s->top] = f;
}

struct Flight pop(struct Stack *s) {
    struct Flight empty = {-1, "", "", 0, 0, "", "", ""};
    if (isEmpty(s)) {
        printf("Stack empty!\n");
        return empty;
    }
    struct Flight f = s->flights[s->top];
    s->top--;
    return f;
}

void displayStack(struct Stack *s) {
    printf("\n--- Emergency / Departed Flights ---\n");
    if (isEmpty(s)) {
        printf("No flights in stack.\n");
        return;
    }
    for (int i = s->top; i >= 0; i--) {
        printf("Flight %d -> %s\n", s->flights[i].flightID, s->flights[i].destination);
    }
}
