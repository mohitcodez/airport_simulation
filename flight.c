#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"

/* Sequential ID counter:
 * - nextID is initialized on first call to getUniqueID by scanning the existing list
 *   and setting nextID = maxExistingID + 1 (or 1 if list empty).
 * - Each call returns nextID++.
 */
int getUniqueID(FlightNode *head) {
    static int nextID = 0;
    if (nextID == 0) {
        int max = 0;
        FlightNode *cur = head;
        while (cur) {
            if (cur->flight.id > max) max = cur->flight.id;
            cur = cur->next;
        }
        nextID = (max >= 1) ? (max + 1) : 1;
    }
    return nextID++;
}

Flight inputFlight(void) {
    Flight flight;
    /* ID assigned by caller (main) using getUniqueID */
    flight.id = 0;
    printf("Enter Airline Name: ");
    fgets(flight.airline, sizeof(flight.airline), stdin);
    flight.airline[strcspn(flight.airline, "\n")] = 0;
    printf("Enter Source (airport name): ");
    fgets(flight.source, sizeof(flight.source), stdin);
    flight.source[strcspn(flight.source, "\n")] = 0;
    printf("Enter Destination (airport name): ");
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
    FlightNode *node = (FlightNode*)malloc(sizeof(FlightNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed while adding flight.\n");
        return;
    }
    node->flight = flight;
    node->next = *head;
    *head = node;
}

FlightNode* searchFlightByID(FlightNode *head, int id) {
    FlightNode *cur = head;
    while (cur) {
        if (cur->flight.id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

void searchFlightByDestination(FlightNode *head, const char *destination) {
    int found = 0;
    FlightNode *cur = head;
    while (cur) {
        if (strcmp(cur->flight.destination, destination) == 0) {
            printFlight(&(cur->flight));
            found = 1;
        }
        cur = cur->next;
    }
    if (!found) printf("No flights found to destination: %s\n", destination);
}

int deleteFlight(FlightNode **head, int id) {
    FlightNode *cur = *head, *prev = NULL;
    while (cur) {
        if (cur->flight.id == id) {
            if (prev) prev->next = cur->next;
            else *head = cur->next;
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

void printFlight(const Flight *flight) {
    printf("ID: %d | Airline: %s | %s -> %s | Dep: %s | Arr: %s\n",
           flight->id, flight->airline, flight->source, flight->destination,
           flight->departure, flight->arrival);
}

void printAllFlights(FlightNode *head) {
    if (!head) {
        printf("No flights available.\n");
        return;
    }
    FlightNode *cur = head;
    while (cur) {
        printFlight(&(cur->flight));
        cur = cur->next;
    }
}

void freeFlights(FlightNode *head) {
    FlightNode *cur = head;
    while (cur) {
        FlightNode *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}