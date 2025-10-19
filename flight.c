#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "utils.h"

Flight flights[MAX_FLIGHTS];
int flight_count = 0;

// Generate simple Flight ID
void generateFlightID(char *id) {
    static int counter = 1;
    sprintf(id, "FL%03d", counter++);
}

// Add a flight
void addFlight() {
    if (flight_count >= MAX_FLIGHTS) {
        printf("Flight limit reached.\n");
        return;
    }

    Flight f;
    generateFlightID(f.id);

    printf("Enter Origin: ");
    scanf("%s", f.origin);
    printf("Enter Destination: ");
    scanf("%s", f.destination);

    printf("Enter Departure Time (HH MM): ");
    scanf("%d %d", &f.dep_hour, &f.dep_min);
    printf("Enter Arrival Time (HH MM): ");
    scanf("%d %d", &f.arr_hour, &f.arr_min);

    f.travel_time = calculateTravelTime(f.dep_hour, f.dep_min, f.arr_hour, f.arr_min);

    printf("Enter Fuel Percentage (0-100): ");
    scanf("%f", &f.fuel_percent);

    printf("Enter Type (D/I): ");
    scanf(" %c", &f.type);

    flights[flight_count++] = f;

    printf("\nFlight %s added successfully!\n", f.id);
}

// View all flights
void viewFlights() {
    if (flight_count == 0) {
        printf("No flights available.\n");
        return;
    }
    for (int i = 0; i < flight_count; i++) {
        Flight f = flights[i];
        printf("[%s] %s -> %s | Dep: %02d:%02d Arr: %02d:%02d | %d min | %.1f%% | %s\n",
               f.id, f.origin, f.destination,
               f.dep_hour, f.dep_min, f.arr_hour, f.arr_min,
               f.travel_time, f.fuel_percent,
               (f.type=='D'||f.type=='d') ? "Domestic":"International");
    }
}

// Compare for sorting
int compareDeparture(const void *a, const void *b) {
    Flight *f1 = (Flight *)a;
    Flight *f2 = (Flight *)b;
    int t1 = f1->dep_hour*60 + f1->dep_min;
    int t2 = f2->dep_hour*60 + f2->dep_min;
    return t1 - t2;
}

// View queue sorted by departure
void viewQueue() {
    if (flight_count == 0) {
        printf("No flights available.\n");
        return;
    }

    Flight temp[MAX_FLIGHTS];
    memcpy(temp, flights, sizeof(Flight)*flight_count);
    qsort(temp, flight_count, sizeof(Flight), compareDeparture);

    printf("\n--- Departure Queue ---\n");
    for (int i = 0; i < flight_count; i++) {
        Flight f = temp[i];
        printf("[%s] %s -> %s | Dep: %02d:%02d | Fuel: %.1f%%\n",
               f.id, f.origin, f.destination, f.dep_hour, f.dep_min, f.fuel_percent);
    }
}

// Check for emergency flights (low fuel <15%)
void emergencyCheck() {
    int found=0;
    for (int i=0;i<flight_count;i++){
        if (flights[i].fuel_percent < 15.0) {
            printf("⚠️ Emergency! Flight %s has low fuel (%.1f%%)\n",
                   flights[i].id, flights[i].fuel_percent);
            found=1;
        }
    }
    if (!found) printf("All flights have safe fuel levels.\n");
}
