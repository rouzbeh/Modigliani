#!/usr/bin/env python3
"""Smoke test: run an example and assert that it produced an action potential.

The simulator can fail in ways neither compiling nor the unit tests notice -
channels silently not attached, a step algorithm quietly discarding its
timestep, a diverging integration writing NaNs. All of those show up in the
recording, so check the recording.

Uses examples/sga_suprathreshold.json, which fires under every algorithm.
examples/sga.json is deliberately left sitting at threshold and only fires
about eight times in ten, so it is not something CI can assert on.

Usage:
    check_spike.py <simulator> [config.json] [extra sim args...]
"""

import array
import glob
import os
import struct
import sys

sys.path.insert(0, os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "doc"))

import run_example  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_CONFIG = os.path.join(REPO, "examples", "sga_suprathreshold.json")

# What a squid axon at 6.3 C does when kicked hard enough at t = 30 ms.
MIN_PEAK_MV = 20.0        # the spike must actually overshoot
MAX_PEAK_MV = 80.0        # ... but not diverge
MAX_REST_MV = -50.0       # resting potential must stay hyperpolarised
MIN_TROUGH_MV = -100.0    # afterhyperpolarisation must stay physiological
EXPECTED_COLUMNS = 4      # Vm + leak + sodium + potassium


def check(run_dir):
    print("checking %s" % run_dir)

    recordings = sorted(glob.glob(os.path.join(run_dir, "compartments",
                                               "*.bin")))
    if not recordings:
        sys.exit("no compartment recording in %s" % run_dir)

    blob = open(recordings[0], "rb").read()
    columns = struct.unpack("I", blob[:4])[0]

    if columns != EXPECTED_COLUMNS:
        sys.exit("expected %d columns (Vm + 3 currents), got %d - a current "
                 "was probably not attached" % (EXPECTED_COLUMNS, columns))

    values = array.array("f")
    payload = blob[4:]
    values.frombytes(payload[: len(payload) // (4 * columns) * 4 * columns])
    rows = len(values) // columns

    if rows < 1000:
        sys.exit("only %d samples recorded, expected thousands" % rows)

    voltage = [values[i * columns] for i in range(rows)]

    for index, value in enumerate(voltage):
        if value != value:
            sys.exit("membrane potential is NaN at sample %d" % index)

    peak, trough, rest = max(voltage), min(voltage), voltage[0]
    print("samples=%d  rest=%.2f mV  peak=%.2f mV  trough=%.2f mV"
          % (rows, rest, peak, trough))

    problems = []
    if rest > MAX_REST_MV:
        problems.append("resting potential %.2f mV is above %.1f mV"
                        % (rest, MAX_REST_MV))
    if peak < MIN_PEAK_MV:
        problems.append("peak %.2f mV never reached %.1f mV - the axon did "
                        "not spike" % (peak, MIN_PEAK_MV))
    if peak > MAX_PEAK_MV:
        problems.append("peak %.2f mV exceeds %.1f mV - the integration "
                        "looks unstable" % (peak, MAX_PEAK_MV))
    if trough < MIN_TROUGH_MV:
        problems.append("trough %.2f mV is below %.1f mV"
                        % (trough, MIN_TROUGH_MV))

    if problems:
        for problem in problems:
            print("FAIL: %s" % problem, file=sys.stderr)
        sys.exit(1)

    print("OK: the axon fired an action potential")


def main(argv):
    if len(argv) < 2:
        raise SystemExit(__doc__.strip())

    simulator = argv[1]
    config = argv[2] if len(argv) > 2 else DEFAULT_CONFIG
    extra = argv[3:]

    import tempfile
    with tempfile.TemporaryDirectory() as outdir:
        check(run_example.run(simulator, config, outdir, extra))


if __name__ == "__main__":
    main(sys.argv)
