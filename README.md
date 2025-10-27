# Flight Management System

## Features

- Add Flight (with random unique ID)
- Search Flight (by ID or destination)
- Delete Flight
- Emergency Landing (LIFO stack)
- List all flights
- Data persists in flights.txt

## Compilation

### Linux (gcc) or Windows (MinGW)

To compile and run:

```sh
make
./flight_system     # Linux
flight_system.exe   # Windows (MinGW)
```

Or manually:
```sh
gcc -Wall -g -o flight_system main.c flight.c stack_queue.c fileio.c
```

## Usage

Run the executable and follow the menu.

- Flights are stored in `flights.txt` (created automatically).
- Emergency landings use a stack (LIFO): push by flight ID, pop to simulate landing.

## Cleaning Up

To remove compiled files:
```sh
make clean
```