#ifndef FILEIO_H
#define FILEIO_H

#include "flight.h"

void saveFlights(FlightNode *head, const char *filename);
void loadFlights(FlightNode **head, const char *filename);

#endif