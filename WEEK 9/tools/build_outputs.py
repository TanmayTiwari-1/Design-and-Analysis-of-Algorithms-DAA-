#!/usr/bin/env python3
"""Compile every program, run the sample + self-test, and regenerate
output.txt, terminal_output.png and the 'Sample run' block of each README.

usage:  python3 tools/build_outputs.py          (from the DAA_Lab09 folder)
needs:  gcc, Python 3, Pillow
"""
import glob
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from render_terminal import render  # noqa: E402


def run(cmd, cwd, stdin=None):
    r = subprocess.run(cmd, shell=True, cwd=cwd, capture_output=True, text=True,
                       stdin=open(stdin) if stdin else None, timeout=300)
    return (r.stdout + r.stderr).rstrip("\n"), r.returncode


def main():
    bad = 0
    for qdir in sorted(glob.glob(os.path.join(ROOT, "Q[0-9][0-9]_*"))):
        name = os.path.basename(qdir)
        src = os.path.basename(glob.glob(os.path.join(qdir, "*.c"))[0])
        exe = src.split("_")[0]
        t = []

        cmd = f"gcc -O2 -Wall -Wextra -o {exe} {src} -lm"
        out, rc = run(cmd, qdir)
        t.append("$ " + cmd)
        if out:
            t.append(out)
        bad += rc != 0

        t.append("$ cat sample_input.txt")
        t.append(open(os.path.join(qdir, "sample_input.txt")).read().rstrip("\n"))

        cmd = f"./{exe} < sample_input.txt"
        out, rc = run(cmd, qdir, stdin=os.path.join(qdir, "sample_input.txt"))
        t.append("$ " + cmd)
        t.append(out)
        bad += rc != 0

        cmd = f"./{exe} --test"
        out, rc = run(cmd, qdir)
        t.append("$ " + cmd)
        t.append(out)
        bad += rc != 0

        text = "\n".join(t) + "\n"
        with open(os.path.join(qdir, "output.txt"), "w") as f:
            f.write(text)
        render(os.path.join(qdir, "output.txt"), os.path.join(qdir, "terminal_output.png"),
               f"{name} - terminal")

        readme = os.path.join(qdir, "README.md")
        block = ("<!-- SAMPLE_RUN_START -->\n```text\n" + text + "```\n\n"
                 "![Terminal output](terminal_output.png)\n<!-- SAMPLE_RUN_END -->")
        md = open(readme).read()
        md = re.sub(r"<!-- SAMPLE_RUN_START -->.*?<!-- SAMPLE_RUN_END -->",
                    lambda m: block, md, flags=re.S)
        open(readme, "w").write(md)

        os.remove(os.path.join(qdir, exe))          # keep the folders source-only
        print(f"{name}: ok" if rc == 0 else f"{name}: NON-ZERO EXIT")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
