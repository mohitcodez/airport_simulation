# gui.py
# Updated: "Show Sorted (Tree)" now sorts flights by flight time (shortest duration first).
# Uses model.FlightManager (fm) for data operations. Includes time parsing/formatting and duration calc.

import tkinter as tk
from tkinter import ttk, messagebox
from model import FlightManager, Flight

# Single FlightManager instance used by the GUI
fm = FlightManager()


def format_time(timestr: str) -> str:
    """Format time strings to HH:MM.
    Accepts '1300', '0800', '13:00', '8:00', '800' etc and returns '13:00' or '' for empty.
    """
    if not timestr:
        return ""
    s = timestr.strip()
    if ":" in s:
        # Normalize H:MM or HH:MM -> zero-pad hour and minute if needed
        parts = s.split(":")
        if len(parts) != 2:
            return s
        h = parts[0].zfill(2)
        m = parts[1].zfill(2)
        return f"{h}:{m}"
    # remove any non-digit
    digits = "".join(ch for ch in s if ch.isdigit())
    if not digits:
        return s
    if len(digits) <= 2:
        # interpret as hour only
        h = digits.zfill(2)
        return f"{h}:00"
    # last two digits are minutes
    hh = digits[:-2].zfill(2)
    mm = digits[-2:].zfill(2)
    return f"{hh}:{mm}"


def parse_time_to_minutes(timestr: str) -> int:
    """Return minutes from midnight for a HH:MM string. If invalid, return -1."""
    s = format_time(timestr)
    if not s:
        return -1
    parts = s.split(":")
    if len(parts) != 2:
        return -1
    try:
        h = int(parts[0])
        m = int(parts[1])
    except ValueError:
        return -1
    if not (0 <= h < 24 and 0 <= m < 60):
        return -1
    return h * 60 + m


def duration_minutes(dep: str, arr: str) -> int:
    """
    Compute flight duration in minutes from dep to arr.
    If arrival is earlier than departure, assume arrival is next day.
    Returns duration in minutes, or a large number (e.g., 24*60*100) if either time invalid.
    """
    d = parse_time_to_minutes(dep)
    a = parse_time_to_minutes(arr)
    if d < 0 or a < 0:
        return 24 * 60 * 100  # treat invalid as very long so they sort at the end
    if a < d:
        # arrival next day
        a += 24 * 60
    return a - d


class HighlightEntry(tk.Entry):
    """A tk.Entry subclass that highlights itself slightly when focused."""

    def __init__(self, master=None, highlight_bg="#fff9c4", normal_bg="white", **kwargs):
        super().__init__(master, bg=normal_bg, **kwargs)
        self.normal_bg = normal_bg
        self.highlight_bg = highlight_bg
        # add a light border to make it visible
        self.config(relief="solid", bd=1, highlightthickness=0)
        self.bind("<FocusIn>", self.on_focus_in)
        self.bind("<FocusOut>", self.on_focus_out)

    def on_focus_in(self, _event=None):
        self.configure(bg=self.highlight_bg)

    def on_focus_out(self, _event=None):
        self.configure(bg=self.normal_bg)


class FlightApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Flight Management System (GUI)")
        self.geometry("1000x700")
        self.create_widgets()
        self.refresh_all_lists()

    def create_widgets(self):
        # Configure main grid
        self.columnconfigure(0, weight=1)   # left column (list)
        self.columnconfigure(1, weight=0)   # middle spacer
        self.columnconfigure(2, weight=1)   # right column (forms)
        self.rowconfigure(0, weight=1)
        self.rowconfigure(1, weight=0)

        # ---------- Left pane: flights table ----------
        left_frame = ttk.Frame(self, padding=(8, 8))
        left_frame.grid(row=0, column=0, sticky="nsew")

        lbl = ttk.Label(left_frame, text="All Flights", font=("Segoe UI", 11, "bold"))
        lbl.grid(row=0, column=0, sticky="w")

        # Treeview for flights (with formatted time columns)
        columns = ("id", "airline", "source", "destination", "departure", "arrival")
        self.flight_tree = ttk.Treeview(left_frame, columns=columns, show="headings", selectmode="browse", height=20)
        self.flight_tree.grid(row=1, column=0, sticky="nsew", pady=(6, 6))
        # Configure headings
        self.flight_tree.heading("id", text="ID")
        self.flight_tree.heading("airline", text="Airline")
        self.flight_tree.heading("source", text="Source")
        self.flight_tree.heading("destination", text="Destination")
        self.flight_tree.heading("departure", text="Dep (HH:MM)")
        self.flight_tree.heading("arrival", text="Arr (HH:MM)")
        # Column widths
        self.flight_tree.column("id", width=60, anchor="center")
        self.flight_tree.column("airline", width=160, anchor="w")
        self.flight_tree.column("source", width=100, anchor="w")
        self.flight_tree.column("destination", width=100, anchor="w")
        self.flight_tree.column("departure", width=80, anchor="center")
        self.flight_tree.column("arrival", width=80, anchor="center")
        self.flight_tree.bind("<<TreeviewSelect>>", self.on_flight_select)

        # Left frame configuration
        left_frame.rowconfigure(1, weight=1)
        left_frame.columnconfigure(0, weight=1)

        # Quick action buttons under the tree
        left_btn_frame = ttk.Frame(left_frame)
        left_btn_frame.grid(row=2, column=0, sticky="ew", pady=(6, 0))
        left_btn_frame.columnconfigure((0, 1, 2), weight=1)
        ttk.Button(left_btn_frame, text="Refresh", command=self.refresh_all_lists).grid(row=0, column=0, sticky="ew", padx=4)
        ttk.Button(left_btn_frame, text="Show Sorted (Tree)", command=self.show_sorted).grid(row=0, column=1, sticky="ew", padx=4)
        ttk.Button(left_btn_frame, text="Save Now", command=lambda: (fm.save(), self.log("Saved flights to disk."))).grid(row=0, column=2, sticky="ew", padx=4)

        # ---------- Right pane: forms and controls ----------
        right_frame = ttk.Frame(self, padding=(8, 8))
        right_frame.grid(row=0, column=2, sticky="nsew")
        right_frame.columnconfigure(0, weight=1)

        # Add flight form (grid layout)
        add_frame = ttk.LabelFrame(right_frame, text="Add Flight", padding=(8, 8))
        add_frame.grid(row=0, column=0, sticky="ew", pady=(0, 8))
        for i in range(2):
            add_frame.columnconfigure(i, weight=1)
        ttk.Label(add_frame, text="Airline:").grid(row=0, column=0, sticky="e", padx=4, pady=4)
        self.add_airline = HighlightEntry(add_frame)
        self.add_airline.grid(row=0, column=1, sticky="ew", padx=4, pady=4)

        ttk.Label(add_frame, text="Source:").grid(row=1, column=0, sticky="e", padx=4, pady=4)
        self.add_source = HighlightEntry(add_frame)
        self.add_source.grid(row=1, column=1, sticky="ew", padx=4, pady=4)

        ttk.Label(add_frame, text="Destination:").grid(row=2, column=0, sticky="e", padx=4, pady=4)
        self.add_dest = HighlightEntry(add_frame)
        self.add_dest.grid(row=2, column=1, sticky="ew", padx=4, pady=4)

        ttk.Label(add_frame, text="Departure (HH:MM):").grid(row=3, column=0, sticky="e", padx=4, pady=4)
        self.add_dep = HighlightEntry(add_frame)
        self.add_dep.grid(row=3, column=1, sticky="ew", padx=4, pady=4)

        ttk.Label(add_frame, text="Arrival (HH:MM):").grid(row=4, column=0, sticky="e", padx=4, pady=4)
        self.add_arr = HighlightEntry(add_frame)
        self.add_arr.grid(row=4, column=1, sticky="ew", padx=4, pady=4)

        ttk.Button(add_frame, text="Add Flight", command=self.add_flight).grid(row=5, column=0, columnspan=2, pady=6, sticky="ew")

        # Search & Delete condensed frame
        sd_frame = ttk.LabelFrame(right_frame, text="Search / Delete", padding=(8, 8))
        sd_frame.grid(row=1, column=0, sticky="ew", pady=(0, 8))
        sd_frame.columnconfigure(1, weight=1)

        ttk.Label(sd_frame, text="Search by ID:").grid(row=0, column=0, sticky="e", padx=4, pady=4)
        self.search_id_entry = HighlightEntry(sd_frame)
        self.search_id_entry.grid(row=0, column=1, sticky="ew", padx=4, pady=4)
        ttk.Button(sd_frame, text="Search", command=self.search_by_id).grid(row=0, column=2, padx=4)

        ttk.Label(sd_frame, text="Search by Destination:").grid(row=1, column=0, sticky="e", padx=4, pady=4)
        self.search_dest_entry = HighlightEntry(sd_frame)
        self.search_dest_entry.grid(row=1, column=1, sticky="ew", padx=4, pady=4)
        ttk.Button(sd_frame, text="Search", command=self.search_by_destination).grid(row=1, column=2, padx=4)

        ttk.Label(sd_frame, text="Delete Flight ID:").grid(row=2, column=0, sticky="e", padx=4, pady=4)
        self.delete_id_entry = HighlightEntry(sd_frame)
        self.delete_id_entry.grid(row=2, column=1, sticky="ew", padx=4, pady=4)
        ttk.Button(sd_frame, text="Delete", command=self.delete_flight).grid(row=2, column=2, padx=4)

        # Stack and Queue
        sq_frame = ttk.LabelFrame(right_frame, text="Emergency Stack & Boarding Queue", padding=(8, 8))
        sq_frame.grid(row=2, column=0, sticky="ew", pady=(0, 8))
        sq_frame.columnconfigure((0, 1, 2), weight=1)

        # Emergency stack
        ttk.Label(sq_frame, text="Emergency Flight ID:").grid(row=0, column=0, sticky="e", padx=4, pady=4)
        self.es_entry = HighlightEntry(sq_frame, width=8)
        self.es_entry.grid(row=0, column=1, sticky="w", padx=4, pady=4)
        stack_btn_frame = ttk.Frame(sq_frame)
        stack_btn_frame.grid(row=0, column=2, sticky="e")
        ttk.Button(stack_btn_frame, text="Push", command=self.push_emergency).pack(side="left", padx=2)
        ttk.Button(stack_btn_frame, text="Pop", command=self.pop_emergency).pack(side="left", padx=2)
        ttk.Button(stack_btn_frame, text="Show Stack", command=self.show_stack).pack(side="left", padx=2)

        # Boarding queue
        ttk.Label(sq_frame, text="Boarding Flight ID:").grid(row=1, column=0, sticky="e", padx=4, pady=4)
        self.bq_entry = HighlightEntry(sq_frame, width=8)
        self.bq_entry.grid(row=1, column=1, sticky="w", padx=4, pady=4)
        queue_btn_frame = ttk.Frame(sq_frame)
        queue_btn_frame.grid(row=1, column=2, sticky="e")
        ttk.Button(queue_btn_frame, text="Enqueue", command=self.enqueue_boarding).pack(side="left", padx=2)
        ttk.Button(queue_btn_frame, text="Dequeue", command=self.dequeue_boarding).pack(side="left", padx=2)
        ttk.Button(queue_btn_frame, text="Show Queue", command=self.show_queue).pack(side="left", padx=2)

        # Itinerary finder
        itin_frame = ttk.LabelFrame(right_frame, text="Find Itinerary (BFS Graph)", padding=(8, 8))
        itin_frame.grid(row=3, column=0, sticky="ew", pady=(0, 8))
        itin_frame.columnconfigure((1, 3), weight=1)

        ttk.Label(itin_frame, text="Source:").grid(row=0, column=0, sticky="e", padx=4, pady=4)
        self.itin_src = HighlightEntry(itin_frame)
        self.itin_src.grid(row=0, column=1, sticky="ew", padx=4, pady=4)

        ttk.Label(itin_frame, text="Destination:").grid(row=0, column=2, sticky="e", padx=4, pady=4)
        self.itin_dst = HighlightEntry(itin_frame)
        self.itin_dst.grid(row=0, column=3, sticky="ew", padx=4, pady=4)

        ttk.Label(itin_frame, text="Max Stops:").grid(row=1, column=0, sticky="e", padx=4, pady=4)
        self.itin_stops = HighlightEntry(itin_frame, width=6)
        self.itin_stops.grid(row=1, column=1, sticky="w", padx=4, pady=4)
        ttk.Button(itin_frame, text="Find Itinerary", command=self.find_itinerary).grid(row=1, column=3, sticky="e", padx=4, pady=4)

        # ---------- Bottom pane: Output / Logs ----------
        out_frame = ttk.LabelFrame(self, text="Output & Logs", padding=(8, 8))
        out_frame.grid(row=1, column=0, columnspan=3, sticky="nsew", padx=8, pady=(0, 8))
        out_frame.columnconfigure(0, weight=1)
        out_frame.rowconfigure(0, weight=1)
        self.output_text = tk.Text(out_frame, height=10, wrap="word")
        self.output_text.grid(row=0, column=0, sticky="nsew")
        # add a vertical scrollbar
        scrollbar = ttk.Scrollbar(out_frame, command=self.output_text.yview)
        scrollbar.grid(row=0, column=1, sticky="ns")
        self.output_text["yscrollcommand"] = scrollbar.set

    # -------------------------
    # GUI action methods
    # -------------------------
    def refresh_all_lists(self):
        # Clear tree
        for iid in self.flight_tree.get_children():
            self.flight_tree.delete(iid)
        # Populate with current flights (format times)
        for f in fm.list_all():
            dep = format_time(f.departure)
            arr = format_time(f.arrival)
            # Use flight.id as iid for easy lookup
            self.flight_tree.insert("", "end", iid=str(f.id), values=(f.id, f.airline, f.source, f.destination, dep, arr))
        self.log("Refreshed flight list.")

    def on_flight_select(self, event):
        sel = self.flight_tree.selection()
        if not sel:
            return
        iid = sel[0]
        try:
            fid = int(iid)
        except ValueError:
            return
        f = fm.search_by_id(fid)
        if f:
            self.log(f"Selected Flight: {fid} | {f.airline} | {f.source} -> {f.destination} | {format_time(f.departure)}-{format_time(f.arrival)}")

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
            self.log(f"Found: {fid} | {f.airline} | {f.source} -> {f.destination} | {format_time(f.departure)}-{format_time(f.arrival)}")
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
                self.log(f"{f.id} | {f.airline} | {f.source} -> {f.destination} | {format_time(f.departure)}-{format_time(f.arrival)}")
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
            self.log(f"Flight {fid} has boarded and was removed from the schedule.")
            self.refresh_all_lists()

    def show_queue(self):
        arr = fm.list_boarding_queue()
        if not arr:
            self.log("Boarding queue empty.")
            return
        self.log("Boarding queue (front -> rear):")
        for fid in arr:
            self.log(f"- Flight ID {fid}")

    # Tree / sorted: now sorts by flight duration (shortest first)
    def show_sorted(self):
        flights = fm.list_all()
        if not flights:
            self.log("No flights to show (sorted).")
            return
        # Compute duration for each flight (in minutes) and sort ascending
        flights_with_dur = []
        for f in flights:
            dur = duration_minutes(f.departure, f.arrival)
            flights_with_dur.append((dur, f))
        flights_with_dur.sort(key=lambda x: x[0])
        self.log("Flights sorted by duration (shortest first):")
        for dur, f in flights_with_dur:
            # format duration as H:MM or M min
            if dur >= 24 * 60 * 50:
                dur_str = "N/A"
            else:
                hours = dur // 60
                mins = dur % 60
                dur_str = f"{hours:d}h {mins:02d}m" if hours > 0 else f"{mins}m"
            self.log(f"{f.id} | {f.airline} | {f.source} -> {f.destination} | {format_time(f.departure)} - {format_time(f.arrival)} | Dur: {dur_str}")

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
            self.log(f"Leg {i}: {leg.id} | {leg.source} -> {leg.destination} | {leg.airline} | {format_time(leg.departure)}-{format_time(leg.arrival)}")

    # Logging helper
    def log(self, text: str):
        self.output_text.insert(tk.END, text + "\n")
        self.output_text.see(tk.END)


if __name__ == "__main__":
    app = FlightApp()
    app.mainloop()