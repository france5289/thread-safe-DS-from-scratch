# Concurrent Counter

## Simple Counter

Just use a big lock to protect the counter.  
The most simple one to implement, however it is not scalable.

## Sloppy (Aggregate) Counter

The idea behind Sloppy (Aggregate) counter is, each CPU core has its own local counter, and there is a global counter in system.
Each local counter is protected by a lock, and the global counter is protected by a lock too.
We also set a threshold $S$, it the value of a local counter is greater than or equal to $S$, we need to transfer its value to global counter then set it to zero.
