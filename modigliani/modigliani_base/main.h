/**
 * @file main.h Main header file for modigliani sources
 * @version: 2
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/// \mainpage
/// Ion channels are transmembrane proteins which selectively allow
/// ions to cross the lipid bilayer. As is the case for almost any
/// molecular process in biology, ion channels opening and closing are
/// stochastic processes (Faisal, Selen, and Wolpert, 2008). When
/// modelling neurons, it is often implicitly assumed that averaging
/// large numbers of ion channels allows the elimination of the
/// effects of individual channels’ randomness at higher levels (such
/// as spike timing). The standard deviation of variability over the
/// mean activity of a set of signalling molecules is inversely
/// proportional to the square root of the number of involved
/// molecules. In large cells, such as the squid giant axon which has
/// been key in understanding neural mechanisms, the variability
/// averages out quickly. In comparison, neuronal connections in our
/// brains can be over 10000 times smaller. This implies that even a
/// small number of channels randomly opening can have a systematic
/// effect on the cell’s behaviour (Faisal, White, and Laughlin, 2005;
/// Faisal and Laughlin, 2007; Faisal, Selen, and Wolpert, 2008).
/// Moreover, neurons perform highly non- linear operations involving
/// high gain amplification and positive feedback. Therefore, small
/// random fluctuations can therefore significantly change whole cell
/// responses.
/// 
/// In order to capture the effects of the variability of ion
/// channels, we need to use the stochastic  partial differential
/// equations describing the evolution of the membrane potential in
/// space and time inside neurons. We use Modigliani to perform
/// Monte-Carlo integration on these equations (Faisal, Laughlin, and
/// White, 2002). Modigliani is a fast, accurate stochastic partial
/// differential equation solver (Faisal, White, and Laughlin, 2005).
/// It is completely object oriented, and its classes are mapped onto
/// relevant entities in neuroscience (see Figure 2). Using
/// Modigliani, we can build complex neurons from the ground up by
/// plugging objects, such as compartments or ion channels together
/// and change them at during runtime.
/// 
/// Modigliani simulates each individual ion channel as a Markov
/// process switching between finite states. A typical NaV1.6 channel,
/// for instance, is simulated as an 8-state markov process, where
/// transition probabilities depend on the membrane potential, and the
/// transition probabilities are given by the well known kinetic
/// rate functions that govern gating particles in the deterministic
/// Hodgkin-Huxley type model (Hodgkin and Huxley, 1952). While
/// simulating neurons stochastically, the choice of integration
/// algorithm can have a crucial effect on the behaviour of the cell.
/// We typically use the binomial algorithm (Faisal, 2010), where the
/// channels of each compartment are treated as a binomial population.
/// At each step, binomial random numbers are generated to estimate
/// the number of channels switching states based on the individual
/// transition probabilities of each ion channel. This algorithm is
/// chosen because of its accuracy as compared to the Langevin
/// algorithm, and speed when compared to the Gillespie algorithm (For
/// details about algorithms and their performance, see Faisal, 2010).

#ifndef MODIGLIANI_MODIGLIANI_BASE_MAIN_H_
#define MODIGLIANI_MODIGLIANI_BASE_MAIN_H_

#define M_MAJOR_VERSION 2
#define M_MINOR_VERSION 3

#define M_STRING_LENGTH 255
#define M_SEPARATOR " ;; "

#define M_EXPORT extern
#define M_TALKLEVEL 9

#define M_DEBUG true

#endif  // MODIGLIANI_MODIGLIANI_BASE_MAIN_H_
