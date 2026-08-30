# Modigliani
[![CI](https://github.com/rouzbeh/Modigliani/actions/workflows/ci.yml/badge.svg)](https://github.com/rouzbeh/Modigliani/actions/workflows/ci.yml)


Modigliani is a set of libraries for stochastic simulations of neurons. Modigliani was first started at the University of Cambridge by Aldo Faisal.

From 2010 to 2015, Modigliani was developed and maintained by Ali Neishabouri at Imperial College London.

## Documentation
Doxygen generated documentation is available at https://rouzbeh.github.io/Modigliani

It is rebuilt and republished automatically on every push to `master`. Start
with the [squid giant axon walkthrough](https://rouzbeh.github.io/Modigliani/sga_example.html),
which covers running the bundled example and reading its output.

To build the documentation locally you need `doxygen`, `graphviz` and
`python3`:

    ./doc/build_docs.sh

The result is written to `doc/html/index.html`.

## Installing on Mac via homebrew
Run `brew tap rouzbeh/tap` to add my collection of homebrew formulae.

Then `brew install Modigliani` to install Modigliani and its dependencies.

## Dependencies
Boost (filesystem, program\_options, random, system), Lua 5.2 or later, and
the autotools. PLplot is optional; if `pkg-config` finds it, live plotting is
compiled in.

On Debian/Ubuntu:

    sudo apt-get install autoconf automake libtool pkg-config \
        libboost-filesystem-dev libboost-program-options-dev \
        libboost-random-dev libboost-system-dev liblua5.4-dev lua5.4

On macOS:

    brew install autoconf automake libtool pkg-config boost lua

## Build instructions
Run `autoreconf -f -v -i`

Optional: Create a folder somewhere. Compiled binaries will go there.

Optional: Go to the newly created folder.

Run the configure script `./configure`

Run `make -j`
