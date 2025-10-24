#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileio.h"

int save_flights_to_file(const Flight* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return 0;
    }
    
    const Flight* current = head;
    while (current != NULL) {
        fprintf(file, "%d,%s,%s,%s,%s,%s\n",
                current->flight_id,
                current->airline_name,
                current->source,
                current->destination,
                current->departure_time,
                current->arrival_time);
        current = current->next;
    }
    
    fclose(file);
    return 1;
}

Flight* load_flights_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing flights file found.\n");
        return NULL;
    }
    
    Flight* head = NULL;
    Flight* current = NULL;
    char line[256];
    
    while (fgets(line, sizeof(line), file)) {
        Flight* new_flight = (Flight*)malloc(sizeof(Flight));
        if (!new_flight) {
            printf("Memory allocation failed while loading flights.\n");
            fclose(file);
            return head;
        }
        
        sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%[^\n]",
               &new_flight->flight_id,
               new_flight->airline_name,
               new_flight->source,
               new_flight->destination,
               new_flight->departure_time,
               new_flight->arrival_time);
        
        new_flight->next = NULL;
        
        if (head == NULL) {
            head = new_flight;
            current = head;
        } else {
            current->next = new_flight;
            current = new_flight;
        }
    }
    
    fclose(file);
    return head;
}