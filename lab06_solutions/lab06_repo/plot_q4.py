import csv
import math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: ([], []))
with open("q4_sorting_reversal/q4_results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row["n"]); v = float(row["value"])
        data[row["metric"]][0].append(n)
        data[row["metric"]][1].append(v)

fig, axes = plt.subplots(1, 2, figsize=(13, 5))

ns, vs = data["partA_reversal_count"]
axes[0].plot(ns, vs, marker='o', label="actual reversal count", linewidth=2, markersize=5)
axes[0].plot(ns, [2*(n-1) for n in ns], linestyle='--', label="bound: 2(n-1)", color='red')
axes[0].set_xlabel("n (permutation size)")
axes[0].set_ylabel("number of reversals")
axes[0].set_title("Q4 Part A: O(n) reversal count")
axes[0].legend(fontsize=9)
axes[0].grid(True, alpha=0.3)

ns2, vs2 = data["partB_total_cost"]
axes[1].plot(ns2, vs2, marker='o', label="actual total reversal cost", linewidth=2, markersize=5)
# scale reference curve n*log2(n)^2 to match at the last point for visual comparison
ref = [n * (math.log2(n)**2) for n in ns2]
scale = vs2[-1] / ref[-1]
axes[1].plot(ns2, [r*scale for r in ref], linestyle='--', label="reference: n*log2(n)^2 (scaled)", color='red')
axes[1].set_xlabel("n (permutation size)")
axes[1].set_ylabel("total reversal cost")
axes[1].set_title("Q4 Part B: O(n log^2 n) reversal cost")
axes[1].legend(fontsize=9)
axes[1].set_xscale("log")
axes[1].set_yscale("log")
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("q4_sorting_reversal/q4_graph.png", dpi=150)
print("saved q4 graph")
