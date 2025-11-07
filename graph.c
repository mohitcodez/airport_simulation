#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/* Helper: find airport by name */
static Airport* findAirport(Airport *head, const char *name) {
    Airport *cur = head;
    while (cur) {
        if (strcmp(cur->name, name) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* Create and prepend airport node */
static Airport* createAirport(Airport **head, const char *name) {
    Airport *a = (Airport*)malloc(sizeof(Airport));
    if (!a) return NULL;
    strncpy(a->name, name, sizeof(a->name));
    a->name[sizeof(a->name)-1] = '\0';
    a->adj = NULL;
    a->next = *head;
    a->visited = 0;
    a->parent = NULL;
    a->parentFlight = NULL;
    *head = a;
    return a;
}

/* Add adjacency flight to source airport */
static int addAdjFlight(Airport *src, FlightNode *fnode) {
    if (!src || !fnode) return 0;
    AdjFlight *af = (AdjFlight*)malloc(sizeof(AdjFlight));
    if (!af) return 0;
    af->fnode = fnode;
    af->next = src->adj;
    src->adj = af;
    return 1;
}

/* Build graph from flight list */
void buildGraph(Airport **head, FlightNode *flights) {
    if (!head) return;
    freeGraph(*head);
    *head = NULL;
    FlightNode *f = flights;
    while (f) {
        Airport *src = findAirport(*head, f->flight.source);
        if (!src) src = createAirport(head, f->flight.source);
        Airport *dst = findAirport(*head, f->flight.destination);
        if (!dst) dst = createAirport(head, f->flight.destination);
        addAdjFlight(src, f);
        f = f->next;
    }
}

/* Free graph (adj lists and airport nodes). Does not free FlightNode list. */
void freeGraph(Airport *head) {
    while (head) {
        AdjFlight *af = head->adj;
        while (af) {
            AdjFlight *tmp = af;
            af = af->next;
            free(tmp);
        }
        Airport *tmpA = head;
        head = head->next;
        free(tmpA);
    }
}

/* BFS queue node */
typedef struct BFSQ {
    Airport *airport;
    int depth;
    struct BFSQ *next;
} BFSQ;

static void bfs_enqueue(BFSQ **front, BFSQ **rear, Airport *a, int depth) {
    BFSQ *n = (BFSQ*)malloc(sizeof(BFSQ));
    if (!n) return;
    n->airport = a;
    n->depth = depth;
    n->next = NULL;
    if (!*rear) *front = *rear = n;
    else {
        (*rear)->next = n;
        *rear = n;
    }
}

static BFSQ* bfs_dequeue(BFSQ **front, BFSQ **rear) {
    if (!*front) return NULL;
    BFSQ *n = *front;
    *front = (*front)->next;
    if (!*front) *rear = NULL;
    return n;
}

static void resetBFSFields(Airport *head) {
    Airport *cur = head;
    while (cur) {
        cur->visited = 0;
        cur->parent = NULL;
        cur->parentFlight = NULL;
        cur = cur->next;
    }
}

/* BFS to find shortest-hop itinerary */
int findItinerary(Airport *airportHead, const char *src, const char *dest, int maxStops) {
    if (!airportHead || !src || !dest) return 0;
    Airport *start = findAirport(airportHead, src);
    Airport *goal = findAirport(airportHead, dest);
    if (!start) {
        printf("Source airport '%s' not found.\n", src);
        return 0;
    }
    if (!goal) {
        printf("Destination airport '%s' not found.\n", dest);
        return 0;
    }
    resetBFSFields(airportHead);
    BFSQ *front = NULL, *rear = NULL;
    bfs_enqueue(&front, &rear, start, 0);
    start->visited = 1;
    start->parent = NULL;
    start->parentFlight = NULL;
    int found = 0;
    while (front) {
        BFSQ *node = bfs_dequeue(&front, &rear);
        Airport *curA = node->airport;
        int depth = node->depth;
        free(node);
        if (strcmp(curA->name, dest) == 0) {
            found = 1;
            break;
        }
        /* Limit exploration by depth: depth counts airports, for stops interpret accordingly */
        if (depth >= maxStops + 1) continue;
        for (AdjFlight *af = curA->adj; af; af = af->next) {
            FlightNode *fnode = af->fnode;
            Airport *neighbor = findAirport(airportHead, fnode->flight.destination);
            if (!neighbor) continue;
            if (!neighbor->visited) {
                neighbor->visited = 1;
                neighbor->parent = curA;
                neighbor->parentFlight = fnode;
                bfs_enqueue(&front, &rear, neighbor, depth + 1);
            }
        }
    }

    if (!found) {
        printf("No itinerary found from '%s' to '%s' within %d stops.\n", src, dest, maxStops);
        /* clear queue nodes */
        while (front) {
            BFSQ *n = bfs_dequeue(&front, &rear);
            free(n);
        }
        resetBFSFields(airportHead);
        return 0;
    }

    /* Reconstruct path */
    Airport *cur = findAirport(airportHead, dest);
    if (!cur) return 0;
    int legs = 0;
    Airport *it = cur;
    while (it && it->parent) {
        legs++;
        it = it->parent;
    }
    if (legs == 0) {
        printf("Source equals destination.\n");
        resetBFSFields(airportHead);
        return 1;
    }
    FlightNode **legsArr = (FlightNode**)malloc(sizeof(FlightNode*) * legs);
    if (!legsArr) {
        printf("Memory allocation failed for itinerary.\n");
        resetBFSFields(airportHead);
        return 0;
    }
    int idx = legs - 1;
    it = cur;
    while (it && it->parent) {
        legsArr[idx--] = it->parentFlight;
        it = it->parent;
    }
    printf("Itinerary found from '%s' to '%s' with %d legs (stops allowed: %d):\n",
           src, dest, legs, maxStops);
    for (int i = 0; i < legs; ++i) {
        Flight *f = &legsArr[i]->flight;
        printf("Leg %d: Flight ID %d | %s -> %s | Airline: %s | Dep: %s Arr: %s\n",
               i+1, f->id, f->source, f->destination, f->airline, f->departure, f->arrival);
    }
    free(legsArr);
    resetBFSFields(airportHead);
    return 1;
}