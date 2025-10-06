#include <stdio.h>
#include <string.h>
#include "udaan.h"

void addFlight(struct FlightDB* db, struct Flight f) {
    if (db->count >= MAX_FLIGHTS) {
        printf("Database full!\n");
        return;
    }
    db->flights[db->count] = f;
    db->count++;
}

void removeFlight(struct FlightDB* db, int id) {
    int found = 0;
    for (int i = 0; i < db->count; i++) {
        if (db->flights[i].flightID == id) {
            for (int j = i; j < db->count - 1; j++) {
                db->flights[j] = db->flights[j + 1];
            }
            db->count--;
            found = 1;
            break;
        }
    }
    if (found)
        printf("Flight %d removed.\n", id);
    else
        printf("Flight not found!\n");
}

void displayFlights(struct FlightDB* db) {
    printf("\n--- Current Flights ---\n");
    if (db->count == 0) {
        printf("No flights available.\n");
        return;
    }
    for (int i = 0; i < db->count; i++) {
        struct Flight f = db->flights[i];
        printf("ID: %d | To: %s | Status: %s | Fuel: %d%% | Priority: %d\n",
            f.flightID, f.destination, f.status, f.fuelLevel, f.priority);
    }
}
