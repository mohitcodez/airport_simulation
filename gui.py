"""
gui.py
Tkinter-based GUI front-end for the Flight Management System.

Usage:
    python gui.py

The GUI operates on the same flights.txt used by the C program. Changes made in GUI
are saved to flights.txt so the C program sees them on the next run.

Features exposed in GUI:
- List all flights
- Add flight (IDs auto-increment starting at 1)
- Search by ID / Destination
- Delete flight
- Emergency stack push/pop & view
- Boarding queue enqueue/dequeue & view
- Show flights sorted by ID (BST inorder)
- Find itinerary (graph BFS): source, destination, max stops
"""

import tkinter as tk
from tkinter import ttk, messagebox
from model import FlightManager, Flight

# Create a single FlightManager instance used by the GUI
fm = FlightManager()


class FlightApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Flight Management System (GUI)")
        self.geometry("900x600")
        self.create_widgets()
        self.refresh_all_lists()

    def create_widgets(self):
        # Left frame: flight list and actions
        left = ttk.Frame(self)
        left.pack(side=tk.LEFT, fill=tk.BOTH, expand=True, padx=8, pady=8)

        # Flights list
        lbl = ttk.Label(left, text="All Flights")
        lbl.pack(anchor=tk.W)
        self.flight_listbox = tk.Listbox(left, height=15)
        self.flight_listbox.pack(fill=tk.BOTH, expand=True)
        self.flight_listbox.bind("<<ListboxSelect>>", self.on_flight_select)

        # Action buttons under flights
        btn_frame = ttk.Frame(left)
        btn_frame.pack(fill=tk.X, pady=4)
        ttk.Button(btn_frame, text="Refresh", command=self.refresh_all_lists).pack(side=tk.LEFT)
        ttk.Button(btn_frame, text="Show Sorted (Tree)", command=self.show_sorted).pack(side=tk.LEFT, padx=4)
        ttk.Button(btn_frame, text="Rebuild Index", command=self.rebuild_indices).pack(side=tk.LEFT)

        # Right frame: forms and other structures
        right = ttk.Frame(self)
        right.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True, padx=8, pady=8)

        # Add flight form
        add_frame = ttk.LabelFrame(right, text="Add Flight")
        add_frame.pack(fill=tk.X, pady=4)
        self.add_airline = tk.Entry(add_frame)
        self.add_source = tk.Entry(add_frame)
        self.add_dest = tk.Entry(add_frame)
        self.add_dep = tk.Entry(add_frame)
        self.add_arr = tk.Entry(add_frame)
        rows = [
            ("Airline:", self.add_airline),
            ("Source:", self.add_source),
            ("Destination:", self.add_dest),
            ("Departure (HH:MM):", self.add_dep),
            ("Arrival (HH:MM):", self.add_arr),
        ]
        for label_text, widget in rows:
            row = ttk.Frame(add_frame)
            row.pack(fill=tk.X, pady=2)
            ttk.Label(row, text=label_text, width=20).pack(side=tk.LEFT)
            widget.pack(side=tk.LEFT, fill=tk.X, expand=True)

        ttk.Button(add_frame, text="Add Flight", command=self.add_flight).pack(pady=6)

        # Search / Delete
        sd_frame = ttk.LabelFrame(right, text="Search / Delete")
        sd_frame.pack(fill=tk.X, pady=4)
        srow = ttk.Frame(sd_frame)
        srow.pack(fill=tk.X, pady=2)
        ttk.Label(srow, text="Search by ID:", width=20).pack(side=tk.LEFT)
        self.search_id_entry = tk.Entry(srow)
        self.search_id_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        ttk.Button(srow, text="Search", command=self.search_by_id).pack(side=tk.LEFT, padx=4)

        srow2 = ttk.Frame(sd_frame)
        srow2.pack(fill=tk.X, pady=2)
        ttk.Label(srow2, text="Search by Destination:", width=20).pack(side=tk.LEFT)
        self.search_dest_entry = tk.Entry(srow2)
        self.search_dest_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        ttk.Button(srow2, text="Search", command=self.search_by_destination).pack(side=tk.LEFT, padx=4)

        del_frame = ttk.Frame(sd_frame)
        del_frame.pack(fill=tk.X, pady=2)
        ttk.Label(del_frame, text="Delete Flight ID:", width=20).pack(side=tk.LEFT)
        self.delete_id_entry = tk.Entry(del_frame)
        self.delete_id_entry.pack(side=tk.LEFT, fill=tk.X, expand=True)
        ttk.Button(del_frame, text="Delete", command=self.delete_flight).pack(side=tk.LEFT, padx=4)

        # Stack and Queue
        sq_frame = ttk.LabelFrame(right, text="Emergency Stack & Boarding Queue")
        sq_frame.pack(fill=tk.BOTH, expand=True, pady=4)

        # emergency stack
        es = ttk.Frame(sq_frame)
        es.pack(fill=tk.X, pady=2)
        ttk.Label(es, text="Emergency Flight ID:").pack(side=tk.LEFT)
        self.es_entry = tk.Entry(es, width=8)
        self.es_entry.pack(side=tk.LEFT, padx=4)
        ttk.Button(es, text="Push", command=self.push_emergency).pack(side=tk.LEFT, padx=2)
        ttk.Button(es, text="Pop", command=self.pop_emergency).pack(side=tk.LEFT, padx=2)
        ttk.Button(es, text="Show Stack", command=self.show_stack).pack(side=tk.LEFT, padx=4)

        # boarding queue
        bq = ttk.Frame(sq_frame)
        bq.pack(fill=tk.X, pady=2)
        ttk.Label(bq, text="Boarding Flight ID:").pack(side=tk.LEFT)
        self.bq_entry = tk.Entry(bq, width=8)
        self.bq_entry.pack(side=tk.LEFT, padx=4)
        ttk.Button(bq, text="Enqueue", command=self.enqueue_boarding).pack(side=tk.LEFT, padx=2)
        ttk.Button(bq, text="Dequeue", command=self.dequeue_boarding).pack(side=tk.LEFT, padx=2)
        ttk.Button(bq, text="Show Queue", command=self.show_queue).pack(side=tk.LEFT, padx=4)

        # Itinerary finder
        itin_frame = ttk.LabelFrame(right, text="Find Itinerary (BFS Graph)")
        itin_frame.pack(fill=tk.X, pady=4)
        row1 = ttk.Frame(itin_frame); row1.pack(fill=tk.X, pady=2)
        ttk.Label(row1, text="Source:", width=10).pack(side=tk.LEFT)
        self.itin_src = tk.Entry(row1); self.itin_src.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=4)
        ttk.Label(row1, text="Destination:", width=10).pack(side=tk.LEFT)
        self.itin_dst = tk.Entry(row1); self.itin_dst.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=4)
        row2 = ttk.Frame(itin_frame); row2.pack(fill=tk.X, pady=2)
        ttk.Label(row2, text="Max Stops (0=direct):", width=20).pack(side=tk.LEFT)
        self.itin_stops = tk.Entry(row2, width=6); self.itin_stops.pack(side=tk.LEFT)
        ttk.Button(itin_frame, text="Find Itinerary", command=self.find_itinerary).pack(pady=6)

        # Output / Logs
        out_frame = ttk.LabelFrame(self, text="Output")
        out_frame.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True, padx=8, pady=8)
        self.output_text = tk.Text(out_frame, height=12)
        self.output_text.pack(fill=tk.BOTH, expand=True)

    # -------------------------
    # GUI action methods
    # -------------------------
    def refresh_all_lists(self):
        self.flight_listbox.delete(0, tk.END)
        for f in fm.list_all():
            self.flight_listbox.insert(tk.END, self.format_flight_short(f))
        self.log("Refreshed flight list.")

    @staticmethod
    def format_flight_short(f: Flight) -> str:
        return f"{f.id}: {f.airline} | {f.source} -> {f.destination} | {f.departure}-{f.arrival}"

    def on_flight_select(self, event):
        sel = self.flight_listbox.curselection()
        if not sel:
            return
        idx = sel[0]
        f = fm.list_all()[idx]
        self.log(f"Selected Flight: {self.format_flight_short(f)}")

    def add_flight(self):
        a = self.add_airline.get().strip()
        s = self.add_source.get().strip()
        d = self.add_dest.get().strip()
        dep = self.add_dep.get().strip()
        arr = self.add_arr.get().strip()
        if not (a and s and d and dep and arr):
            messagebox.showwarning("Input error", "Please fill all fields.")
            return
        flight = fm.add_flight(airline=a, source=s, destination=d, departure=dep, arrival=arr)
        self.log(f"Added flight ID {flight.id}.")
        self.clear_add_form()
        self.refresh_all_lists()

    def clear_add_form(self):
        for e in [self.add_airline, self.add_source, self.add_dest, self.add_dep, self.add_arr]:
            e.delete(0, tk.END)

    def search_by_id(self):
        s = self.search_id_entry.get().strip()
        try:
            fid = int(s)
        except ValueError:
            messagebox.showwarning("Input error", "Enter a valid integer ID.")
            return
        f = fm.search_by_id(fid)
        if f:
            self.log(f"Found: {self.format_flight_short(f)}")
        else:
            self.log(f"No flight with ID {fid} found.")

    def search_by_destination(self):
        dest = self.search_dest_entry.get().strip()
        if not dest:
            return
        res = fm.search_by_destination(dest)
        if res:
            self.log(f"{len(res)} flight(s) to {dest}:")
            for f in res:
                self.log(self.format_flight_short(f))
        else:
            self.log(f"No flights found to {dest}.")

    def delete_flight(self):
        s = self.delete_id_entry.get().strip()
        try:
            fid = int(s)
        except ValueError:
            messagebox.showwarning("Input error", "Enter a valid integer ID.")
            return
        ok = fm.delete_flight(fid)
        if ok:
            self.log(f"Deleted flight {fid}.")
            self.refresh_all_lists()
        else:
            self.log(f"No flight with ID {fid} found.")

    # Emergency stack handlers
    def push_emergency(self):
        s = self.es_entry.get().strip()
        try:
            fid = int(s)
        except ValueError:
            messagebox.showwarning("Input error", "Enter a valid integer ID.")
            return
        ok = fm.push_emergency(fid)
        if ok:
            self.log(f"Pushed flight {fid} to emergency stack.")
        else:
            self.log(f"Failed to push flight {fid} (not found or stack full).")

    def pop_emergency(self):
        fid = fm.pop_emergency()
        if fid is None:
            self.log("Emergency stack is empty.")
        else:
            self.log(f"Popped emergency flight {fid} (performed landing).")

    def show_stack(self):
        arr = fm.list_emergency_stack()
        if not arr:
            self.log("Emergency stack empty.")
            return
        self.log("Emergency stack (top first):")
        for fid in arr:
            self.log(f"- Flight ID {fid}")

    # Boarding queue handlers
    def enqueue_boarding(self):
        s = self.bq_entry.get().strip()
        try:
            fid = int(s)
        except ValueError:
            messagebox.showwarning("Input error", "Enter a valid integer ID.")
            return
        ok = fm.enqueue_boarding(fid)
        if ok:
            self.log(f"Enqueued flight {fid} for boarding.")
        else:
            self.log(f"Failed to enqueue flight {fid} (not found).")

    def dequeue_boarding(self):
        fid = fm.dequeue_boarding()
        if fid is None:
            self.log("Boarding queue empty.")
        else:
            self.log(f"Dequeued flight {fid} (boarded).")

    def show_queue(self):
        arr = fm.list_boarding_queue()
        if not arr:
            self.log("Boarding queue empty.")
            return
        self.log("Boarding queue (front -> rear):")
        for fid in arr:
            self.log(f"- Flight ID {fid}")

    # Tree / sorted
    def show_sorted(self):
        arr = fm.inorder_traverse()
        if not arr:
            self.log("No flights to show (sorted).")
            return
        self.log("Flights sorted by ID (inorder BST):")
        for f in arr:
            self.log(self.format_flight_short(f))

    def rebuild_indices(self):
        fm.rebuild()
        self.log("Rebuilt tree index and graph from current flights.")
        self.refresh_all_lists()

    # Itinerary finder
    def find_itinerary(self):
        src = self.itin_src.get().strip()
        dst = self.itin_dst.get().strip()
        try:
            stops = int(self.itin_stops.get().strip())
        except ValueError:
            messagebox.showwarning("Input error", "Enter a valid integer for max stops.")
            return
        # ensure graph is up-to-date
        fm.rebuild()
        legs = fm.find_itinerary(src, dst, stops)
        if legs is None:
            self.log(f"No itinerary found from '{src}' to '{dst}' within {stops} stops.")
            return
        self.log(f"Itinerary from {src} to {dst} ({len(legs)} legs):")
        for i, leg in enumerate(legs, start=1):
            self.log(f"Leg {i}: {leg.id} | {leg.source} -> {leg.destination} | {leg.airline} | {leg.departure}-{leg.arrival}")

    # Logging helper
    def log(self, text: str):
        self.output_text.insert(tk.END, text + "\n")
        self.output_text.see(tk.END)


if __name__ == "__main__":
    app = FlightApp()
    app.mainloop()