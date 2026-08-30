#!/usr/bin/env python3
"""Render a Modigliani compartment recording as an SVG figure.

Reads the binary file written by Membrane_compartment::WriteOutput() - a
modigliani::Size column count followed by rows of 32-bit floats, the first
column being the membrane potential and the rest the individual currents -
and emits a two panel SVG: membrane potential on top, currents below.

Deliberately dependency free (standard library only) so that building the
documentation needs nothing beyond python3.

Usage:
    plot_trace.py <compartment.bin> <time.txt> <out.svg> [label ...]
"""

import array
import struct
import sys

# Panel geometry, in SVG user units.
WIDTH = 760
PANEL_H = 210
PAD_L, PAD_R, PAD_T, PAD_B = 62, 14, 26, 34
GAP = 46

# Colour-blind safe. Two sets: the second is brightened for dark backgrounds,
# selected by a prefers-color-scheme query inside the SVG.
COLOURS = ["#1f78b4", "#e31a1c", "#33a02c", "#ff7f00", "#6a3d9a", "#b15928"]
COLOURS_DARK = ["#6ab0e8", "#ff6b6b", "#5fd35f", "#ffab4d", "#b088e0",
                "#d9a066"]


def read_recording(path):
    """Return (columns, rows) where rows is a list of per-sample tuples."""
    with open(path, "rb") as handle:
        blob = handle.read()

    if len(blob) < 4:
        raise SystemExit("%s: too short to contain a column count" % path)

    columns = struct.unpack("I", blob[:4])[0]

    if columns == 0 or columns > 4096:
        raise SystemExit("%s: implausible column count %d" % (path, columns))

    values = array.array("f")
    payload = blob[4:]
    values.frombytes(payload[: len(payload) // (4 * columns) * 4 * columns])
    rows = len(values) // columns

    if rows == 0:
        raise SystemExit("%s: no samples" % path)

    return columns, [values[i * columns:(i + 1) * columns] for i in range(rows)]


def read_times(path):
    """Time.txt is one millisecond value per line after a '%' comment."""
    times = []
    with open(path) as handle:
        for line in handle:
            line = line.strip()
            if not line or line.startswith("%"):
                continue
            try:
                times.append(float(line))
            except ValueError:
                pass
    return times


def nice_ticks(low, high, count=5):
    """Pick round tick values spanning [low, high]."""
    if high <= low:
        high = low + 1.0
    raw = (high - low) / count
    magnitude = 10.0 ** (len(str(int(abs(raw)))) - 1 if abs(raw) >= 1 else -1)
    for candidate in (1, 2, 2.5, 5, 10):
        step = candidate * magnitude
        if step >= raw:
            break
    # Only keep ticks that actually fall inside the panel, otherwise the
    # outermost label is drawn beyond the frame.
    ticks = []
    value = step * (int(low / step) - 1)
    while value <= high:
        if value >= low:
            ticks.append(value)
        value += step
    return ticks, step


def fmt(value, step):
    if step >= 1:
        return "%d" % round(value)
    if step >= 0.1:
        return "%.1f" % value
    if step >= 0.01:
        return "%.2f" % value
    return "%.3f" % value


class Panel(object):
    """One set of axes: maps data coordinates to SVG coordinates."""

    def __init__(self, top, xlo, xhi, ylo, yhi):
        self.top = top
        self.xlo, self.xhi = xlo, xhi
        pad = (yhi - ylo) * 0.08 or 1.0
        self.ylo, self.yhi = ylo - pad, yhi + pad

    @property
    def left(self):
        return PAD_L

    @property
    def right(self):
        return WIDTH - PAD_R

    @property
    def bottom(self):
        return self.top + PANEL_H

    def sx(self, x):
        span = self.xhi - self.xlo or 1.0
        return self.left + (x - self.xlo) / span * (self.right - self.left)

    def sy(self, y):
        span = self.yhi - self.ylo or 1.0
        return self.bottom - (y - self.ylo) / span * (self.bottom - self.top)


def draw_axes(out, panel, xlabel, ylabel, show_x):
    xticks, xstep = nice_ticks(panel.xlo, panel.xhi)
    yticks, ystep = nice_ticks(panel.ylo, panel.yhi)

    for value in xticks:
        x = panel.sx(value)
        out.append('<line x1="%.1f" y1="%.1f" x2="%.1f" y2="%.1f" '
                   'class="grid"/>' % (x, panel.top, x, panel.bottom))
        if show_x:
            out.append('<text x="%.1f" y="%.1f" class="tick mid">%s</text>'
                       % (x, panel.bottom + 15, fmt(value, xstep)))

    for value in yticks:
        y = panel.sy(value)
        out.append('<line x1="%.1f" y1="%.1f" x2="%.1f" y2="%.1f" '
                   'class="grid"/>' % (panel.left, y, panel.right, y))
        out.append('<text x="%.1f" y="%.1f" class="tick end">%s</text>'
                   % (panel.left - 7, y + 3.5, fmt(value, ystep)))

    out.append('<rect x="%.1f" y="%.1f" width="%.1f" height="%.1f" '
               'class="frame"/>'
               % (panel.left, panel.top, panel.right - panel.left, PANEL_H))

    if show_x:
        out.append('<text x="%.1f" y="%.1f" class="axis mid">%s</text>'
                   % ((panel.left + panel.right) / 2, panel.bottom + 30,
                      xlabel))
    out.append('<text transform="translate(%.1f,%.1f) rotate(-90)" '
               'class="axis mid">%s</text>'
               % (16, (panel.top + panel.bottom) / 2, ylabel))


def polyline(panel, times, values, index):
    points = " ".join("%.2f,%.2f" % (panel.sx(t), panel.sy(v))
                      for t, v in zip(times, values))
    return '<polyline points="%s" class="s%d"/>' % (points, index)


def main(argv):
    if len(argv) < 4:
        raise SystemExit(__doc__.strip())

    binary, timefile, target = argv[1], argv[2], argv[3]
    labels = argv[4:]

    columns, rows = read_recording(binary)
    times = read_times(timefile)

    count = min(len(rows), len(times)) if times else len(rows)
    if not times:
        times = [float(i) for i in range(count)]
    rows, times = rows[:count], times[:count]

    voltage = [row[0] for row in rows]
    currents = [[row[c] for row in rows] for c in range(1, columns)]

    while len(labels) < columns - 1:
        labels.append("current %d" % (len(labels) + 1))

    top = Panel(PAD_T, times[0], times[-1], min(voltage), max(voltage))
    if currents:
        flat = [v for series in currents for v in series]
        bottom = Panel(PAD_T + PANEL_H + GAP, times[0], times[-1],
                       min(flat), max(flat))
        height = bottom.bottom + PAD_B
    else:
        bottom, height = None, top.bottom + PAD_B

    out = []
    out.append('<svg xmlns="http://www.w3.org/2000/svg" width="%d" '
               'height="%d" viewBox="0 0 %d %d" '
               'font-family="Helvetica,Arial,sans-serif">'
               % (WIDTH, int(height), WIDTH, int(height)))
    series = "".join(
        ".s%d{stroke:%s}" % (i, c) for i, c in enumerate(COLOURS))
    series_dark = "".join(
        ".s%d{stroke:%s}" % (i, c) for i, c in enumerate(COLOURS_DARK))

    out.append("<style>"
               "polyline{fill:none;stroke-width:1.4}"
               ".bg{fill:#ffffff}"
               ".grid{stroke:#d8d8d8;stroke-width:.6}"
               ".frame{fill:none;stroke:#666;stroke-width:1}"
               ".tick{font-size:10px;fill:#444}"
               ".axis{font-size:11px;fill:#222}"
               ".mid{text-anchor:middle}.end{text-anchor:end}"
               ".key{font-size:10px;fill:#222}"
               + series +
               "@media (prefers-color-scheme:dark){"
               ".bg{fill:none}"
               ".grid{stroke:#3c3c3c}"
               ".frame{stroke:#8a8a8a}"
               ".tick{fill:#a8a8a8}"
               ".axis{fill:#d0d0d0}"
               ".key{fill:#d0d0d0}"
               + series_dark +
               "}"
               "</style>")
    out.append('<rect width="100%" height="100%" class="bg"/>')

    draw_axes(out, top, "time (ms)", "membrane potential (mV)",
              bottom is None)
    out.append(polyline(top, times, voltage, 0))

    if bottom is not None:
        draw_axes(out, bottom, "time (ms)", "current (nA)", True)
        for index, series in enumerate(currents):
            out.append(polyline(bottom, times, series,
                                (index + 1) % len(COLOURS)))

        x = bottom.left + 10
        y = bottom.top + 14
        for index, label in enumerate(labels[:len(currents)]):
            out.append('<line x1="%.1f" y1="%.1f" x2="%.1f" y2="%.1f" '
                       'class="s%d" stroke-width="2.2"/>'
                       % (x, y - 3.5, x + 16, y - 3.5,
                          (index + 1) % len(COLOURS)))
            out.append('<text x="%.1f" y="%.1f" class="key">%s</text>'
                       % (x + 21, y, label))
            x += 26 + 7 * len(label)

    out.append("</svg>")

    with open(target, "w") as handle:
        handle.write("\n".join(out) + "\n")

    print("%s: %d samples, %.2f-%.2f ms, Vm %.2f..%.2f mV"
          % (target, count, times[0], times[-1], min(voltage), max(voltage)))


if __name__ == "__main__":
    main(sys.argv)
