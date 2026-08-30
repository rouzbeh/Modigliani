#!/usr/bin/env python3
"""Assert that runs are independent by default and repeatable on demand.

Both properties matter for Monte Carlo work: a batch of trials launched
together must not collapse into copies of a single trial, and a result worth
keeping must be reproducible. Checked from both sides:

  * the same --seed twice must give identical recordings, and
  * two unseeded runs, started back to back, must not.

Usage:
    check_reproducible.py <simulator>
"""

import hashlib
import os
import sys
import tempfile

sys.path.insert(0, os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "doc"))

import run_example  # noqa: E402

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CONFIG = os.path.join(REPO, "examples", "sga_suprathreshold.json")


def digest(simulator, extra):
    with tempfile.TemporaryDirectory() as outdir:
        run = run_example.run(simulator, CONFIG, outdir, extra)
        path = os.path.join(run, "compartments", "compartment_0.bin")
        if not os.path.exists(path):
            sys.exit("no recording at %s" % path)
        return hashlib.sha256(open(path, "rb").read()).hexdigest()


def main(argv):
    if len(argv) < 2:
        raise SystemExit(__doc__.strip())
    simulator = argv[1]

    seeded = [digest(simulator, ["--seed", "20240101"]) for _ in range(2)]
    print("--seed 20240101 : %s" % seeded[0][:32])
    print("--seed 20240101 : %s" % seeded[1][:32])

    if seeded[0] != seeded[1]:
        sys.exit("FAIL: the same --seed produced different output")

    unseeded = [digest(simulator, []) for _ in range(2)]
    print("no seed         : %s" % unseeded[0][:32])
    print("no seed         : %s" % unseeded[1][:32])

    if unseeded[0] == unseeded[1]:
        sys.exit("FAIL: two unseeded runs were identical - the generators "
                 "are not being seeded independently")

    print("OK: --seed reproduces, unseeded runs differ")


if __name__ == "__main__":
    main(sys.argv)
