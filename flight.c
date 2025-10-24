#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "flight.h"

// Generate a random unique flight ID
int generate_flight_id(void) {
    return rand() % 90000 + 10000; // 5-digit number
}

// Create a new flight with user input
Flight* create_flight(void) {
    Flight* new_flight = (Flight*)malloc(sizeof(Flight));
    if (!new_flight) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    new_flight->flight_id = generate_flight_id();
    
    printf("\nEnter Flight Details:\n");
    printf("Airline Name: ");
    scanf(" %[^\n]s", new_flight->airline_name);
    
    printf("Source: ");
    scanf(" %[^\n]s", new_flight->source);
    
    printf("Destination: ");
    scanf(" %[^\n]s", new_flight->destination);
    
    printf("Departure Time (HH:MM): ");
    scanf(" %[^\n]s", new_flight->departure_time);
    
    printf("Arrival Time (HH:MM): ");
    scanf(" %[^\n]s", new_flight->arrival_time);
    
    new_flight->next = NULL;
    return new_flight;
}

// Display a single flight's details
void display_flight(const Flight* flight) {
    printf("\nFlight ID: %d\n", flight->flight_id);
    printf("Airline: %s\n", flight->airline_name);
    printf("Source: %s\n", flight->source);
    printf("Destination: %s\n", flight->destination);
    printf("Departure: %s\n", flight->departure_time);
    printf("Arrival: %s\n", flight->arrival_time);
}

// Search flight by ID
void search_flight_by_id(const Flight* head, int id) {
    const Flight* current = head;
    while (current != NULL) {
        if (current->flight_id == id) {
            display_flight(current);
            return;
        }
        current = current->next;
    }
    printf("Flight with ID %d not found.\n", id);
}

// Search flight by destination
void search_flight_by_destination(const Flight* head, const char* destination) {
    const Flight* current = head;
    int found = 0;
    
    while (current != NULL) {
        if (strcasecmp(current->destination, destination) == 0) {
            display_flight(current);
            found = 1;
        }
        current = current->next;
    }
    
    if (!found) {
        printf("No flights found to %s.\n", destination);
    }
}

// Delete flight by ID
Flight* delete_flight(Flight* head, int id) {
    Flight* current = head;
    Flight* prev = NULL;
    
    if (current != NULL && current->flight_id == id) {
        head = current->next;
        free(current);
        printf("Flight %d deleted successfully.\n", id);
        return head;
    }
    
    while (current != NULL && current->flight_id != id) {
        prev = current;
        current = current->next;
    }
    
    if (current == NULL) {
        printf("Flight %d not found.\n", id);
        return head;
    }
    
    prev->next = current->next;
    free(current);
    printf("Flight %d deleted successfully.\n", id);
    return head;
}

// Display all flights
void display_all_flights(const Flight* head) {
    if (head == NULL) {
        printf("No flights available.\n");
        return;
    }
    
    const Flight* current = head;
    while (current != NULL) {
        display_flight(current);
        printf("------------------------\n");
        current = current->next;
    }
}