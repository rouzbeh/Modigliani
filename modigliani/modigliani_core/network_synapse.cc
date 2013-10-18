///
/// \file network_synapse.cc
/// \author Ali Neishabouri &copy; created 24/01/2013
/// \version   0.1
/// Copyright (C) 2013 Ali Neishabouri

#include "network_synapse.h"

namespace modigliani_core {

Network_synapse::Network_synapse(
    const modigliani_base::Real new_reversal_potential,
    Membrane_compartment * const src, const modigliani_base::Real newTimeStep,
    const std::string lua_file, const modigliani_base::Real new_strength)
    : Membrane_current(new_reversal_potential), source(src) {
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

modigliani_base::ReturnEnum Network_synapse::StepCurrent() {
  v_input = source->vm();
  lua_getglobal(_L, "step_current");
  // the first argument
  lua_pushnumber(_L, voltage_);

  // call the function with 1 argument, return 0 result
  lua_call(_L, 1, 0);

  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::Real Network_synapse::ComputeConductance() {
  lua_getglobal(_L, "compute_conductance");

  /* call the function with 0
   argument, return 1 result */
  lua_call(_L, 0, 1);
  modigliani_base::Real conduc = lua_tonumber(_L, -1);
  lua_pop(_L, 1);

  // Make sure it's a number
  assert(conduc == conduc);

  return (set_conductance(conduc));
}

modigliani_base::Real Network_synapse::MaxConductivity() const {
  return (0);
}
}
