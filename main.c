#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flight.h"
#include "stack_queue.h"
#include "fileio.h"

#define FLIGHTS_FILE "flights.txt"

void display_menu(void) {
    printf("\nFlight Management System\n");
    printf("1. Add Flight\n");
    printf("2. Search Flight by ID\n");
    printf("3. Search Flight by Destination\n");
    printf("4. Delete Flight\n");
    printf("5. Emergency Landing (Push)\n");
    printf("6. Process Emergency Landing (Pop)\n");
    printf("7. Display All Flights\n");
    printf("8. Display Emergency Stack\n");
    printf("9. Display Flight Queue\n");
    printf("10. Save and Exit\n");
    printf("Enter your choice: ");
}

int main(void) {
    Flight* flight_list = load_flights_from_file(FLIGHTS_FILE);
    EmergencyStack emergency_stack;
    FlightQueue normal_queue;
    int choice, flight_id;
    char destination[50];
    
    init_emergency_stack(&emergency_stack);
    init_flight_queue(&normal_queue);
    srand((unsigned int)time(NULL));
    
    do {
        display_menu();
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                Flight* new_flight = create_flight();
                if (new_flight) {
                    enqueue_flight(&normal_queue, new_flight);
                    new_flight->next = flight_list;
                    flight_list = new_flight;
                }
                break;
            }
            case 2:
                printf("Enter Flight ID: ");
                scanf("%d", &flight_id);
                search_flight_by_id(flight_list, flight_id);
                break;
            case 3:
                printf("Enter Destination: ");
                scanf(" %[^\n]s", destination);
                search_flight_by_destination(flight_list, destination);
                break;
            case 4:
                printf("Enter Flight ID to delete: ");
                scanf("%d", &flight_id);
                flight_list = delete_flight(flight_list, flight_id);
                break;
            case 5: {
                printf("Enter Flight ID for emergency landing: ");
                scanf("%d", &flight_id);
                Flight* emergency_flight = create_flight();
                if (emergency_flight) {
                    push_emergency(&emergency_stack, emergency_flight);
                }
                break;
            }
            case 6: {
                Flight* processed = pop_emergency(&emergency_stack);
                if (processed) {
                    printf("Emergency landing processed for flight %d\n", 
                           processed->flight_id);
                    free(processed);
                }
                break;
            }
            case 7:
                display_all_flights(flight_list);
                break;
            case 8:
                display_emergency_stack(&emergency_stack);
                break;
            case 9:
                display_flight_queue(&normal_queue);
                break;
            case 10:
                save_flights_to_file(flight_list, FLIGHTS_FILE);
                printf("Flights saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 10);
    
    // Cleanup
    while (flight_list != NULL) {
        Flight* temp = flight_list;
        flight_list = flight_list->next;
        free(temp);
    }
    
    return 0;
}