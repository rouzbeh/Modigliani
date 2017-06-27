/**
 * @file synaptic_background.h
 * @brief Synaptic_background class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENSE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_SYNAPTIC_BACKGROUND_H_
#define MODIGLIANI_MODIGLIANI_CORE_SYNAPTIC_BACKGROUND_H_

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include "membrane_current.h"

namespace modigliani {
  /**
   *  @brief This class represents a synaptic background current
  */
  class Synaptic_background:public Membrane_current {
  public:
    /**
     * @brief Create a Synaptic_background current
     */
    Synaptic_background(modigliani::Real newAvgSynBkCond,  /* in nS */
                        modigliani::Real newESynBk,        /* Synaptic background reversal potential mV */
                        modigliani::Real newSynSigma,      /* Synaptic noise "diffusion" nS */
                        modigliani::Real newSynTau,        /* Synaptic time constant ms */
                        modigliani::Real newVBase = -65.0  /* in mV */
      );

    Synaptic_background(const Synaptic_background & original) = delete;

     Synaptic_background & operator=(const Synaptic_background & right) =
      delete;

     modigliani::ReturnEnum step_current();
    /** compute and return conductance in mSiemens */
     modigliani::Real compute_conductance();

     virtual ~ Synaptic_background();

    static bool seed_set_;

  private:
     modigliani::Real avgSynBkCond;        /* ns */
     modigliani::Real synSigma;    /* ns */
     modigliani::Real synVar;      /* ns^2 */
     modigliani::Real synTau;      /* ms */
     modigliani::Real vBase;       /* mV */
     modigliani::Real gT;  /* nS */
     modigliani::Real amplitude;   /* nS */

     boost::random::mt19937 rng_;
     boost::random::normal_distribution <> norm_;
    static unsigned int seed_;

  };
}                               // namespace modigliani
#endif                          // MODIGLIANI_MODIGLIANI_CORE_SYNAPTIC_BACKGROUND_H_
