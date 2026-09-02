import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: ([], []))
with open("q1_array_operations/q1_results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row["n"]); t = float(row["time_seconds"])
        data[row["operation"]][0].append(n)
        data[row["operation"]][1].append(t)

on_ops = ["max", "first_second_largest", "mean", "std_dev", "remove_duplicates", "reverse", "partition"]
onlogn_ops = ["median", "mode"]

fig, axes = plt.subplots(1, 2, figsize=(13, 5))

for op in on_ops:
    ns, ts = data[op]
    axes[0].plot(ns, ts, marker='o', label=op, linewidth=1.5, markersize=4)
axes[0].set_xlabel("n (array size)")
axes[0].set_ylabel("time (seconds)")
axes[0].set_title("Q1: O(n) operations")
axes[0].legend(fontsize=8)
axes[0].set_xscale("log")
axes[0].set_yscale("log")
axes[0].grid(True, alpha=0.3)

for op in onlogn_ops:
    ns, ts = data[op]
    axes[1].plot(ns, ts, marker='o', label=op, linewidth=1.5, markersize=4)
axes[1].set_xlabel("n (array size)")
axes[1].set_ylabel("time (seconds)")
axes[1].set_title("Q1: O(n log n) operations")
axes[1].legend(fontsize=8)
axes[1].set_xscale("log")
axes[1].set_yscale("log")
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("q1_array_operations/q1_graph.png", dpi=150)
print("saved q1 graph")
