#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "flight.h"

Flight flights[MAX_FLIGHTS];
int flightCount = 0;
const char *DATA_FILE = "flights.dat";

// Internal function prototypes
void generateFlightID(char *id);
void generateRandomTime(int *hour, int *min);
void calculateArrival(int depH, int depM, int travelMin, int *arrH, int *arrM);
int compareFlights(const void *a, const void *b);

// Generate random Flight ID like "AI523"
void generateFlightID(char *id) {
    char airline[3];
    airline[0] = 'A' + rand() % 26;
    airline[1] = 'A' + rand() % 26;
    airline[2] = '\0';
    int num = rand() % 900 + 100; // 100–999
    sprintf(id, "%s%d", airline, num);
}

// Generate a random time (00:00 to 23:59)
void generateRandomTime(int *hour, int *min) {
    *hour = rand() % 24;
    *min = rand() % 60;
}

// Calculate arrival time
void calculateArrival(int depH, int depM, int travelMin, int *arrH, int *arrM) {
    int total = depH * 60 + depM + travelMin;
    *arrH = (total / 60) % 24;
    *arrM = total % 60;
}

// Add flight
void addFlight() {
    if (flightCount >= MAX_FLIGHTS) {
        printf("Flight limit reached.\n");
        return;
    }

    Flight f;
    generateFlightID(f.flightID);

    printf("\nEnter Origin Airport: ");
    scanf("%s", f.origin);
    printf("Enter Destination Airport: ");
    scanf("%s", f.destination);

    generateRandomTime(&f.depHour, &f.depMin);
    f.travelTime = 60 + rand() % 300; // 1–5 hours random travel time
    calculateArrival(f.depHour, f.depMin, f.travelTime, &f.arrHour, &f.arrMin);

    flights[flightCount++] = f;
    saveFlights(); // auto-save

    printf("\nFlight added successfully!\n");
    printf("Flight ID: %s\n", f.flightID);
    printf("Departure: %02d:%02d  Arrival: %02d:%02d\n", f.depHour, f.depMin, f.arrHour, f.arrMin);
    printf("Travel Time: %d min\n", f.travelTime);
}

// View all flights
void viewFlights() {
    if (flightCount == 0) {
        printf("No flights available.\n");
        return;
    }

    printf("\n--- All Flights ---\n");
    for (int i = 0; i < flightCount; i++) {
        Flight f = flights[i];
        printf("%d) %s | %s → %s | Dep: %02d:%02d | Arr: %02d:%02d | %d min\n",
               i + 1, f.flightID, f.origin, f.destination,
               f.depHour, f.depMin, f.arrHour, f.arrMin, f.travelTime);
    }
}

// Compare by departure time
int compareFlights(const void *a, const void *b) {
    Flight *fa = (Flight *)a;
    Flight *fb = (Flight *)b;
    int timeA = fa->depHour * 60 + fa->depMin;
    int timeB = fb->depHour * 60 + fb->depMin;
    return timeA - timeB;
}

// View queue sorted by departure
void viewQueue() {
    if (flightCount == 0) {
        printf("No flights available.\n");
        return;
    }

    Flight sorted[MAX_FLIGHTS];
    memcpy(sorted, flights, sizeof(Flight) * flightCount);
    qsort(sorted, flightCount, sizeof(Flight), compareFlights);

    printf("\n--- Departure Queue (sorted by time) ---\n");
    for (int i = 0; i < flightCount; i++) {
        Flight f = sorted[i];
        printf("%d) %s | %s → %s | Dep: %02d:%02d | Arr: %02d:%02d | %d min\n",
               i + 1, f.flightID, f.origin, f.destination,
               f.depHour, f.depMin, f.arrHour, f.arrMin, f.travelTime);
    }
}

// Save flights to file
void saveFlights() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        printf("Error saving flight data!\n");
        return;
    }
    fwrite(&flightCount, sizeof(int), 1, fp);
    fwrite(flights, sizeof(Flight), flightCount, fp);
    fclose(fp);
}

// Load flights from file
void loadFlights() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        flightCount = 0;
        return;
    }
    fread(&flightCount, sizeof(int), 1, fp);
    fread(flights, sizeof(Flight), flightCount, fp);
    fclose(fp);
}
