#!/usr/bin/env python3
"""Run one of the bundled examples into a directory of our choosing.

The examples write to whatever `simulation_parameters.outputFolder` says,
and the simulator has to run with its working directory set to the example
folder so that the relative Lua `require` paths resolve. This wraps both
details up so the documentation build and the CI smoke test can share them.

Prints the path of the run directory that was produced.

Usage:
    run_example.py <simulator> <config.json> <output-dir> [extra sim args...]
"""

import json
import os
import re
import subprocess
import sys
import tempfile


def run(simulator, config, outdir, extra=()):
    outdir = os.path.abspath(outdir)
    config = os.path.abspath(config)
    simulator = os.path.abspath(simulator)
    workdir = os.path.dirname(config)

    os.makedirs(outdir, exist_ok=True)

    # Point the copy at our output directory. The configuration is
    # hand-formatted, so rewrite the one key rather than reserialising it.
    text = open(config).read()
    text, count = re.subn(r'("outputFolder"\s*:\s*)"(?:[^"\\]|\\.)*"',
                          lambda m: m.group(1) + json.dumps(outdir + "/"),
                          text)
    if count != 1:
        raise SystemExit("%s: expected exactly one outputFolder, found %d"
                         % (config, count))

    handle, temp_config = tempfile.mkstemp(suffix=".json", dir=workdir)
    try:
        with os.fdopen(handle, "w") as out:
            out.write(text)

        command = [simulator, "--config-file", temp_config] + list(extra)
        # The simulator chats on stdout, and callers capture our stdout to
        # get the run directory. Send its output to stderr so the two do
        # not get mixed together.
        result = subprocess.run(command, cwd=workdir, stdout=sys.stderr)

        if result.returncode != 0:
            raise SystemExit("%s exited with %d"
                             % (os.path.basename(simulator),
                                result.returncode))
    finally:
        os.unlink(temp_config)

    runs = [os.path.join(outdir, name) for name in os.listdir(outdir)]
    runs = [path for path in runs if os.path.isdir(path)]

    if not runs:
        raise SystemExit("no run directory appeared under %s" % outdir)

    return max(runs, key=os.path.getmtime)


def main(argv):
    if len(argv) < 4:
        raise SystemExit(__doc__.strip())
    print(run(argv[1], argv[2], argv[3], argv[4:]))


if __name__ == "__main__":
    main(sys.argv)
