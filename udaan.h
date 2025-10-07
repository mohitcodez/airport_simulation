#ifndef UDAAN_H
#define UDAAN_H

#define MAX_FLIGHTS 100
#define MAX_QUEUE 50

// ------------------ STRUCTURES ------------------
struct Flight {
    int flightID;
    char destination[30];
    char status[20];
    int fuelLevel;
    int priority;
    char aircraftType[20];
    char estimatedArrival[20];
    char estimatedDeparture[20];
};

struct FlightDB {
    struct Flight flights[MAX_FLIGHTS];
    int count;
};

struct Node {
    struct Flight data;
    struct Node *left;
    struct Node *right;
};

struct Stack {
    struct Flight flights[MAX_FLIGHTS];
    int top;
};

struct Queue {
    struct Flight flights[MAX_QUEUE];
    int front, rear;
};

// flight.c
void addFlight(struct FlightDB *db, struct Flight f);
void removeFlight(struct FlightDB *db, int id);
void displayFlights(struct FlightDB *db);

// bst.c
struct Node* insertBST(struct Node* root, struct Flight f);
struct Flight* searchBST(struct Node* root, int id);
void printBST(struct Node* root);

// stack.c
void initStack(struct Stack *s);
int isEmpty(struct Stack *s);
void push(struct Stack *s, struct Flight f);
struct Flight pop(struct Stack *s);
void displayStack(struct Stack *s);

// history.c
void recordDeparture(struct Stack *history, struct Flight f);
void showHistory(struct Stack *history);

// scheduler.c
void initQueue(struct Queue *q);
int isQueueEmpty(struct Queue *q);
int isQueueFull(struct Queue *q);
void enqueue(struct Queue *q, struct Flight f);
struct Flight dequeue(struct Queue *q);
void displayQueue(struct Queue *q);
void scheduleFlights(struct Queue *landing, struct Queue *takeoff, struct Flight f, int type);
void allocateRunway(struct Queue *landing, struct Queue *takeoff);
void handleEmergency(struct Stack *emergency, struct Queue *landing);
void handleDelay(struct Flight *f);

#endif
