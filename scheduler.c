#include <stdio.h>
#include <string.h>
#include "udaan.h"

void initQueue(struct Queue *q) {
    q->front = q->rear = -1;
}

int isQueueEmpty(struct Queue *q) {
    return q->front == -1;
}

int isQueueFull(struct Queue *q) {
    return (q->rear + 1) % MAX_QUEUE == q->front;
}

void enqueue(struct Queue *q, struct Flight f) {
    if (isQueueFull(q)) {
        printf("Queue full! Cannot add flight %d\n", f.flightID);
        return;
    }
    if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_QUEUE;
    }
    q->flights[q->rear] = f;
}

struct Flight dequeue(struct Queue *q) {
    struct Flight empty = {-1, "", "", 0, 0, "", "", ""};
    if (isQueueEmpty(q)) {
        printf("Queue empty!\n");
        return empty;
    }
    struct Flight f = q->flights[q->front];
    if (q->front == q->rear)
        q->front = q->rear = -1;
    else
        q->front = (q->front + 1) % MAX_QUEUE;
    return f;
}

void displayQueue(struct Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Queue empty!\n");
        return;
    }
    printf("Flights in Queue:\n");
    int i = q->front;
    while (1) {
        struct Flight f = q->flights[i];
        printf("ID: %d | To: %s | Status: %s | Priority: %d\n",
               f.flightID, f.destination, f.status, f.priority);
        if (i == q->rear) break;
        i = (i + 1) % MAX_QUEUE;
    }
}



