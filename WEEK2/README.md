# Merging k Sorted Arrays

This project implements and compares two methods for merging `k` sorted arrays, each containing `n` elements, into one sorted array.

## Methods

### Method 1 — Sequential Merging
Arrays are merged one after another.

**Time Complexity:** `Θ(nk²)`

### Method 2 — Pairwise Merging
Arrays are merged in pairs at each level.

**Time Complexity:** `Θ(nk log k)`

## Experiments

Two experiments were performed:

- **Fixed k = 16:** Vary `n`
- **Fixed n = 2000:** Vary `k`

For each experiment, the following were measured:

- Number of comparisons
- Execution time

## Results

The results are stored in:

- `kmerge_vs_n.csv`
- `kmerge_vs_k.csv`

Graphs are available in the `plots/` folder.

## Conclusion

Pairwise merging is more efficient than sequential merging, especially as the number of arrays `k` increases.

| Method | Complexity |
|---|---|
| Sequential | `Θ(nk²)` |
| Pairwise | `Θ(nk log k)` |
