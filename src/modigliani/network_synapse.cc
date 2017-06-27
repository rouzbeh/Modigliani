/**
 * @file network_synapse.cc
 * @brief Network_synapse class implementation
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

#include "modigliani/network_synapse.h"

namespace modigliani {
  Network_synapse::Network_synapse(const modigliani::
                                   Real new_reversal_potential,
                                   Membrane_compartment * const src,
                                   const modigliani::Real newTimeStep,
                                   const std::string lua_file,
                                   const modigliani::Real new_strength)
  :Membrane_current(new_reversal_potential), source(src) {
    v_input = 0;
    _lua_script = lua_file;
    set_timestep(newTimeStep);
    _L = luaL_newstate();
    luaL_openlibs(_L);
    int status = luaL_dofile(_L, _lua_script.c_str());
    if (status) {
      /* If something went wrong, error message is at the top of */
      /* the stack */
      fprintf(stderr, "Network_synapse : Couldn't load file %s: %s\n",
              _lua_script.c_str(), lua_tostring(_L, -1));
      exit(1);
    }
    lua_getglobal(_L, "set_timestep");
    /* the first argument */
    lua_pushnumber(_L, newTimeStep);
    /* call the function with 1
       argument, return 0 result */
    lua_call(_L, 1, 0);

    // Set the param
    lua_getglobal(_L, "set_strength");
    /* the first argument */
    lua_pushnumber(_L, new_strength);
    /* call the function with 1
       argument, return 0 result */
    lua_call(_L, 1, 0);
  }

  Network_synapse::~Network_synapse() {
  }

  modigliani::ReturnEnum Network_synapse::StepCurrent() {
    v_input = source->vm();
    lua_getglobal(_L, "step_current");
    // the first argument
    lua_pushnumber(_L, voltage_);

    // call the function with 1 argument, return 0 result
    lua_call(_L, 1, 0);

    return (modigliani::ReturnEnum::SUCCESS);
  }

  modigliani::Real Network_synapse::ComputeConductance() {
    lua_getglobal(_L, "compute_conductance");

    /* call the function with 0
       argument, return 1 result */
    lua_call(_L, 0, 1);
    modigliani::Real conduc = lua_tonumber(_L, -1);
    lua_pop(_L, 1);

    // Make sure it's a number
    assert(conduc == conduc);

    return (set_conductance(conduc));
  }

  modigliani::Real Network_synapse::MaxConductivity()const {
    return (0);
  }
}  // namespace modigliani
