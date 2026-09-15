"""Map a shared device uptime clock to host monotonic time, explicitly estimated.

The minimum observed arrival-minus-source offset removes variable queue delay.
Constant transport latency cannot be identified without device clock sync.
"""
class DeviceClock:
    def __init__(self):
        self.reset()

    def reset(self):
        self.offset = None
        self.first_arrival = None
        self.last = {}
        self.count = 0

    def update(self, role, source, arrival):
        previous = self.last.get(role)
        if source <= 0:
            return False, False
        reset = False
        if previous:
            old_source, old_arrival = previous
            if source == old_source:
                return False, False
            if source < old_source or source-old_source > arrival-old_arrival+500_000_000:
                self.reset()
                reset = True
        self.last[role] = (source, arrival)
        if self.first_arrival is None:
            self.first_arrival = arrival
        offset = arrival-source
        self.offset = offset if self.offset is None else min(self.offset, offset)
        self.count += 1
        return True, reset

    def ready(self, now):
        return self.count >= 6 and self.first_arrival is not None and now-self.first_arrival >= 200_000_000

    def mapped(self, source):
        return source+self.offset
