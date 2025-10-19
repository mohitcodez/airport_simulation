#ifndef FLIGHT_H
#define FLIGHT_H

#define MAX_FLIGHTS 100

typedef struct {
    char id[10];
    char origin[50];
    char destination[50];
    int dep_hour, dep_min;
    int arr_hour, arr_min;
    int travel_time;      // in minutes
    float fuel_percent;   // 0-100
    char type;            // D=Domestic, I=International
} Flight;

extern Flight flights[MAX_FLIGHTS];
extern int flight_count;

// Flight operations
void addFlight();
void viewFlights();
void viewQueue();
void emergencyCheck();

#endif
