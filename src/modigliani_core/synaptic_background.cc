/**
 * @file synaptic_background.cc
 *
 * @brief Synaptic_background class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synaptic_background.h"

namespace modigliani_core {

bool Synaptic_background::seed_set_ = false;

Synaptic_background::Synaptic_background(
    modigliani_base::Real newAvgSynBkCond, /* in nS */
    modigliani_base::Real newESynBk, /* Synaptic background reversal potential mV */
    modigliani_base::Real newSynSigma, /* Synaptic noise "diffusion" nS */
    modigliani_base::Real newSynTau, /* Synaptic time constant ms */
    modigliani_base::Real newVBase /* in mV */
    )
    : Membrane_current(newESynBk - newVBase) {
  avgSynBkCond = newAvgSynBkCond;
  synSigma = newSynSigma;
  synTau = newSynTau;
  synVar = synSigma * synSigma;
  vBase = newVBase;

  if (Synaptic_background::seed_set_ == false) {
    Synaptic_background::seed_ = time(NULL);
    std::cout << "Initial seed = " << Synaptic_background::seed_ << std::endl;
    Synaptic_background::seed_set_ = true;
  }
  rng_ = boost::random::mt19937();
  rng_.seed(Synaptic_background::seed_++);
  norm_ = boost::random::normal_distribution<>();
  
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
      (synVar * synTau / 2.0) * (1.0 - exp(-2.0 * timestep() / synTau)));

  gT += avgSynBkCond + (gT - avgSynBkCond) * exp(- timestep() / synTau)
      + amplitude * norm_(rng_);

  return (modigliani_base::ReturnEnum::SUCCESS);
}

}  // namespace modigliani_core
