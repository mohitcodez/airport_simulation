"""
model.py
Python-side model for Flight Management System.

This file implements:
- Flight dataclass
- FlightManager: loads/saves flights.txt (CSV), manages flights list (linked-list semantics via Python list)
- Stack (emergency), Queue (boarding) implementations
- BST index (TreeNode) built from flights (references Flight objects)
- Graph builder (airports adjacency) and BFS itinerary finder

The GUI (gui.py) imports this module to operate on flights.txt and in-memory data.
"""

from dataclasses import dataclass, field
from typing import List, Optional, Dict, Tuple
import csv
import os

DATAFILE = "flights.txt"


@dataclass
class Flight:
    id: int
    airline: str
    source: str
    destination: str
    departure: str
    arrival: str


class Stack:
    """Array-like stack for emergency flights (stores flight IDs)."""
    def __init__(self, capacity: int = 50):
        self.capacity = capacity
        self.data: List[int] = []

    def push(self, fid: int) -> bool:
        if len(self.data) >= self.capacity:
            return False
        self.data.append(fid)
        return True

    def pop(self) -> Optional[int]:
        if not self.data:
            return None
        return self.data.pop()

    def peek_all(self) -> List[int]:
        # top-first
        return list(reversed(self.data))


class Queue:
    """Simple FIFO queue for boarding (stores flight IDs)."""
    def __init__(self):
        self.data: List[int] = []

    def enqueue(self, fid: int) -> bool:
        self.data.append(fid)
        return True

    def dequeue(self) -> Optional[int]:
        if not self.data:
            return None
        return self.data.pop(0)

    def as_list(self) -> List[int]:
        return list(self.data)


class TreeNode:
    """BST node referencing a Flight object."""
    def __init__(self, flight: Flight):
        self.flight = flight
        self.left: Optional['TreeNode'] = None
        self.right: Optional['TreeNode'] = None


