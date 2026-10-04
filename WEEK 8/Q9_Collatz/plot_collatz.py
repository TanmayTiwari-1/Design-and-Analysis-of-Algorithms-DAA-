"""Draws graphs for Q9. Run ./collatz first (mode 1 and mode 2) to create the CSV files.
   Usage: python3 plot_collatz.py"""
import csv
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

def read(name):
    with open(name) as f:
        return list(csv.DictReader(f))

try:
    t = read("trajectory.csv")
    plt.figure(figsize=(8, 4))
    plt.plot([int(r["index"]) for r in t], [int(r["value"]) for r in t], lw=1)
    plt.xlabel("step"); plt.ylabel("value"); plt.title("Collatz trajectory (%s)" % t[0]["value"])
    plt.grid(alpha=.3); plt.tight_layout(); plt.savefig("graph_trajectory.png", dpi=120); plt.close()
    print("saved graph_trajectory.png")
except FileNotFoundError:
    print("trajectory.csv not found (run mode 1 first)")

try:
    d = read("collatz_interval.csv")
    n = [int(r["n"]) for r in d]
    plt.figure(figsize=(8, 4))
    plt.scatter(n, [int(r["steps"]) for r in d], s=1)
    plt.xlabel("starting value n"); plt.ylabel("steps to reach 1")
    plt.title("Collatz stopping time over [%d, %d]" % (n[0], n[-1]))
    plt.grid(alpha=.3); plt.tight_layout(); plt.savefig("graph_interval_steps.png", dpi=120); plt.close()
    print("saved graph_interval_steps.png")
except FileNotFoundError:
    print("collatz_interval.csv not found (run mode 2 first)")
