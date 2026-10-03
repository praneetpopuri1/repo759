"""Plots the task 1 scaling analysis (scan time vs. n).

Usage: python plot_task1.py [task1.out] [task1.pdf]

Each data line of task1.out is "n time_ms first last", as written by task1.sh.
Lines starting with '#' are notes; the first one is printed under the title.
"""
import math
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
from matplotlib.ticker import (  # noqa: E402
    FixedLocator,
    FuncFormatter,
    NullFormatter,
    NullLocator,
)

SERIES = "#2a78d6"
INK = "#0b0b0b"
INK_SECONDARY = "#52514e"
GRID = "#e1e0d9"
AXIS = "#c3c2b7"


def read_runs(path):
    sizes, times, notes = [], [], []
    with open(path) as f:
        for line in f:
            line = line.strip()
            if line.startswith("#"):
                notes.append(line.lstrip("#").strip())
            elif line:
                fields = line.split()
                sizes.append(int(fields[0]))
                times.append(float(fields[1]))
    return sizes, times, notes


def power_of_two(value):
    return f"$2^{{{round(math.log2(value))}}}$"


def loglog_slope(sizes, times):
    """Least-squares slope of log(time) vs. log(n); 1.0 means time ~ n."""
    points = [(math.log(n), math.log(t)) for n, t in zip(sizes, times) if t > 0]
    mean_x = sum(x for x, _ in points) / len(points)
    mean_y = sum(y for _, y in points) / len(points)
    covariance = sum((x - mean_x) * (y - mean_y) for x, y in points)
    variance = sum((x - mean_x) ** 2 for x, _ in points)
    return covariance / variance


def main():
    data_path = sys.argv[1] if len(sys.argv) > 1 else "task1.out"
    plot_path = sys.argv[2] if len(sys.argv) > 2 else "task1.pdf"
    sizes, times, notes = read_runs(data_path)

    plt.rcParams.update(
        {"pdf.fonttype": 42, "svg.fonttype": "path", "font.size": 10})
    fig, ax = plt.subplots(figsize=(7.0, 4.6))

    ax.plot(sizes, times, color=SERIES, lw=1.75, marker="o", ms=6, mec="white",
            mew=1.25, solid_joinstyle="round", solid_capstyle="round", zorder=3)

    ax.set_xscale("log", base=2)
    ax.set_yscale("log")
    ax.xaxis.set_major_locator(FixedLocator(sizes[::2]))
    ax.xaxis.set_minor_locator(FixedLocator(sizes[1::2]))
    ax.xaxis.set_major_formatter(FuncFormatter(lambda v, _: power_of_two(v)))
    ax.xaxis.set_minor_formatter(NullFormatter())
    ax.yaxis.set_major_formatter(FuncFormatter(lambda v, _: f"{v:g}"))
    ax.yaxis.set_minor_locator(NullLocator())
    ax.set_xlabel("Array length n", color=INK_SECONDARY)
    ax.set_ylabel("Scan time (ms)", color=INK_SECONDARY)

    ax.grid(True, which="major", color=GRID, lw=0.6)
    ax.set_axisbelow(True)
    for side in ("top", "right"):
        ax.spines[side].set_visible(False)
    for side in ("left", "bottom"):
        ax.spines[side].set_color(AXIS)
    ax.tick_params(which="both", colors=INK_SECONDARY)
    ax.tick_params(which="minor", length=2)

    ax.annotate(f"{times[-1]:,.0f} ms at n = {power_of_two(sizes[-1])}",
                xy=(sizes[-1], times[-1]), xytext=(-10, 0),
                textcoords="offset points", ha="right", va="center",
                color=INK_SECONDARY, fontsize=9)
    ax.text(0.98, 0.04,
            f"Least-squares log-log slope: {loglog_slope(sizes, times):.2f}"
            " (1.00 = linear in n)",
            transform=ax.transAxes, ha="right", va="bottom",
            color=INK_SECONDARY, fontsize=9)

    ax.set_title("Task 1: inclusive scan time vs. array length", loc="left",
                 color=INK, fontsize=12, pad=20 if notes else 8)
    if notes:
        ax.text(0, 1.02, notes[0], transform=ax.transAxes, ha="left",
                va="bottom", color=INK_SECONDARY, fontsize=8.5)

    fig.savefig(plot_path, bbox_inches="tight")


if __name__ == "__main__":
    main()
