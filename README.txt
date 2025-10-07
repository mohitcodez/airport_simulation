Udaan Track - Airport Runway & Flight Control System
----------------------------------------------------

Modules included:
- flight.c     -> Flight database (array)
- bst.c        -> Binary Search Tree search
- stack.c      -> Emergency/History handling
- history.c    -> Departure history log
- scheduler.c  -> Landing/Takeoff queues
- udaan.h      -> Header file
- main.c       -> Menu-driven program

To compile (Linux/macOS/Windows with GCC):
    gcc main.c flight.c bst.c stack.c history.c scheduler.c -o udaan

To run:
    ./udaan

Author: Mohit Thakur & Team (AEROSTRUCTS)
