#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flight.h"

int main() {
    int choice;
    srand(time(NULL));

    loadFlights();

    while (1) {
        printf("\n===== AIRPORT FLIGHT SIMULATION =====\n");
        printf("1. Add Flight\n");
        printf("2. View All Flights\n");
        printf("3. View Departure Queue (by Departure Time)\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addFlight();
                break;
            case 2:
                viewFlights();
                break;
            case 3:
                viewQueue();
                break;
            case 4:
                saveFlights();
                printf("Exiting... Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
