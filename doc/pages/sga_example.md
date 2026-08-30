Running the squid giant axon example {#sga_example}
====================================

Modigliani ships with a complete, runnable model of a patch of squid giant
axon (SGA) membrane in `examples/`. It is small enough to finish in under a
second and it exercises most of the machinery: JSON configuration, Lua
channel models, the binomial population algorithm, and binary recording to
disk. This page walks through it.

The figure at the bottom of this page is not a stored screenshot. It is
regenerated from a real simulation every time the documentation is built.

[TOC]

Quick start                                            {#sga_quickstart}
-----------

Build the project, then run the example from inside `examples/` so that the
relative Lua paths resolve:

    autoreconf -f -v -i
    ./configure
    make -j

    cd examples
    ../src/auxsrc/modigliani --config-file sga_suprathreshold.json

By default the results land in a timestamped folder under `/tmp/`, which is
what `simulation_parameters.outputFolder` selects. Change that key to write
somewhere else.

There are two variants of the same model, differing only in how long the
stimulus lasts:

Configuration                | Stimulus            | Fires
---------------------------- | ------------------- | -------------------------
`sga_suprathreshold.json`    | 0.2 nA for 1 ms     | Always, on every algorithm
`sga.json`                   | 0.2 nA for 0.1 ms   | About 8 runs in 10

`sga.json` sits deliberately at threshold, which is a more interesting model
but a worse first thing to run. The @ref sga_threshold section below explains
what is going on there.

What the model contains                                    {#sga_model}
-----------------------

Both configurations describe a single compartment at 6.3 degrees Celsius
carrying three currents:

Current    | Type          | Density (per mum^2) | Conductance | Reversal
---------- | ------------- | ------------------- | ----------- | ---------
leak       | `leak`        | -                   | 0.3 mS/cm^2 | -54.4 mV
sodium     | `lua`         | 60                  | 20 pS       | +50 mV
potassium  | `lua`         | 18                  | 20 pS       | -77 mV

The two voltage gated currents are `lua` currents: their kinetics live in
`examples/SGA_sodium.lua` and `examples/SGA_potassium.lua`, which in turn
pull the state diagrams from `examples/common/8_state_sodium.lua` and
`examples/common/5_state_potassium.lua`. Modigliani calls into those scripts
once at start up to tabulate a transition probability for every state pair
at every voltage between `minV` and `maxV`, then never touches Lua again in
the main loop.

`chAlg` selects the integration algorithm per current:

`chAlg` | Algorithm            | Notes
------- | -------------------- | --------------------------------------------
1       | Deterministic        | Hodgkin-Huxley style, no channel noise
2       | Single channel       | Every channel stepped individually; slow
4       | Binomial population  | The default, and the one to use

The example uses `4`. Passing `--algorithm N` on the command line overrides
whatever the configuration file says, for every current at once.

Anatomy and stimulus are themselves Lua fragments embedded in the JSON.
`anatomy_lua` builds the list of compartments, `electrods_lua` picks which
of them get recorded, and `inject_current_lua` defines the stimulus - here a
0.2 nA pulse lasting 1 ms, delivered at t = 30 ms:

    function current(t)
      if (t > 30 and t < 31) then return 0.2 else return 0; end;
    end;

What comes out                                            {#sga_output}
--------------

Each run creates its own timestamped directory:

    Aug30_213540/
      conf.json                        copy of the configuration used
      log.log                          compartment count, completion status
      Time.txt                         one sample time in ms per line
      TypePerCompartment.txt
      LengthPerCompartment.txt
      compartments/
        compartment_0.bin              the recording

`compartment_0.bin` is binary: a `modigliani::Size` column count, then one
row of 32-bit floats per sample. Column 1 is the membrane potential in mV
and the rest are the individual currents in nA, in the order the currents
were attached. Sampling is every `sampN` iterations, so the 50000 iteration
run above yields 5000 samples.

Use `show_binary_file` to inspect it as text - it detects and skips the
column count header:

    ../src/auxsrc/show_binary_file <dir>/compartments/compartment_0.bin 4 500

    -64.9982   -0.00998756   -0.0252996   0.0345651
    -64.9945   -0.00998403   -0.0252988   0.0345759
    ...

The trailing arguments are the number of columns and a decimation factor;
add column indices to print only some of them.

The result                                                 {#sga_result}
----------

\image html sga_trace.svg "sga_suprathreshold.json: 50 ms at 1 us resolution, binomial population algorithm, 0.2 nA stimulus for 1 ms from t = 30 ms."
\image latex sga_trace.svg "Squid giant axon membrane potential and currents."

The trace shows what it should. The membrane sits near -65 mV, jittering
visibly because the sodium and potassium channels are being simulated as
finite stochastic populations rather than smooth conductances. The stimulus
at 30 ms depolarises the patch past threshold; sodium channels open, the
inward current drives the potential to roughly +43 mV, then sodium
inactivates while the slower potassium current carries the membrane back
down and overshoots to about -76 mV. The afterhyperpolarisation decays over
the following ten milliseconds. Run to run the peak moves by a millivolt or
two, because the channel populations are stochastic.

Note the sign convention: inward sodium current is negative, outward
potassium current is positive.

Spiking at threshold                                     {#sga_threshold}
--------------------

`sga.json` shortens the stimulus to 0.1 ms, and that single change puts the
model somewhere considerably more interesting. Measured over ten runs each:

Algorithm                | Peak potential                          | Spiked
------------------------ | --------------------------------------- | ------
`--algorithm 1`, deterministic | -58.2 mV every time               | 0 / 10
`--algorithm 4`, binomial      | +36 to +40 mV, or about -59 mV    | 8 / 10

The deterministic solver never fires: 0.2 nA for 0.1 ms is simply below
threshold for this patch. The stochastic solver, given exactly the same
parameters, fires most of the time. The difference is the channel noise.
Sodium channels are a finite population being sampled, so the number that
happen to be open in the milliseconds after the stimulus fluctuates, and
often enough that fluctuation is what carries the membrane over threshold.

This is the phenomenon Modigliani exists to study, and it is worth
appreciating that no amount of care with the deterministic equations would
reveal it. It also means `sga.json` is a poor smoke test, which is why the
figure above and the continuous integration check both use the
suprathreshold variant.

Reproducing a particular run                                {#sga_seeding}
----------------------------

Every generator is seeded from `std::random_device`, so runs differ. The
base seed is printed at start up:

    Modigliani RNG base seed = 2739451103 (repeat this run with --seed 2739451103)

Passing that back reproduces the run exactly:

    ../src/auxsrc/modigliani --config-file sga.json --seed 2739451103

Which is the way to investigate an interesting trial, or to get a stable
figure out of a stochastic model.

Trying variations                                       {#sga_variations}
-----------------

Because the channel populations are stochastic, two runs never coincide
exactly. Interesting things to change:

- `--algorithm 1` switches to the deterministic solver. The jitter
  disappears entirely and repeated runs are identical, seed or no seed.
- Lowering `chDen` for the sodium current shrinks the channel population, so
  the relative size of the fluctuations grows. Take it low enough and the
  axon starts to fire spontaneously.
- `numTrials` repeats the whole simulation, appending to the same recording
  files, which is the cheap way to gather statistics.
- `--progressbar` is worth having on longer runs.

Regenerating the figure                                     {#sga_regen}
-----------------------

`doc/build_docs.sh` runs the whole chain - build, simulate, plot, doxygen -
and is what continuous integration invokes:

    ./doc/build_docs.sh

The plotting step is `doc/plot_trace.py`, which reads a recording plus its
`Time.txt` and writes an SVG. It uses only the Python standard library, so
building the documentation needs nothing beyond `python3` and `doxygen`.
