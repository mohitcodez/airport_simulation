#ifndef FILEIO_H
#define FILEIO_H

#include "flight.h"

// File operations
int save_flights_to_file(const Flight* head, const char* filename);
Flight* load_flights_from_file(const char* filename);

#endif