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

```markdown
# Flight Management System — Python GUI Frontend

This repository adds a Python Tkinter GUI for the Flight Management System project (the original core was implemented in C).
The GUI operates on the same `flights.txt` CSV file used by the C program, so both frontends share the same persisted data.

Files added:
- model.py  — Contains Python-side data model (Flight, FlightManager) and data structures (stack, queue, BST, graph).
- gui.py    — Tkinter GUI that calls methods from model.py and saves/loads `flights.txt`.
- README.md — This file.

How it works:
- The GUI reads `flights.txt` on start (if present) and builds in-memory indices (BST and graph).
- Actions in GUI (add/delete) update `flights.txt` immediately (so the C program can read the changes).
- The GUI demonstrates:
  - Linked-list-style storage (Python list of Flight)
  - Array stack (emergency)
  - Linked queue (boarding)
  - Binary search tree (index by ID, inorder traversal)
  - Graph + BFS itinerary finder (connections)

Run:
1. Make sure you have Python 3.7+ installed (Tkinter is included in standard CPython distributions).
2. Place `model.py`, `gui.py`, and `flights.txt` (if present) in the same folder as your C project.
3. Run:
   python gui.py

Notes:
- The GUI assigns sequential IDs starting at 1 or continuing from the max ID already in `flights.txt`.
- Changes are saved to `flights.txt`. If you plan to use both the C binary and the GUI concurrently, avoid simultaneous writes.
- If you want the GUI to call the C binary directly (e.g., to perform emergency landing logic in C), that integration can be added — currently the GUI reimplements the features in Python and uses `flights.txt` as the shared persistence layer.

Next steps (optional):
- Add integration to call the C binary for some operations via subprocess or ctypes.
- Add persistence for emergency stack and boarding queue.
- Improve UI (table view, sorting, filtering) and add sample demo scripts.
