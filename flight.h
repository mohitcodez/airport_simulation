#ifndef FLIGHT_H
#define FLIGHT_H

#include <time.h>

// Flight structure definition
typedef struct Flight {
    int flight_id;
    char airline_name[50];
    char source[50];
    char destination[50];
    char departure_time[20];
    char arrival_time[20];
    struct Flight* next;
} Flight;

// Function declarations for flight operations
int generate_flight_id(void);
Flight* create_flight(void);
void display_flight(const Flight* flight);
void search_flight_by_id(const Flight* head, int id);
void search_flight_by_destination(const Flight* head, const char* destination);
Flight* delete_flight(Flight* head, int id);
void display_all_flights(const Flight* head);

#endif