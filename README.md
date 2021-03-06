# Modigliani
[![Build Status](https://travis-ci.org/rouzbeh/Modigliani.svg?branch=master)](https://travis-ci.org/rouzbeh/Modigliani)


Modigliani is a set of libraries for stochastic simulations of neurons. Modigliani was first started at the University of Cambridge by Aldo Faisal.

From 2010 to 2015, Modigliani was developed and maintained by Ali Neishabouri at Imperial College London.

## Documentation
Doxygen generated documentation is available at http://rouzbeh.github.io/Modigliani

## Installing on Mac via homebrew
Run `brew tap rouzbeh/tap` to add my collection of homebrew formulae.

Then `brew install Modigliani` to install Modigliani and its dependencies.

## Build instructions
Run `autoreconf -f -v -i`

Optional: Create a folder somewhere. Compiled binaries will go there.

Optional: Go to the newly created folder.

Run the configure script `./configure`

Run `make -j`
