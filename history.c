#include <stdio.h>
#include <string.h>
#include "udaan.h"

void recordDeparture(struct Stack *history, struct Flight f) {
    if (history->top >= MAX_FLIGHTS - 1) {
        printf("History log full!\n");
        return;
    }
    history->top++;
    history->flights[history->top] = f;
    printf("Flight %d recorded in history log.\n", f.flightID);
}

void showHistory(struct Stack *history) {
    printf("\n--- Departure History ---\n");
    if (history->top == -1) {
        printf("No flights in history.\n");
        return;
    }
    for (int i = history->top; i >= 0; i--) {
        struct Flight f = history->flights[i];
        printf("Flight %d | To: %s | Status: %s | Departed: %s\n",
               f.flightID, f.destination, f.status, f.estimatedDeparture);
    }
}
