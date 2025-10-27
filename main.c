#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "stack_queue.h"
#include "fileio.h"

#define DATAFILE "flights.txt"

void menu();

int main() {
    FlightNode *head = NULL;
    Stack emergencyStack;
    initStack(&emergencyStack);

    loadFlights(&head, DATAFILE);

    int choice;
    do {
        menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // flush newline

        switch (choice) {
            case 1: {
                Flight flight = inputFlight();
                addFlight(&head, flight);
                printf("Flight added successfully! ID: %d\n", flight.id);
                break;
            }
            case 2: {
                int searchChoice;
                printf("Search by:\n1. Flight ID\n2. Destination\nChoice: ");
                scanf("%d", &searchChoice);
                getchar();
                if (searchChoice == 1) {
                    int id;
                    printf("Enter Flight ID: ");
                    scanf("%d", &id);
                    getchar();
                    FlightNode *found = searchFlightByID(head, id);
                    if (found) {
                        printFlight(&(found->flight));
                    } else {
                        printf("Flight not found.\n");
                    }
                } else {
                    char dest[32];
                    printf("Enter Destination: ");
                    fgets(dest, sizeof(dest), stdin);
                    dest[strcspn(dest, "\n")] = 0;
                    searchFlightByDestination(head, dest);
                }
                break;
            }
            case 3: {
                int id;
                printf("Enter Flight ID to delete: ");
                scanf("%d", &id);
                getchar();
                if (deleteFlight(&head, id)) {
                    printf("Flight deleted.\n");
                } else {
                    printf("Flight not found.\n");
                }
                break;
            }
            case 4: {
                int id;
                printf("Enter Flight ID for emergency landing: ");
                scanf("%d", &id);
                FlightNode *found = searchFlightByID(head, id);
                if (found) {
                    pushStack(&emergencyStack, id);
                    printf("Flight %d pushed to emergency stack.\n", id);
                } else {
                    printf("No flight found with ID %d.\n", id);
                }
                break;
            }
            case 5: {
                int id = popStack(&emergencyStack);
                if (id == -1) {
                    printf("No emergency flights in stack.\n");
                } else {
                    printf("Emergency landing performed for Flight ID: %d\n", id);
                }
                break;
            }
            case 6: {
                printf("Emergency Stack:\n");
                printStack(&emergencyStack);
                break;
            }
            case 7: {
                printf("All Flights:\n");
                printAllFlights(head);
                break;
            }
            case 0: {
                saveFlights(head, DATAFILE);
                freeFlights(head);
                printf("Exiting. Data saved.\n");
                break;
            }
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);

    return 0;
}

void menu() {
    printf("\n=== Flight Management System ===\n");
    printf("1. Add Flight\n");
    printf("2. Search Flight\n");
    printf("3. Delete Flight\n");
    printf("4. Emergency Landing (Push to stack)\n");
    printf("5. Perform Emergency Landing (Pop from stack)\n");
    printf("6. Show Emergency Stack\n");
    printf("7. List All Flights\n");
    printf("0. Exit\n");
}