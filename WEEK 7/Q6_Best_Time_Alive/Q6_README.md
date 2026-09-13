# Q6 — The Best Time to Be Alive

## Files
- `best_time_alive.c` — source code
- `best_time_alive` — compiled executable (Linux binary, built with `gcc -O2`)
- `output.png` — terminal screenshot of it compiling and running

## Problem
Given the birth and death years of a set of prominent (deceased)
scientists, find the year (or years) in which the largest number of them
were alive simultaneously. Tie-break rule: if scientist A died the same
year scientist B was born, A's death is considered to happen **before**
B's birth that year (so they do not count as overlapping).

## Input representation
`n` scientists, each with `(name, birthYear, deathYear)` — built in as a
small sample dataset (15 well-known scientists, Copernicus through
Hawking) directly in the source. Each lifetime is converted into two
**events**:
```
(birthYear, +1)   "a life begins"
(deathYear, -1)   "a life ends"
```

## Algorithm — sweep line / max overlapping intervals
Sort all `2n` events by year, breaking ties by processing `-1` (death)
events **before** `+1` (birth) events in the same year — this directly
encodes the problem's tie-break rule. Then sweep left to right keeping a
running `alive` counter: add the event's delta, and after every `+1`
event compare `alive` against the best seen so far, recording the year
whenever a new maximum is set.

## Usage
```bash
./best_time_alive
```
(No arguments — operates on the built-in sample dataset.)

## Verification
On the sample of 15 scientists, the program reports a peak of **5**
scientists alive at once, starting in **1912**, and lists them:
Einstein, Curie, Bohr, Turing, Ramanujan — a result that can be
manually spot-checked against their known lifespans.

## Complexity
- Sorting the `2n` events: `O(n log n)`.
- Sweeping: `O(n)`.
- Total: `O(n log n)` time, `O(n)` space.
