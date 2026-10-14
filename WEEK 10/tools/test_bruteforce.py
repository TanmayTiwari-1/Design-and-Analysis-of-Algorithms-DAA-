"""Cross-check every C program against a naive Python brute force on random inputs."""
import random, subprocess, sys
from collections import deque

BIN = "/home/claude/lab10/bin"

def run(name, inp):
    return subprocess.run([f"{BIN}/{name}"], input=inp, capture_output=True, text=True).stdout

def comps(n, edges, alive=None):
    adj = {i: [] for i in range(1, n + 1)}
    for u, v in edges:
        if alive is None or (u in alive and v in alive):
            adj[u].append(v); adj[v].append(u)
    seen, out = set(), []
    for s in range(1, n + 1):
        if (alive is not None and s not in alive) or s in seen: continue
        q, c = deque([s]), [s]; seen.add(s)
        while q:
            x = q.popleft()
            for y in adj[x]:
                if y not in seen: seen.add(y); q.append(y); c.append(y)
        out.append(c)
    return out

def is_bip(n, edges):
    adj = {i: [] for i in range(1, n + 1)}
    for u, v in edges:
        adj[u].append(v); adj[v].append(u)
    col = {}
    for s in range(1, n + 1):
        if s in col: continue
        col[s] = 0; q = deque([s])
        while q:
            x = q.popleft()
            for y in adj[x]:
                if y not in col: col[y] = col[x] ^ 1; q.append(y)
                elif col[y] == col[x]: return False
    return True

def t1():
    n = random.randint(1, 8); m = random.randint(0, 15)
    es = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    inp = f"{n} {m}\n" + "".join(f"{u} {v}\n" for u, v in es)
    got = [l.split()[-1] for l in run("q1", inp).splitlines() if l.startswith("Edge")]
    cur, exp = [], []
    for e in es:
        if is_bip(n, cur + [e]): cur.append(e); exp.append("ADDED")
        else: exp.append("REJECTED")
    return got == exp

def t2():
    R, C = random.randint(1, 5), random.randint(1, 5); k = random.randint(1, 15)
    ops = [(random.randrange(R), random.randrange(C)) for _ in range(k)]
    inp = f"{R} {C} {k}\n" + "".join(f"{r} {c}\n" for r, c in ops)
    got = [int(l.split()[-1]) for l in run("q2", inp).splitlines()]
    land, exp = set(), []
    for r, c in ops:
        land.add((r, c)); seen = set(); cnt = 0
        for s in land:
            if s in seen: continue
            cnt += 1; st = [s]; seen.add(s)
            while st:
                x, y = st.pop()
                for nb in ((x+1, y), (x-1, y), (x, y+1), (x, y-1)):
                    if nb in land and nb not in seen: seen.add(nb); st.append(nb)
        exp.append(cnt)
    return got == exp

def t3():
    n = random.randint(1, 8); K = random.randint(1, n); m = random.randint(1, 12)
    w = [random.randint(0, 20) for _ in range(n)]
    es = [(random.randint(1, n), random.randint(1, n), random.randint(1, 9)) for _ in range(m)]
    inp = f"{n} {K}\n{' '.join(map(str, w))}\n{m}\n" + "".join(f"{u} {v} {c}\n" for u, v, c in es)
    got = [int(l.split()[-1]) for l in run("q3", inp).splitlines()]
    exp = []
    for i in range(1, m + 1):
        best = 0
        for c in comps(n, [(u, v) for u, v, _ in es[:i]]):
            if len(c) <= K: best = max(best, sum(w[x - 1] for x in c))
        exp.append(best)
    return got == exp

def t4():
    n = random.randint(1, 8); m = random.randint(0, 12)
    w = [random.randint(0, 20) for _ in range(n)]
    es = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    order = random.sample(range(1, n + 1), n)
    inp = f"{n} {m}\n{' '.join(map(str, w))}\n" + "".join(f"{u} {v}\n" for u, v in es) + " ".join(map(str, order)) + "\n"
    got = [int(l.split()[-1]) for l in run("q4", inp).splitlines()]
    exp = []
    for i in range(1, n + 1):
        alive = set(order[i:])
        best = 0
        for c in comps(n, es, alive): best = max(best, sum(w[x - 1] for x in c))
        exp.append(best)
    return got == exp

def t5():
    n = random.randint(1, 8); m = random.randint(0, 10)
    A = random.sample(range(1, n + 1), n)
    es = [(random.randint(1, n), random.randint(1, n)) for _ in range(m)]
    inp = f"{n}\n{' '.join(map(str, A))}\n{m}\n" + "".join(f"{u} {v}\n" for u, v in es)
    got = list(map(int, run("q5", inp).split()))
    exp = A[:]
    for c in comps(n, es):
        c.sort()
        vals = sorted(A[x - 1] for x in c)
        for pos, v in zip(c, vals): exp[pos - 1] = v
    return got == exp

random.seed(10)
ok = True
for name, f in [("q1", t1), ("q2", t2), ("q3", t3), ("q4", t4), ("q5", t5)]:
    bad = sum(0 if f() else 1 for _ in range(500))
    print(f"{name}: 500 random tests, {bad} mismatches")
    ok &= bad == 0
sys.exit(0 if ok else 1)
