/**
 * \file synaptic_background.cc
 *
 * \brief Synaptic_background class implementation
 *
 * \author Ahmed Aldo Faisal &copy; created 20.8.2003
 * \version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * \section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "synaptic_background.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
/** Create a Synaptic_background */
Synaptic_background::Synaptic_background(
    modigliani_base::Real newAvgSynBkCond, /* in nS */
    modigliani_base::Real newESynBk, /* Synaptic background reversal potential mV */
    modigliani_base::Real __attribute__((unused)) newSynSigma, /* Synaptic noise "diffusion" nS */
    modigliani_base::Real newSynTau, /* Synaptic time constant ms */
    modigliani_base::Real newVBase /* in mV */
    )
    : Membrane_current(newESynBk - newVBase) {
  avgSynBkCond = newAvgSynBkCond;
  synSigma = newAvgSynBkCond;
  synTau = newSynTau;
  synVar = synSigma * synSigma;
  vBase = newVBase;

  gT = 0.0;
  amplitude = 0.0;
}

/* ***      DESTRUCTOR		***/
Synaptic_background::~Synaptic_background() {
}

/* ***  PUBLIC                                    ***   */
modigliani_base::Real Synaptic_background::compute_conductance() {
  return (set_conductance(0.001 /* mS/nS */* gT /* nS */));
}

modigliani_base::ReturnEnum Synaptic_background::step_current() {
  amplitude = sqrt(
      (synVar * synTau / 2.0) * (1.0 - exp(-2.0 * _timeStep() / synTau)));

  gT += avgSynBkCond + (gT - avgSynBkCond) * exp(-_timeStep() / synTau)
      + amplitude * normRnd.RndVal();

  return (modigliani_base::ReturnEnum::SUCCESS);
}
