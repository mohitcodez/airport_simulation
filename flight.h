#ifndef FLIGHT_H
#define FLIGHT_H

typedef struct {
    int id;
    char airline[32];
    char source[32];
    char destination[32];
    char departure[16];
    char arrival[16];
} Flight;

typedef struct FlightNode {
    Flight flight;
    struct FlightNode *next;
} FlightNode;

// Basic operations
Flight inputFlight();
void addFlight(FlightNode **head, Flight flight);
FlightNode* searchFlightByID(FlightNode *head, int id);
void searchFlightByDestination(FlightNode *head, const char *destination);
int deleteFlight(FlightNode **head, int id);
void printFlight(const Flight *flight);
void printAllFlights(FlightNode *head);
void freeFlights(FlightNode *head);

#endif