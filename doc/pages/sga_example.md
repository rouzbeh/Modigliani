Running the squid giant axon example {#sga_example}
====================================

Modigliani ships with a complete, runnable model of a patch of squid giant
axon (SGA) membrane in `examples/`. It finishes in under a second and it
exercises most of the machinery: JSON configuration, Lua channel models, the
binomial population algorithm, and binary recording to disk. It is the
fastest way to see the simulator work, and a reasonable starting point for a
model of your own.

[TOC]

Quick start                                            {#sga_quickstart}
-----------

Build the project, then run the example from inside `examples/`, so that the
relative Lua paths resolve:

    autoreconf -f -v -i
    ./configure
    make -j

    cd examples
    ../src/auxsrc/modigliani --config-file sga_suprathreshold.json

Results land in a timestamped folder under `/tmp/`. Point
`simulation_parameters.outputFolder` somewhere else to change that.

Two configurations are provided. They are the same model, and differ only in
how long the stimulus lasts:

Configuration              | Stimulus          | Behaviour
-------------------------- | ----------------- | ------------------------------------------
`sga_suprathreshold.json`  | 0.2 nA for 1 ms   | Fires reliably
`sga.json`                 | 0.2 nA for 0.1 ms | Sits at threshold, fires on most runs

Start with `sga_suprathreshold.json`. Once the output makes sense, `sga.json`
is the more interesting of the two - see @ref sga_threshold.

What the model contains                                    {#sga_model}
-----------------------

A single compartment at 6.3 degrees Celsius carrying three currents:

Current    | Type          | Density (per mum^2) | Conductance | Reversal
---------- | ------------- | ------------------- | ----------- | ---------
leak       | `leak`        | -                   | 0.3 mS/cm^2 | -54.4 mV
sodium     | `lua`         | 60                  | 20 pS       | +50 mV
potassium  | `lua`         | 18                  | 20 pS       | -77 mV

The two voltage gated currents are `lua` currents: their kinetics live in
`examples/SGA_sodium.lua` and `examples/SGA_potassium.lua`, which in turn
pull the state diagrams from `examples/common/8_state_sodium.lua` and
`examples/common/5_state_potassium.lua`. Modigliani calls into those scripts
once at start up, to tabulate a transition probability for every state pair
at every voltage between `minV` and `maxV`. The main loop never touches Lua,
so the scripts cost nothing per timestep however elaborate they are.

`chAlg` selects the integration algorithm, per current:

`chAlg` | Algorithm            | Notes
------- | -------------------- | --------------------------------------------
1       | Deterministic        | Hodgkin-Huxley style, no channel noise
2       | Single channel       | Every channel stepped individually; slow
4       | Binomial population  | The usual choice

Both examples use `4`. `--algorithm N` overrides the configuration file for
every current at once, which is the convenient way to compare algorithms on
an otherwise identical model.

Anatomy and stimulus are Lua fragments embedded in the JSON. `anatomy_lua`
builds the list of compartments, `electrods_lua` picks which of them get
recorded, and `inject_current_lua` defines the stimulus as a function of
time in milliseconds:

    function current(t)
      if (t > 30 and t < 31) then return 0.2 else return 0; end;
    end;

Anything Lua can compute is available here, so stimuli need not be
rectangular pulses.

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

`conf.json` is a verbatim copy of the configuration, so a result directory
is self describing: you can always tell what produced it.

`compartment_0.bin` is binary - a `modigliani::Size` column count, then one
row of 32-bit floats per sample. Column 1 is the membrane potential in mV,
and the rest are the individual currents in nA, in the order the currents
were attached. Sampling happens every `sampN` iterations, so the 50000
iteration run above yields 5000 samples.

`show_binary_file` prints it as text. Give it the file, the number of
columns, and how many rows to skip between printed ones:

    ../src/auxsrc/show_binary_file <dir>/compartments/compartment_0.bin 4 500

    -64.9982   -0.00998756   -0.0252996   0.0345651
    -64.9945   -0.00998403   -0.0252988   0.0345759
    ...

Add column indices after that to print only some of them, counting from 1.

The result                                                 {#sga_result}
----------

\image html sga_trace.svg "sga_suprathreshold.json: 50 ms at 1 us resolution, binomial population algorithm, 0.2 nA stimulus for 1 ms from t = 30 ms."
\image latex sga_trace.svg "Squid giant axon membrane potential and currents."

The membrane sits near -65 mV, jittering visibly because the sodium and
potassium channels are simulated as finite stochastic populations rather
than smooth conductances. The stimulus at 30 ms depolarises the patch past
threshold; sodium channels open and the inward current drives the potential
to roughly +43 mV. Sodium then inactivates while the slower potassium
current carries the membrane back down, overshooting to about -76 mV, and
the afterhyperpolarisation decays over the following ten milliseconds. Run
to run the peak moves by a millivolt or two.

Note the sign convention: inward sodium current is negative, outward
potassium current is positive.

Firing at threshold                                      {#sga_threshold}
-------------------

`sga.json` shortens the stimulus to 0.1 ms, which places the model right at
threshold and makes it worth running more than once.

Algorithm                       | Result
------------------------------- | ------------------------------------------
`--algorithm 1`, deterministic  | Does not fire
`--algorithm 4`, binomial       | Fires on most runs

0.2 nA for 0.1 ms is below threshold for this patch, and the deterministic
solver reports exactly that. Given identical parameters, the stochastic
solver fires most of the time. The difference is channel noise: sodium
channels are a finite population being sampled, so the number that happen to
be open in the milliseconds after the stimulus fluctuates, and often enough
that fluctuation carries the membrane over threshold.

This is what Modigliani is for. Running one configuration under
`--algorithm 1` and then `--algorithm 4` is the shortest demonstration of
why a stochastic treatment of channel gating gives answers a deterministic
one cannot.

Reproducing a run                                          {#sga_seeding}
-----------------

Generators are seeded from `std::random_device`, so runs differ. The base
seed is printed at start up:

    Modigliani RNG base seed = 2739451103 (repeat this run with --seed 2739451103)

Passing it back reproduces the run exactly:

    ../src/auxsrc/modigliani --config-file sga.json --seed 2739451103

Use this to revisit an interesting trial, to attach a specific figure to a
result, or to compare two algorithms on identical noise.

Plotting a recording                                      {#sga_plotting}
--------------------

`doc/plot_trace.py` turns a recording into an SVG, membrane potential on top
and currents below. It takes the recording, the `Time.txt` that accompanies
it, an output path, and a label per current:

    python3 doc/plot_trace.py \
        <dir>/compartments/compartment_0.bin \
        <dir>/Time.txt \
        trace.svg \
        leak sodium potassium

It uses only the Python standard library, so there is nothing to install,
and it works on any compartment recording rather than just this example.
The figure above is produced this way.

Trying variations                                       {#sga_variations}
-----------------

- `--algorithm 1` switches to the deterministic solver. The jitter
  disappears and repeated runs are identical, seed or no seed.
- Lowering `chDen` for the sodium current shrinks the channel population, so
  the relative size of the fluctuations grows. Take it low enough and the
  axon begins to fire spontaneously.
- `numTrials` repeats the whole simulation, appending to the same recording
  files - the cheap way to gather statistics.
- Raising `numIter`, or widening the stimulus window in
  `inject_current_lua`, gives repetitive firing rather than a single spike.
- `--progressbar` is worth having on longer runs.
