#include <stdio.h>
#include <stdlib.h>
#include "stack_queue.h"

// Stack operations
void init_emergency_stack(EmergencyStack* stack) {
    stack->top = NULL;
}

void push_emergency(EmergencyStack* stack, Flight* flight) {
    flight->next = stack->top;
    stack->top = flight;
    printf("Flight %d added to emergency stack.\n", flight->flight_id);
}

Flight* pop_emergency(EmergencyStack* stack) {
    if (stack->top == NULL) {
        printf("Emergency stack is empty.\n");
        return NULL;
    }
    
    Flight* flight = stack->top;
    stack->top = stack->top->next;
    flight->next = NULL;
    return flight;
}

void display_emergency_stack(const EmergencyStack* stack) {
    if (stack->top == NULL) {
        printf("No emergency flights.\n");
        return;
    }
    
    printf("\nEmergency Flights Stack:\n");
    const Flight* current = stack->top;
    while (current != NULL) {
        display_flight(current);
        printf("------------------------\n");
        current = current->next;
    }
}

// Queue operations
void init_flight_queue(FlightQueue* queue) {
    queue->front = queue->rear = NULL;
}

void enqueue_flight(FlightQueue* queue, Flight* flight) {
    if (queue->rear == NULL) {
        queue->front = queue->rear = flight;
    } else {
        queue->rear->next = flight;
        queue->rear = flight;
    }
    flight->next = NULL;
    printf("Flight %d added to queue.\n", flight->flight_id);
}

Flight* dequeue_flight(FlightQueue* queue) {
    if (queue->front == NULL) {
        printf("Flight queue is empty.\n");
        return NULL;
    }
    
    Flight* flight = queue->front;
    queue->front = queue->front->next;
    
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    
    flight->next = NULL;
    return flight;
}

void display_flight_queue(const FlightQueue* queue) {
    if (queue->front == NULL) {
        printf("No flights in queue.\n");
        return;
    }
    
    printf("\nFlight Queue:\n");
    const Flight* current = queue->front;
    while (current != NULL) {
        display_flight(current);
        printf("------------------------\n");
        current = current->next;
    }
}