class FlightManager:
    """Manages flights and auxiliary structures for GUI use."""
    def __init__(self, datafile: str = DATAFILE):
        self.datafile = datafile
        self.flights: List[Flight] = []  # acts like linked list storage
        self.emergency_stack = Stack()
        self.boarding_queue = Queue()
        self.tree_root: Optional[TreeNode] = None
        self.graph_adj: Dict[str, List[Flight]] = {}  # source -> list of outgoing flights
        self._load()
        self._rebuild_indices()

    # ---------------------
    # Persistence
    # ---------------------
    def _load(self):
        self.flights.clear()
        if not os.path.exists(self.datafile):
            return
        with open(self.datafile, newline='', encoding='utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
                if len(row) < 6:
                    continue
                try:
                    fid = int(row[0])
                except ValueError:
                    continue
                flight = Flight(
                    id=fid,
                    airline=row[1],
                    source=row[2],
                    destination=row[3],
                    departure=row[4],
                    arrival=row[5]
                )
                # prepend to mimic C linked-list insertion order (optional)
                self.flights.append(flight)

    def save(self):
        with open(self.datafile, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            for fl in self.flights:
                writer.writerow([fl.id, fl.airline, fl.source, fl.destination, fl.departure, fl.arrival])

    # ---------------------
    # ID generation
    # ---------------------
    def _next_id(self) -> int:
        """Generate next sequential ID starting from 1, not reusing deleted IDs."""
        if not self.flights:
            return 1
        max_id = max(f.id for f in self.flights)
        return max_id + 1

    # ---------------------
    # CRUD operations
    # ---------------------
    def add_flight(self, airline: str, source: str, destination: str, departure: str, arrival: str) -> Flight:
        fid = self._next_id()
        flight = Flight(id=fid, airline=airline.strip(), source=source.strip(), destination=destination.strip(),
                        departure=departure.strip(), arrival=arrival.strip())
        # prepend to mimic original C behavior (head insert). Using append keeps chronological order.
        self.flights.append(flight)
        # update indices
        self._insert_into_tree(flight)
        self._add_to_graph(flight)
        self.save()
        return flight

    def delete_flight(self, fid: int) -> bool:
        idx = next((i for i, f in enumerate(self.flights) if f.id == fid), None)
        if idx is None:
            return False
        self.flights.pop(idx)
        # rebuild indices for simplicity
        self._rebuild_indices()
        self.save()
        return True

    def search_by_id(self, fid: int) -> Optional[Flight]:
        return next((f for f in self.flights if f.id == fid), None)

    def search_by_destination(self, dest: str) -> List[Flight]:
        return [f for f in self.flights if f.destination.lower() == dest.lower()]

    def list_all(self) -> List[Flight]:
        return list(self.flights)

    # ---------------------
    # Stack / Queue operations
    # ---------------------
    def push_emergency(self, fid: int) -> bool:
        if not self.search_by_id(fid):
            return False
        return self.emergency_stack.push(fid)

    def pop_emergency(self) -> Optional[int]:
        return self.emergency_stack.pop()

    def list_emergency_stack(self) -> List[int]:
        return self.emergency_stack.peek_all()

    def enqueue_boarding(self, fid: int) -> bool:
        if not self.search_by_id(fid):
            return False
        return self.boarding_queue.enqueue(fid)

    def dequeue_boarding(self) -> Optional[int]:
        return self.boarding_queue.dequeue()

    def list_boarding_queue(self) -> List[int]:
        return self.boarding_queue.as_list()

    # ---------------------
    # BST index (by flight id)
    # ---------------------
    def _insert_into_tree(self, flight: Flight):
        if not self.tree_root:
            self.tree_root = TreeNode(flight)
            return
        cur = self.tree_root
        while True:
            if flight.id < cur.flight.id:
                if cur.left:
                    cur = cur.left
                else:
                    cur.left = TreeNode(flight)
                    return
            elif flight.id > cur.flight.id:
                if cur.right:
                    cur = cur.right
                else:
                    cur.right = TreeNode(flight)
                    return
            else:
                # duplicate id — shouldn't happen
                return

    def _build_tree(self):
        self.tree_root = None
        for f in self.flights:
            self._insert_into_tree(f)

    def inorder_traverse(self) -> List[Flight]:
        result: List[Flight] = []

        def _inorder(node: Optional[TreeNode]):
            if not node:
                return
            _inorder(node.left)
            result.append(node.flight)
            _inorder(node.right)

        _inorder(self.tree_root)
        return result

    # ---------------------
    # Graph (airport adjacency) and BFS itinerary
    # ---------------------
    def _add_to_graph(self, flight: Flight):
        src = flight.source
        self.graph_adj.setdefault(src, []).append(flight)
        # ensure destination exists as a node (even if no outgoing edges)
        self.graph_adj.setdefault(flight.destination, [])

    def _build_graph(self):
        self.graph_adj = {}
        for f in self.flights:
            self.graph_adj.setdefault(f.source, []).append(f)
            self.graph_adj.setdefault(f.destination, [])

    def find_itinerary(self, src: str, dest: str, max_stops: int) -> Optional[List[Flight]]:
        """
        BFS to find shortest-hop itinerary from src to dest.
        max_stops is number of intermediate stops allowed (0 = direct only).
        Returns list of Flight objects representing the legs in order, or None if not found.
        """
        if src not in self.graph_adj or dest not in self.graph_adj:
            return None

        # BFS queue of (airport_name)
        from collections import deque
        q = deque()
        q.append(src)

        # visited and parent maps
        visited = {src: True}
        parent_airport: Dict[str, Optional[str]] = {src: None}
        parent_flight: Dict[str, Optional[Flight]] = {src: None}
        depth = {src: 0}  # number of airport hops from source

        found = False
        while q:
            airport = q.popleft()
            d = depth[airport]
            if airport == dest:
                found = True
                break
            # depth cutoff: allow up to max_stops intermediate stops => max airports depth = max_stops + 1
            if d >= max_stops + 1:
                continue
            for flight in self.graph_adj.get(airport, []):
                neighbor = flight.destination
                if neighbor not in visited:
                    visited[neighbor] = True
                    parent_airport[neighbor] = airport
                    parent_flight[neighbor] = flight
                    depth[neighbor] = d + 1
                    q.append(neighbor)
        if not found:
            return None

        # reconstruct path from dest to src
        legs: List[Flight] = []
        cur = dest
        while parent_airport[cur] is not None:
            f = parent_flight[cur]
            if not f:
                break
            legs.append(f)
            cur = parent_airport[cur]
        legs.reverse()
        return legs

    # ---------------------
    # Utilities
    # ---------------------
    def _rebuild_indices(self):
        self._build_tree()
        self._build_graph()

    def rebuild(self):
        """Public rebuild (call after external edits or many changes)."""
        self._rebuild_indices()
        self.save()

    # For convenience, reload from disk (in case external edits occurred)
    def reload_from_disk(self):
        self._load()
        self._rebuild_indices()