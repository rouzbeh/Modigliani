#!/bin/sh
#
# Build the Modigliani documentation, including a figure generated from a
# real run of the squid giant axon example.
#
# Steps: build the simulator if it is not already built, run examples/sga.json,
# turn the recording into an SVG, then run doxygen over src/ and doc/pages/.
#
# Needs: a configured build tree (or autotools to make one), python3, doxygen.
# Output lands in doc/html/.
#
# Usage: ./doc/build_docs.sh [path-to-build-tree]

set -e

root=$(cd "$(dirname "$0")/.." && pwd)
build=${1:-$root}
images="$root/doc/images"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

echo "==> project root: $root"
echo "==> build tree:   $build"

# ---------------------------------------------------------------- build ----
sim="$build/src/auxsrc/modigliani"

if [ ! -x "$sim" ]; then
  echo "==> building the simulator"
  if [ ! -f "$build/Makefile" ]; then
    [ -f "$root/configure" ] || (cd "$root" && autoreconf -f -v -i)
    mkdir -p "$build"
    (cd "$build" && "$root/configure")
  fi
  make -C "$build" -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
fi

[ -x "$sim" ] || { echo "no simulator at $sim" >&2; exit 1; }

# ------------------------------------------------------------- simulate ----
# The figure uses the suprathreshold example, which fires on every run.
# examples/sga.json sits at threshold and fires about eight times in ten.
echo "==> running the SGA example"
run=$(python3 "$root/doc/run_example.py" \
        "$sim" "$root/examples/sga_suprathreshold.json" "$work/out")
[ -n "$run" ] || { echo "the simulation produced no output" >&2; exit 1; }
echo "==> recording: $run"

# ----------------------------------------------------------------- plot ----
echo "==> plotting"
mkdir -p "$images"
python3 "$root/doc/plot_trace.py" \
  "$run/compartments/compartment_0.bin" \
  "$run/Time.txt" \
  "$images/sga_trace.svg" \
  leak sodium potassium

# --------------------------------------------------------------- doxygen ---
echo "==> doxygen"
(cd "$root" && doxygen Modigliani.Doxyfile)

echo "==> done: $root/doc/html/index.html"
