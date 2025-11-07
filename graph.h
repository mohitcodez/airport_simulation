#ifndef GRAPH_H
#define GRAPH_H

#include "flight.h"

/* Adjacency entry pointing to a flight (outgoing) */
typedef struct AdjFlight {
    FlightNode *fnode;
    struct AdjFlight *next;
} AdjFlight;

/* Airport node (adjacency list head). Airports stored in a linked list. */
typedef struct Airport {
    char name[32];
    AdjFlight *adj;
    struct Airport *next;

    /* BFS temporary fields */
    int visited;
    struct Airport *parent;
    FlightNode *parentFlight;
} Airport;

/* Build graph from flights linked list; frees existing graph if *head non-NULL */
void buildGraph(Airport **head, FlightNode *flights);
void freeGraph(Airport *head);

/* Find itinerary using BFS (shortest hops). Returns 1 if found and prints itinerary, 0 otherwise.
   maxStops: number of intermediate stops allowed (0 = direct only). */
int findItinerary(Airport *airportHead, const char *src, const char *dest, int maxStops);

#endif