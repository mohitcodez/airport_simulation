#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include "flight.h"

// Stack for emergency landings
typedef struct {
    Flight* top;
} EmergencyStack;

// Queue for normal flights
typedef struct {
    Flight* front;
    Flight* rear;
} FlightQueue;

// Stack operations
void init_emergency_stack(EmergencyStack* stack);
void push_emergency(EmergencyStack* stack, Flight* flight);
Flight* pop_emergency(EmergencyStack* stack);
void display_emergency_stack(const EmergencyStack* stack);

// Queue operations
void init_flight_queue(FlightQueue* queue);
void enqueue_flight(FlightQueue* queue, Flight* flight);
Flight* dequeue_flight(FlightQueue* queue);
void display_flight_queue(const FlightQueue* queue);

#endif