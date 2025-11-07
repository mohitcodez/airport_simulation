#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"

void saveFlights(FlightNode *head, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file for saving: %s\n", filename);
        return;
    }
    FlightNode *cur = head;
    while (cur) {
        fprintf(fp, "%d,%s,%s,%s,%s,%s\n",
                cur->flight.id,
                cur->flight.airline,
                cur->flight.source,
                cur->flight.destination,
                cur->flight.departure,
                cur->flight.arrival);
        cur = cur->next;
    }
    fclose(fp);
}

void loadFlights(FlightNode **head, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        Flight flight;
        char *token = strtok(line, ",");
        if (!token) continue;
        flight.id = atoi(token);
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(flight.airline, token, sizeof(flight.airline));
        flight.airline[strcspn(flight.airline, "\n")] = 0;
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(flight.source, token, sizeof(flight.source));
        flight.source[strcspn(flight.source, "\n")] = 0;
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(flight.destination, token, sizeof(flight.destination));
        flight.destination[strcspn(flight.destination, "\n")] = 0;
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(flight.departure, token, sizeof(flight.departure));
        flight.departure[strcspn(flight.departure, "\n")] = 0;
        token = strtok(NULL, ",");
        if (!token) continue;
        strncpy(flight.arrival, token, sizeof(flight.arrival));
        flight.arrival[strcspn(flight.arrival, "\n")] = 0;
        addFlight(head, flight);
    }
    fclose(fp);
}