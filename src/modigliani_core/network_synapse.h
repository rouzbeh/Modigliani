/**
 * @file network_synapse.h
 * @brief Network_synapse class header
 *
 * Copyright (C) 2013 Ali Neishabouri
 *
 * @section LICENCE
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_NETWORK_SYNAPSE_H_
#define MODIGLIANI_MODIGLIANI_CORE_NETWORK_SYNAPSE_H_

#include <string>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "modigliani_core/membrane_current.h"
#include "modigliani_core/membrane_compartment.h"
namespace modigliani_core {
/**
 * @brief This class acts as a simplified synapse for use in neuronal network simulations.
 *
 * The synapse has access to the membrane voltage of the source
 * neuron at each step. The voltage is given to a lua script,
 * and the conductance is read from the same script.
 */
  class Network_synapse:public Membrane_current {
  public:
  /**
   * @brief The constructor initialises the lua interpreter.
   *
   * @param new_reversal_potential Reversal potential in
   * [@f$\si{\milli\volt}@f$]
   * @param src The presynaptic membrane compartment
   * @param newTimeStep Timestep in [@f$\si{\milli\second}@f$]
   * @param lua_file File containing the lua script governing the
   * synapse. The lua script needs to supply the following methods :
   * - void set_timestep(float)
   * - void set_parameter(float)
   * - void step_current(float)
   * - float comput_conductacne(void)
   * @param new_strength Synaptic strength
   */
    Network_synapse(const modigliani_base::Real new_reversal_potential,
                    Membrane_compartment * const src,
                    const modigliani_base::Real newTimeStep,
                    const std::string lua_file,
                    const modigliani_base::Real new_strength);

     virtual ~ Network_synapse();

     Network_synapse(const Network_synapse & original) = delete;

     Network_synapse & operator=(const Network_synapse & right) = delete;

  /**
   * @brief Runs one step of simulation
   * @return Success or failure
   */
     modigliani_base::ReturnEnum StepCurrent();

  /**
   * @brief Compute and return conductance
   * @return Conductance in [@f$\si{\milli\siemens}@f$]
   */
     modigliani_base::Real ComputeConductance();

  /**
   * @brief Returns maximum conductivity
   * @return Conductivity in [@f$\si{\milli\siemens}@f$]
   */
     modigliani_base::Real MaxConductivity() const;

  private:
     Membrane_compartment * source;
     modigliani_base::Real v_input;
     std::string _lua_script;
    lua_State *_L;
    // modigliani_base::Real avgSynBkCond; /* ns */
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_NETWORK_SYNAPSE_H_
