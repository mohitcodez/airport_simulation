#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "flight.h"

static int generateUniqueID() {
    return rand() % 90000 + 10000; // 5-digit random
}

Flight inputFlight() {
    Flight flight;
    srand((unsigned int)time(NULL) + rand());
    flight.id = generateUniqueID();
    printf("Enter Airline Name: ");
    fgets(flight.airline, sizeof(flight.airline), stdin);
    flight.airline[strcspn(flight.airline, "\n")] = 0;
    printf("Enter Source: ");
    fgets(flight.source, sizeof(flight.source), stdin);
    flight.source[strcspn(flight.source, "\n")] = 0;
    printf("Enter Destination: ");
    fgets(flight.destination, sizeof(flight.destination), stdin);
    flight.destination[strcspn(flight.destination, "\n")] = 0;
    printf("Enter Departure Time (HH:MM): ");
    fgets(flight.departure, sizeof(flight.departure), stdin);
    flight.departure[strcspn(flight.departure, "\n")] = 0;
    printf("Enter Arrival Time (HH:MM): ");
    fgets(flight.arrival, sizeof(flight.arrival), stdin);
    flight.arrival[strcspn(flight.arrival, "\n")] = 0;
    return flight;
}

void addFlight(FlightNode **head, Flight flight) {
    FlightNode *newNode = (FlightNode*)malloc(sizeof(FlightNode));
    newNode->flight = flight;
    newNode->next = *head;
    *head = newNode;
}

FlightNode* searchFlightByID(FlightNode *head, int id) {
    FlightNode *curr = head;
    while (curr) {
        if (curr->flight.id == id)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void searchFlightByDestination(FlightNode *head, const char *destination) {
    int found = 0;
    FlightNode *curr = head;
    while (curr) {
        if (strcmp(curr->flight.destination, destination) == 0) {
            printFlight(&(curr->flight));
            found = 1;
        }
        curr = curr->next;
    }
    if (!found)
        printf("No flights found to destination: %s\n", destination);
}

int deleteFlight(FlightNode **head, int id) {
    FlightNode *curr = *head, *prev = NULL;
    while (curr) {
        if (curr->flight.id == id) {
            if (prev) prev->next = curr->next;
            else *head = curr->next;
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

void printFlight(const Flight *flight) {
    printf("ID: %d | Airline: %s | Src: %s | Dest: %s | Dep: %s | Arr: %s\n",
           flight->id, flight->airline, flight->source, flight->destination,
           flight->departure, flight->arrival);
}

void printAllFlights(FlightNode *head) {
    FlightNode *curr = head;
    if (!curr) {
        printf("No flights available.\n");
        return;
    }
    while (curr) {
        printFlight(&(curr->flight));
        curr = curr->next;
    }
}

void freeFlights(FlightNode *head) {
    FlightNode *curr = head;
    while (curr) {
        FlightNode *tmp = curr;
        curr = curr->next;
        free(tmp);
    }
}