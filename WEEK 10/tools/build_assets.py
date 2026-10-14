"""Make terminal-style PNGs and runtime-vs-N graphs for the five programs."""
import glob, random, subprocess, time, os
from PIL import Image, ImageDraw, ImageFont
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = "/home/claude/lab10"
SRC = {"q1": "q1_bipartite", "q2": "q2_islands", "q3": "q3_maxweight_size",
       "q4": "q4_reverse_dsu", "q5": "q5_smallest_perm"}
TITLE = {"q1": "Q1 Online Bipartiteness", "q2": "Q2 Number of Islands II",
         "q3": "Q3 Max Weight Component (size <= K)", "q4": "Q4 Reverse-Time DSU",
         "q5": "Q5 Lexicographically Smallest Permutation"}

FONT = (glob.glob("/usr/share/fonts/**/DejaVuSansMono.ttf", recursive=True) or [None])[0]
font = ImageFont.truetype(FONT, 18) if FONT else ImageFont.load_default()

def terminal_png(lines, path, title):
    lh, pad, bar = 26, 20, 34
    w = max(900, int(max(font.getlength(l) for l in lines)) + 2 * pad)
    h = bar + pad * 2 + lh * len(lines)
    img = Image.new("RGB", (w, h), (30, 30, 30))
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, w, bar], fill=(60, 60, 60))
    for i, c in enumerate([(255, 95, 86), (255, 189, 46), (39, 201, 63)]):
        d.ellipse([12 + i * 24, 10, 26 + i * 24, 24], fill=c)
    d.text((w // 2 - 120, 8), title, fill=(220, 220, 220), font=font)
    y = bar + pad
    for l in lines:
        col = (120, 220, 120) if l.startswith("$") else (230, 230, 230)
        d.text((pad, y), l, fill=col, font=font)
        y += lh
    img.save(path)

# ---------------- terminal screenshots ----------------
for q, f in SRC.items():
    inp = open(f"{ROOT}/input/{q}.txt").read().rstrip("\n").splitlines()
    out = subprocess.run([f"{ROOT}/bin/{q}"], stdin=open(f"{ROOT}/input/{q}.txt"),
                         capture_output=True, text=True).stdout
    open(f"{ROOT}/output/{q}.txt", "w").write(out)
    lines = [f"$ gcc -O2 -o {q} {f}.c", f"$ cat input/{q}.txt"] + inp + \
            [f"$ ./{q} < input/{q}.txt"] + out.rstrip("\n").splitlines()
    terminal_png(lines, f"{ROOT}/screenshots/{q}_terminal.png", f"{q}: terminal")

# ---------------- runtime graphs ----------------
random.seed(1)

def gen(q, n):
    if q == "q1":
        m = 2 * n
        return f"{n} {m}\n" + "".join(f"{random.randint(1,n)} {random.randint(1,n)}\n" for _ in range(m))
    if q == "q2":
        s = int((2 * n) ** 0.5) + 1
        return f"{s} {s} {n}\n" + "".join(f"{random.randrange(s)} {random.randrange(s)}\n" for _ in range(n))
    if q == "q3":
        w = " ".join(str(random.randint(0, 100)) for _ in range(n))
        return f"{n} {max(1, n // 10)}\n{w}\n{n}\n" + "".join(
            f"{random.randint(1,n)} {random.randint(1,n)} {random.randint(1,9)}\n" for _ in range(n))
    if q == "q4":
        w = " ".join(str(random.randint(0, 100)) for _ in range(n))
        order = list(range(1, n + 1)); random.shuffle(order)
        return f"{n} {2*n}\n{w}\n" + "".join(
            f"{random.randint(1,n)} {random.randint(1,n)}\n" for _ in range(2 * n)) + " ".join(map(str, order)) + "\n"
    A = list(range(1, n + 1)); random.shuffle(A)
    return f"{n}\n{' '.join(map(str, A))}\n{n}\n" + "".join(
        f"{random.randint(1,n)} {random.randint(1,n)}\n" for _ in range(n))

SIZES = {q: [100000, 200000, 400000, 800000, 1600000] for q in SRC}
SIZES["q3"] = [1000, 2000, 4000, 8000, 16000]
EXPECT = {"q1": "O((V+E) α(V))", "q2": "O(R·C + K α)", "q3": "O(E·V)",
          "q4": "O((V+E) α(V))", "q5": "O(n log n + m α)"}

for q in SRC:
    xs, ys = [], []
    for n in SIZES[q]:
        data = gen(q, n)
        best = 1e9
        for _ in range(3):
            t = time.perf_counter()
            subprocess.run([f"{ROOT}/bin/{q}"], input=data, text=True, stdout=subprocess.DEVNULL)
            best = min(best, time.perf_counter() - t)
        xs.append(n); ys.append(best * 1000)
        print(q, n, f"{best*1000:.1f} ms")
    fig, ax = plt.subplots(figsize=(7, 4.2), dpi=130)
    ax.plot(xs, ys, "o-", color="#2a6fdb", label="measured")
    if q == "q3":
        ref = [ys[0] * (x / xs[0]) ** 2 for x in xs]; lab = "c·N² reference"
    elif q == "q5":
        ref = [ys[0] * (x * (x ** 0.5 and __import__('math').log2(x))) / (xs[0] * __import__('math').log2(xs[0])) for x in xs]; lab = "c·N log N reference"
    else:
        ref = [ys[0] * x / xs[0] for x in xs]; lab = "c·N reference"
    ax.plot(xs, ref, "--", color="#999", label=lab)
    ax.set_xlabel("input size N" + (" (vertices = edges)" if q == "q3" else ""))
    ax.set_ylabel("running time (ms)")
    ax.set_title(f"{TITLE[q]}  —  {EXPECT[q]}")
    ax.grid(alpha=.3); ax.legend()
    fig.tight_layout()
    fig.savefig(f"{ROOT}/graphs/{q}_runtime.png")
    plt.close(fig)
