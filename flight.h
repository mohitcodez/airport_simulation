#ifndef FLIGHT_H
#define FLIGHT_H

#define MAX_FLIGHTS 100

typedef struct {
    char flightID[10];
    char origin[50];
    char destination[50];
    int depHour, depMin;
    int arrHour, arrMin;
    int travelTime; // in minutes
} Flight;

// Function Prototypes
void addFlight();
void viewFlights();
void viewQueue();
void loadFlights();
void saveFlights();

#endif
