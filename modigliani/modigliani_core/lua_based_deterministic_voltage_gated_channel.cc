/**
 * @file lua_based_deterministic_voltage_gated_channel.cc
 * @brief Lua_based_deterministic_voltage_gated_channel class implementation.
 *
 * Copyright 2013 Mohammad Ali Neishabouri
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

#include "modigliani_core/lua_based_deterministic_voltage_gated_channel.h"

namespace modigliani_core {
  Lua_based_deterministic_voltage_gated_channel::
  Lua_based_deterministic_voltage_gated_channel
    (modigliani_base::Real newArea,
     modigliani_base::Real newDensity,
     modigliani_base::Real newConductivity,
     modigliani_base::Real newReversalPotential,
     modigliani_base::Real newTimeStep,
     modigliani_base::Real newTemperature,
     std::string new_lua_script)
  :Voltage_gated_ion_channel_current(newReversalPotential,
                                     newDensity,
                                     newArea,
                                     newConductivity) {
    UpdateNumChannels();        // TODO(Ali)
    lua_script = new_lua_script;
    set_timestep(newTimeStep);
    set_temperature(newTemperature);
    L = luaL_newstate();
    luaL_openlibs(L);

    boost::filesystem::path lua_path(lua_script);
    auto lua_path_parent = lua_path.parent_path();
    auto lua_path_common = lua_path.parent_path();
     lua_path_parent /= "?.lua";
     lua_path_common /= "common";
     lua_path_common /= "?.lua";
     SetLuaPath(L, lua_path_common.string());
     SetLuaPath(L, lua_path_parent.string());
    int status = luaL_dofile(L, lua_script.c_str());
    if (status) {
      /* If something went wrong, error message is at the top of */
      /* the stack */
      fprintf(stderr,
              "Lua_based_deterministic_voltage_gated_channel says : Couldn't load file %s: %s\n",
              lua_script.c_str(), lua_tostring(L, -1));
      exit(1);
    }
    stepV = _lua_get_real(L, "step");
    lua_getglobal(L, "set_timestep");
    /* the first argument */
    lua_pushnumber(L, newTimeStep);
    /* call the function with 1
       argument, return 0 result */
    lua_call(L, 1, 0);

    // Set temperature
    lua_getglobal(L, "set_temp");
    /* the first argument */
    lua_pushnumber(L, newTemperature);
    /* call the function with 1
       argument, return 0 result */
    lua_call(L, 1, 0);
  }

  Lua_based_deterministic_voltage_gated_channel::
  ~Lua_based_deterministic_voltage_gated_channel() {
    lua_close(L);
  }

  inline modigliani_base::ReturnEnum
    Lua_based_deterministic_voltage_gated_channel::StepCurrent() {
    switch (simulation_mode()) {
      case DETERMINISTIC: {
          lua_getglobal(L, "step_current");
          // the first argument
          lua_pushnumber(L, voltage_);

          // call the function with 1 argument, return 0 result
          lua_call(L, 1, 0);

          return (modigliani_base::ReturnEnum::SUCCESS);
        }

        break;
      default:
        std::cerr
          << "Lua_based_deterministic_voltage_gated_channel::StepCurrent - "
          << "ERROR : Unsupported simulation mode."
          << std::endl;
        return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
        break;
    }
    return (modigliani_base::ReturnEnum::FAIL);
  }

  inline modigliani_base::
    Real Lua_based_deterministic_voltage_gated_channel::OpenChannels() const {
    lua_getglobal(L, "open_channels");

    // call the function with 0 argument, return 1 result
    lua_call(L, 0, 1);
    modigliani_base::Real count = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return (count * num_channels());
  } inline modigliani_base::
    Real Lua_based_deterministic_voltage_gated_channel::ComputeConductance() {
    lua_getglobal(L, "compute_conductance");
    /* call the function with 0
       argument, return 1 result */
    lua_call(L, 0, 1);
    modigliani_base::Real conduc = lua_tonumber(L, -1);
    lua_pop(L, 1);

    if (conduc != conduc) {
      std::cerr << "Lua_based_deterministic_voltage_gated_channel : "
                << "conduc != conduc !" << std::endl;
      std::cerr << "Voltage was " << voltage_ << std::endl;
      assert(conduc == conduc);
    }

    return (set_conductance
            (conduc * MaxConductivity() * area() /* muMeter^2 */ *1.0e-8));
  }

  modigliani_base::Real
  Lua_based_deterministic_voltage_gated_channel::_lua_get_real(lua_State * L,
                                                               string name) {
    lua_getglobal(L, name.c_str());
    modigliani_base::Real ret = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return (ret);
  }
}  // namespace modigliani_core
