#include <stdio.h>
#include <stdlib.h>
#include "flight.h"

int main() {
    int choice;
    while(1){
        printf("\n===== AIRPORT SIMULATION =====\n");
        printf("1. Add Flight\n2. View All Flights\n3. View Departure Queue\n4. Emergency Check\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&choice);

        switch(choice){
            case 1: addFlight(); break;
            case 2: viewFlights(); break;
            case 3: viewQueue(); break;
            case 4: emergencyCheck(); break;
            case 5: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
