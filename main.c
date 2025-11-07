#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "flight.h"
#include "stack_queue.h"
#include "fileio.h"
#include "tree.h"
#include "graph.h"

#define DATAFILE "flights.txt"

void menu(void);

int main(void) {
    FlightNode *head = NULL;
    Stack emergencyStack;
    Queue boardingQueue;
    TreeNode *flightIndex = NULL;
    Airport *airportGraph = NULL;

    initStack(&emergencyStack);
    initQueue(&boardingQueue);

    /* Seed RNG once */
    srand((unsigned int)time(NULL));

    /* Load flights and build indexes */
    loadFlights(&head, DATAFILE);
    flightIndex = buildTreeFromList(head);
    buildGraph(&airportGraph, head);

    int choice;
    do {
        menu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Try again.\n");
            continue;
        }
        getchar(); /* consume newline */

        switch (choice) {
            case 1: {
                Flight f = inputFlight();
                f.id = getUniqueID(head);
                addFlight(&head, f);
                /* insert new head node into tree */
                flightIndex = insertTreeNode(flightIndex, head);
                /* rebuild graph for simplicity */
                freeGraph(airportGraph);
                airportGraph = NULL;
                buildGraph(&airportGraph, head);
                printf("Flight added successfully! ID: %d\n", f.id);
                break;
            }
            case 2: {
                int sc;
                printf("Search by:\n1. Flight ID\n2. Destination\nChoice: ");
                if (scanf("%d", &sc) != 1) { while (getchar()!='\n'); break; }
                getchar();
                if (sc == 1) {
                    int id;
                    printf("Enter Flight ID: ");
                    if (scanf("%d", &id) != 1) { while (getchar()!='\n'); break; }
                    getchar();
                    FlightNode *found = searchFlightByID(head, id);
                    if (found) printFlight(&(found->flight));
                    else printf("Flight not found.\n");
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
                if (scanf("%d", &id) != 1) { while (getchar()!='\n'); break; }
                getchar();
                if (deleteFlight(&head, id)) {
                    freeTree(flightIndex);
                    flightIndex = buildTreeFromList(head);
                    freeGraph(airportGraph);
                    airportGraph = NULL;
                    buildGraph(&airportGraph, head);
                    printf("Flight deleted.\n");
                } else printf("Flight not found.\n");
                break;
            }
            case 4: {
                int id;
                printf("Enter Flight ID for emergency landing: ");
                if (scanf("%d", &id) != 1) { while (getchar()!='\n'); break; }
                getchar();
                if (searchFlightByID(head, id)) {
                    if (pushStack(&emergencyStack, id)) printf("Flight %d pushed to emergency stack.\n", id);
                    else printf("Emergency stack full.\n");
                } else printf("No flight found with ID %d.\n", id);
                break;
            }
            case 5: {
                int id = popStack(&emergencyStack);
                if (id == -1) printf("No emergency flights in stack.\n");
                else printf("Emergency landing performed for Flight ID: %d\n", id);
                break;
            }
            case 6:
                printf("Emergency Stack:\n");
                printStack(&emergencyStack);
                break;
            case 7:
                printf("All Flights (unsorted linked list):\n");
                printAllFlights(head);
                break;
            case 8: {
                int id;
                printf("Enter Flight ID to enqueue for boarding: ");
                if (scanf("%d", &id) != 1) { while (getchar()!='\n'); break; }
                getchar();
                if (!searchFlightByID(head, id)) printf("No flight with ID %d.\n", id);
                else if (enqueue(&boardingQueue, id)) printf("Flight %d added to boarding queue.\n", id);
                else printf("Failed to enqueue.\n");
                break;
            }
            case 9: {
                int id = dequeue(&boardingQueue);
                if (id == -1) printf("No flights waiting to board.\n");
                else printf("Flight %d has boarded (dequeued).\n", id);
                break;
            }
            case 10:
                printQueue(&boardingQueue);
                break;
            case 11:
                printf("Flights sorted by ID (BST inorder):\n");
                inorderPrintTree(flightIndex);
                break;
            case 12:
                freeTree(flightIndex);
                flightIndex = buildTreeFromList(head);
                printf("Rebuilt flight index (tree) from linked list.\n");
                break;
            case 13:
                saveFlights(head, DATAFILE);
                printf("Data saved to %s\n", DATAFILE);
                break;
            case 14: {
                char src[32], dest[32];
                int maxStops;
                printf("Enter Source Airport: ");
                fgets(src, sizeof(src), stdin);
                src[strcspn(src, "\n")] = 0;
                printf("Enter Destination Airport: ");
                fgets(dest, sizeof(dest), stdin);
                dest[strcspn(dest, "\n")] = 0;
                printf("Enter maximum allowed stops (0 = direct only): ");
                if (scanf("%d", &maxStops) != 1) { while (getchar()!='\n'); break; }
                getchar();
                if (!airportGraph) buildGraph(&airportGraph, head);
                findItinerary(airportGraph, src, dest, maxStops);
                break;
            }
            case 0:
                saveFlights(head, DATAFILE);
                freeFlights(head);
                freeTree(flightIndex);
                freeQueue(&boardingQueue);
                freeGraph(airportGraph);
                printf("Exiting. Data saved.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}

void menu(void) {
    printf("\n=== Flight Management System (Phase 3) ===\n");
    printf("1. Add Flight\n");
    printf("2. Search Flight\n");
    printf("3. Delete Flight\n");
    printf("4. Emergency Landing (Push to stack)\n");
    printf("5. Perform Emergency Landing (Pop from stack)\n");
    printf("6. Show Emergency Stack\n");
    printf("7. List All Flights (unsorted)\n");
    printf("8. Enqueue Flight for Boarding (queue)\n");
    printf("9. Dequeue Flight (board)\n");
    printf("10. Show Boarding Queue\n");
    printf("11. Show Flights Sorted by ID (tree inorder)\n");
    printf("12. Rebuild Flight Index (tree)\n");
    printf("13. Save Data Now\n");
    printf("14. Find Itinerary / Connections (BFS on airport graph)\n");
    printf("0. Exit\n");
}