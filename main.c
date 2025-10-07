#include <stdio.h>
#include <string.h>
#include "udaan.h"

int main() {
    struct FlightDB db;
    db.count = 0;
    struct Node *root = NULL;
    struct Stack emergency, historyLog;
    struct Queue landingQueue, takeoffQueue;

    initStack(&emergency);
    initStack(&historyLog);
    initQueue(&landingQueue);
    initQueue(&takeoffQueue);

    int choice;
    do {
        printf("\n=== UDAAN TRACK SYSTEM ===\n");
        printf("1. Add Flight\n2. Remove Flight\n3. Display Flights\n4. Search Flight (BST)\n");
        printf("5. Handle Emergency (Push to Stack)\n6. View Emergency Stack\n");
        printf("7. Record Flight Departure\n8. View Departure History\n");
        printf("9. Add Flight to Landing Queue\n10. Add Flight to Takeoff Queue\n");
        printf("11. Allocate Runway\n12. Handle Emergency (Land Now)\n13. Show Queues\n0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct Flight f;
            printf("Enter Flight ID: "); scanf("%d", &f.flightID);
            printf("Destination: "); scanf("%s", f.destination);
            printf("Status: "); scanf("%s", f.status);
            printf("Fuel Level: "); scanf("%d", &f.fuelLevel);
            printf("Priority (1-High, 5-Low): "); scanf("%d", &f.priority);
            printf("Aircraft Type: "); scanf("%s", f.aircraftType);
            printf("Arrival: "); scanf("%s", f.estimatedArrival);
            printf("Departure: "); scanf("%s", f.estimatedDeparture);

            addFlight(&db, f);
            root = insertBST(root, f);
        } else if (choice == 2) {
            int id; printf("Enter Flight ID to remove: "); scanf("%d", &id);
            removeFlight(&db, id);
        } else if (choice == 3) {
            displayFlights(&db);
        } else if (choice == 4) {
            int id; printf("Enter Flight ID to search: "); scanf("%d", &id);
            struct Flight* found = searchBST(root, id);
            if (found != NULL)
                printf("Found -> %s | Status: %s | Fuel: %d%%\n",
                       found->destination, found->status, found->fuelLevel);
            else printf("Flight not found!\n");
        } else if (choice == 5) {
            int id; printf("Enter Flight ID for emergency: "); scanf("%d", &id);
            struct Flight* found = searchBST(root, id);
            if (found != NULL) { push(&emergency, *found); printf("Emergency added!\n"); }
            else printf("Flight not found!\n");
        } else if (choice == 6) {
            displayStack(&emergency);
        } else if (choice == 7) {
            int id; printf("Enter Flight ID that departed: "); scanf("%d", &id);
            struct Flight* found = searchBST(root, id);
            if (found != NULL) { strcpy(found->status, "Departed"); recordDeparture(&historyLog, *found); }
            else printf("Flight not found!\n");
        } else if (choice == 8) {
            showHistory(&historyLog);
        } else if (choice == 9) {
            int id; printf("Enter Flight ID to add to landing queue: "); scanf("%d", &id);
            struct Flight* found = searchBST(root, id);
            if (found != NULL) scheduleFlights(&landingQueue, &takeoffQueue, *found, 1);
            else printf("Flight not found!\n");
        } else if (choice == 10) {
            int id; printf("Enter Flight ID to add to takeoff queue: "); scanf("%d", &id);
            struct Flight* found = searchBST(root, id);
            if (found != NULL) scheduleFlights(&landingQueue, &takeoffQueue, *found, 2);
            else printf("Flight not found!\n");
        } else if (choice == 11) {
            allocateRunway(&landingQueue, &takeoffQueue);
        } else if (choice == 12) {
            handleEmergency(&emergency, &landingQueue);
        } else if (choice == 13) {
            printf("\n--- Landing Queue ---\n"); displayQueue(&landingQueue);
            printf("\n--- Takeoff Queue ---\n"); displayQueue(&takeoffQueue);
        }
    } while (choice != 0);

    printf("Exiting system... Safe travels!\n");
    return 0;
}
