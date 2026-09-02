import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: ([], []))
with open("q3_convolution/q3_results.csv") as f:
    reader = csv.DictReader(f)
    for row in reader:
        n = int(row["n"]); t = float(row["time_seconds"])
        data[row["method"]][0].append(n)
        data[row["method"]][1].append(t)

fig, ax = plt.subplots(figsize=(8, 5.5))
ns, ts = data["fft"]
ax.plot(ns, ts, marker='o', label="FFT convolution O(n log n)", linewidth=2, markersize=5)
ns2, ts2 = data["naive"]
ax.plot(ns2, ts2, marker='s', label="Naive convolution O(n*m)", linewidth=2, markersize=5)

ax.set_xlabel("n (= m, vector length)")
ax.set_ylabel("time (seconds)")
ax.set_title("Q3: FFT vs Naive Convolution")
ax.legend(fontsize=10)
ax.set_xscale("log")
ax.set_yscale("log")
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("q3_convolution/q3_graph.png", dpi=150)
print("saved q3 graph")
