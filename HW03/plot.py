"""Plots the HW03 timing sweeps written by run_local.sh (or the Slurm jobs).

Usage: python plot.py

Each data file has lines of "x time_ms". Produces task1.pdf, task2.pdf,
task3_ts.pdf, and task3_t.pdf.
"""
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt  # noqa: E402
from matplotlib.ticker import FixedLocator, FuncFormatter  # noqa: E402

SERIES = "#2a78d6"
INK = "#0b0b0b"
INK_SECONDARY = "#52514e"
GRID = "#e1e0d9"
AXIS = "#c3c2b7"
CORES_NOTE = "Apple M4 (4 performance + 6 efficiency cores), median of 5 runs"


def read_runs(path):
    xs, times = [], []
    with open(path) as f:
        for line in f:
            fields = line.split()
            if fields:
                xs.append(int(fields[0]))
                times.append(float(fields[1]))
    return xs, times


def plot(data_path, plot_path, title, xlabel, ylabel, log_x=False):
    xs, times = read_runs(data_path)

    plt.rcParams.update(
        {"pdf.fonttype": 42, "svg.fonttype": "path", "font.size": 10})
    fig, ax = plt.subplots(figsize=(7.0, 4.6))

    ax.plot(xs, times, color=SERIES, lw=1.75, marker="o", ms=6, mec="white",
            mew=1.25, solid_joinstyle="round", solid_capstyle="round", zorder=3)

    if log_x:
        ax.set_xscale("log", base=2)
        ax.xaxis.set_major_locator(FixedLocator(xs))
        ax.xaxis.set_major_formatter(
            FuncFormatter(lambda v, _: f"$2^{{{round(v).bit_length() - 1}}}$"))
        ax.xaxis.set_minor_locator(FixedLocator([]))
    else:
        ax.xaxis.set_major_locator(FixedLocator(xs))
    ax.set_ylim(bottom=0)
    ax.set_xlabel(xlabel, color=INK_SECONDARY)
    ax.set_ylabel(ylabel, color=INK_SECONDARY)

    best = min(range(len(xs)), key=times.__getitem__)
    ax.annotate(f"min {times[best]:.3g} ms", xy=(xs[best], times[best]),
                xytext=(0, 10), textcoords="offset points", ha="center",
                va="bottom", color=INK_SECONDARY, fontsize=9)

    ax.grid(True, which="major", color=GRID, lw=0.6)
    ax.set_axisbelow(True)
    for side in ("top", "right"):
        ax.spines[side].set_visible(False)
    for side in ("left", "bottom"):
        ax.spines[side].set_color(AXIS)
    ax.tick_params(which="both", colors=INK_SECONDARY)

    ax.set_title(title, loc="left", color=INK, fontsize=12, pad=20)
    ax.text(0, 1.02, CORES_NOTE, transform=ax.transAxes, ha="left",
            va="bottom", color=INK_SECONDARY, fontsize=8.5)

    fig.savefig(plot_path, bbox_inches="tight")
    plt.close(fig)


def main():
    threads = "Number of threads t"
    plot("task1.out", "task1.pdf",
         "Task 1: parallel mmul time vs. threads (n = 1024)", threads,
         "mmul time (ms)")
    plot("task2.out", "task2.pdf",
         "Task 2: parallel convolve time vs. threads (n = 1024, m = 3)",
         threads, "convolve time (ms)")
    plot("task3_ts.out", "task3_ts.pdf",
         "Task 3: msort time vs. threshold (n = $10^6$, t = 8)",
         "Threshold ts", "msort time (ms)", log_x=True)
    plot("task3_t.out", "task3_t.pdf",
         "Task 3: msort time vs. threads (n = $10^6$, ts = 16)", threads,
         "msort time (ms)")


if __name__ == "__main__":
    main()
