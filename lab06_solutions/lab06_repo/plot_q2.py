import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: ([], []))
with open("q2_matrix_operations/q2_results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row["n"]); t = float(row["time_seconds"])
        data[row["operation"]][0].append(n)
        data[row["operation"]][1].append(t)

n2_ops = ["add", "is_zero", "is_symmetric", "transpose"]
n3_ops = ["multiply", "determinant"]

fig, axes = plt.subplots(1, 2, figsize=(13, 5))

for op in n2_ops:
    ns, ts = data[op]
    axes[0].plot(ns, ts, marker='o', label=op, linewidth=1.5, markersize=4)
axes[0].set_xlabel("n (matrix size, n x n)")
axes[0].set_ylabel("time (seconds)")
axes[0].set_title("Q2: O(n^2) operations")
axes[0].legend(fontsize=9)
axes[0].set_xscale("log")
axes[0].set_yscale("log")
axes[0].grid(True, alpha=0.3)

for op in n3_ops:
    ns, ts = data[op]
    axes[1].plot(ns, ts, marker='o', label=op, linewidth=1.5, markersize=4)
ns_e, ts_e = data["eigenvalue"]
axes[1].plot(ns_e, ts_e, marker='o', label="eigenvalue (power iteration)", linewidth=1.5, markersize=4)
axes[1].set_xlabel("n (matrix size, n x n)")
axes[1].set_ylabel("time (seconds)")
axes[1].set_title("Q2: O(n^3)-ish operations")
axes[1].legend(fontsize=9)
axes[1].set_xscale("log")
axes[1].set_yscale("log")
axes[1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("q2_matrix_operations/q2_graph.png", dpi=150)
print("saved q2 graph")
