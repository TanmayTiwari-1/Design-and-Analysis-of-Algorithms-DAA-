#!/usr/bin/env python3
"""Render a plain-text terminal transcript as a dark terminal-style PNG.

usage: render_terminal.py transcript.txt out.png "window title"
Lines that start with "$ " are drawn as shell prompts.
"""
import sys
from PIL import Image, ImageDraw, ImageFont

FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"
BOLD = "/usr/share/fonts/truetype/dejavu/DejaVuSansMono-Bold.ttf"

BG, BAR, FG = "#1e1e1e", "#323233", "#d4d4d4"
GREEN, RED, CYAN, GREY = "#6adf7a", "#f26b6b", "#6cc4e8", "#8a8a8a"


def render(text_path, png_path, title, cols=118, size=16):
    raw_lines = open(text_path, encoding="utf-8").read().rstrip("\n").split("\n")
    lines = []
    for raw in raw_lines:
        raw = raw.replace("\t", "    ")
        if len(raw) <= cols:
            lines.append(raw)
        else:
            for i in range(0, len(raw), cols):
                lines.append(raw[i:i + cols])

    font = ImageFont.truetype(FONT, size)
    bold = ImageFont.truetype(BOLD, size)
    cw = font.getlength("M")
    lh = size + 6
    pad, bar = 20, 36
    width = int(pad * 2 + cw * max(max(len(l) for l in lines), 70))
    height = bar + pad * 2 + lh * len(lines)

    img = Image.new("RGB", (width, height), BG)
    d = ImageDraw.Draw(img)
    d.rectangle([0, 0, width, bar], fill=BAR)
    for i, c in enumerate(("#ff5f56", "#ffbd2e", "#27c93f")):
        d.ellipse([16 + i * 24, 12, 28 + i * 24, 24], fill=c)
    d.text((width / 2, bar / 2), title, font=font, fill="#cccccc", anchor="mm")

    y = bar + pad
    for l in lines:
        x = pad
        if l.startswith("$ "):
            prompt = "student@daa-lab:~$ "
            d.text((x, y), prompt, font=bold, fill=GREEN)
            d.text((x + cw * len(prompt), y), l[2:], font=bold, fill="#ffffff")
        elif l.startswith("RESULT: ALL TESTS PASSED"):
            d.text((x, y), l, font=bold, fill=GREEN)
        elif l.startswith("RESULT") or "FAIL" in l or "MISMATCH" in l or "NO" == l.strip()[-2:]:
            d.text((x, y), l, font=bold, fill=RED)
        elif "matches" in l or "= OK" in l or l.rstrip().endswith("= yes"):
            d.text((x, y), l, font=font, fill=GREEN)
        else:
            d.text((x, y), l, font=font, fill=FG)
        y += lh
    img.save(png_path)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        sys.exit(__doc__)
    render(sys.argv[1], sys.argv[2], sys.argv[3])
