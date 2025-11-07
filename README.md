# Flight Management System 
A small education-focused Flight Management System that demonstrates classic data structures (linked list, stack, queue, binary search tree, graph) in a practical application. The core system is implemented in C (console application) and a convenient Python/Tkinter GUI frontend is included that reads/writes the same `flights.txt` persistence file.

This repository is intended for teaching and demonstration: it's easy to explain to faculty, shows how different data structures work together, and includes a useful feature (itinerary finder using a graph + BFS).

---

## Highlights / Key Features
- Linked list storage of flights (main persistent store)
- Emergency stack (array-based) — LIFO semantics for emergency landings
- Boarding queue (linked-list semantics in C; FIFO) — enqueue/dequeue operations
- Binary Search Tree (BST) index by Flight ID — quick ID-based search and sorted listing (inorder traversal)
- Airport graph built from the flight list — adjacency lists and BFS itinerary finder (shortest hops)
- Sequential Flight IDs starting at 1 and incrementing (keeps uniqueness across runs)
- Persistence: flights saved/loaded from `flights.txt` in CSV format
- Python/Tkinter GUI that uses the same `flights.txt` (so C and Python frontends can share data)

---

## Project structure
Important files (C backend):
- `main.c` — entry point and menu-driven console UI
- `flight.h` / `flight.c` — flight data model and linked-list operations
- `stack_queue.h` / `stack_queue.c` — emergency stack and boarding queue implementations
- `tree.h` / `tree.c` — BST index by flight ID (references linked-list nodes)
- `graph.h` / `graph.c` — airport graph and BFS itinerary finder
- `fileio.h` / `fileio.c` — load/save flights to `flights.txt`
- `Makefile` — build rules for the C program
- `flights.txt` — sample data file (CSV)

Python GUI:
- `model.py` — Python-side model (Flight dataclass), stack/queue/tree/graph and persistence
- `gui.py` — Tkinter GUI front-end interacting with `model.py`
- `requirements.txt` — notes (no external packages required; uses standard library)

Other:
- `README.md` — this file

---

## Build & run (C console program)

Prerequisites:
- GCC or other C compiler supporting C11
- Make (optional)

To build using the provided Makefile (recommended):
```bash
make
```
This creates an executable named `flight`.

Manual gcc build:
```bash
gcc -std=c11 -Wall -Wextra -O2 -o flight main.c flight.c stack_queue.c tree.c graph.c fileio.c
```

Debug build (no sanitizers on some Windows toolchains — use plain debug flags if sanitizers cause compiler issues):
```bash
gcc -std=c11 -Wall -Wextra -g -O0 -o flight_debug main.c flight.c stack_queue.c tree.c graph.c fileio.c
```

Run:
```bash
./flight        # Unix
flight.exe      # Windows
```

Menu highlights:
- 1 — Add Flight (IDs assigned sequentially starting at 1 or max(existing)+1)
- 2 — Search Flight (by ID or destination)
- 3 — Delete Flight
- 4 — Emergency Landing (push to emergency stack)
- 5 — Perform Emergency Landing (pop from stack)
- 6 — Show Emergency Stack
- 7 — List All Flights (linked list)
- 8 — Enqueue Flight for Boarding (queue)
- 9 — Dequeue Flight (board) — current behavior in C is configurable; the Python GUI removes on dequeue (see GUI notes)
- 10 — Show Boarding Queue
- 11 — Show Flights Sorted by ID (BST inorder)
- 12 — Rebuild Flight Index (tree)
- 13 — Save Data Now
- 14 — Find Itinerary / Connections (BFS on airport graph)
- 0 — Exit (saves data)

---

## Run the Python GUI

Prerequisites:
- Python 3.7+ (Tkinter included in standard CPython builds)
- Files `model.py`, `gui.py`, and `flights.txt` in the same folder as the C program if you want shared persistence

Run:
```bash
python gui.py
```

GUI features:
- Add / Delete flights
- View all flights (list)
- Show sorted flights (BST inorder)
- Emergency stack: push/pop/show
- Boarding queue: enqueue / dequeue / show
  - Note: the GUI currently removes the flight from the schedule when you dequeue (boards the flight and archives/removes it), which is useful when treating the schedule as upcoming flights only. If you prefer "mark as boarded" or archiving instead, see Proposed Improvements below.
- Find Itinerary: enter Source, Destination, and max stops (0 = direct only). GUI uses BFS on the airport graph to find shortest-hop paths.

---

## Data format (`flights.txt`)
CSV format, one flight per line:
```
id,airline,source,destination,departure,arrival
```
Example:
```
1,AirAlpha,CityA,CityB,08:00,09:30
2,BlueAir,CityB,CityC,10:00,11:15
```

Notes:
- The C program and Python GUI use the same `flights.txt`. Changes saved in one will be seen by the other on next load (or after saving).
- IDs are sequential and will start from 1 if `flights.txt` is empty; otherwise the next new ID equals (max existing ID) + 1.
