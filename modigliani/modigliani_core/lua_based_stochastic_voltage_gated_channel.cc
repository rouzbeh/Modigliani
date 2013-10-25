/**
 * @file lua_based_stochastic_voltage_gated_channel.cc
 * @brief Lua_based_stochastic_voltage_gated_channel class implementation
 *
 * Copyright 2013 Mohammad Ali Neishabouri
 *
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

#include "modigliani_core/lua_based_stochastic_voltage_gated_channel.h"

namespace modigliani_core {
bool Lua_based_stochastic_voltage_gated_channel::initTableLookUp = false;
std::map<string, Transition_rate_matrix*>
Lua_based_stochastic_voltage_gated_channel::probability_matrix_map;
std::map<string, modigliani_base::Size>
Lua_based_stochastic_voltage_gated_channel::number_of_states_map;
std::map<string, double>
Lua_based_stochastic_voltage_gated_channel::base_temperature_map;
std::map<string, std::vector<modigliani_base::Size> >
Lua_based_stochastic_voltage_gated_channel::open_states_map;

Lua_based_stochastic_voltage_gated_channel::
Lua_based_stochastic_voltage_gated_channel(
    modigliani_base::Real newArea, modigliani_base::Real newDensity,
    modigliani_base::Real newConductivity,
    modigliani_base::Real newReversalPotential,
    modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
    string fileName)
    : Voltage_gated_ion_channel_current(newReversalPotential,
                                        newDensity,
                                        newArea, newConductivity
                                        ) {
  UpdateNumChannels();  // TODO(Ali)

  set_timestep(newTimeStep);
  set_temperature(newTemperature);
  if (number_of_states_map[fileName] == 0) {
    load_file(fileName, newTemperature, newTimeStep);
  }
  baseTemp = base_temperature_map[fileName];

  assert(number_of_states_map[fileName] > 0);

  if (false == initTableLookUp) {
    initTableLookUp = true;
  }
  channels_ptr_ = new Ion_channels(num_channels(),
                                   number_of_states_map[fileName],
                                   probability_matrix_map[fileName],
                                   newTimeStep);
  for (unsigned int i = 0; i < open_states_map[fileName].size(); i++) {
    channels_ptr_->SetAsOpenState(open_states_map[fileName][i]);
  }

  channels_ptr_->SteadyStateDistribution(0);
  channels_ptr_->SteadyStateDistribution(-60);
}

Lua_based_stochastic_voltage_gated_channel::
~Lua_based_stochastic_voltage_gated_channel() {
  delete channels_ptr_;
  channels_ptr_ = 0;
}

void Lua_based_stochastic_voltage_gated_channel::load_file(string fileName,
                                                           double temperature,
                                                           double time_step) {
  cout << "Getting probabilities from " << fileName << std::endl;
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  // Add the folder of the lua to the LUA_PATH
  boost::filesystem::path lua_path(fileName);
  auto lua_path_parent = lua_path.parent_path();
  auto lua_path_common = lua_path.parent_path();
  lua_path_parent /= "?.lua";
  lua_path_common /= "common";
  lua_path_common /= "?.lua";
  SetLuaPath(L, lua_path_common.string());
  SetLuaPath(L, lua_path_parent.string());

  int status = luaL_dofile(L, fileName.c_str());
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(
        stderr,
        "Lua_based_stochastic_voltage_gated_channel says : Couldn't load file %s: %s\n",
        fileName.c_str(), lua_tostring(L, -1));
    exit(1);
  }

  base_temperature_map[fileName] = lua_get_real(L, "base_temp");

  number_of_states_map[fileName] = lua_get_real(L, "number_states");

  open_states_map[fileName] = std::vector<modigliani_base::Size>();

  lua_getglobal(L, "open_states");
  /* table is in the stack at index 't' */
  lua_pushnil(L); /* first key */
  while (lua_next(L, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    int found = lua_tonumber(L, -1);
    open_states_map[fileName].push_back(found);
    lua_pop(L, 1);
  }
  lua_pop(L, 1);

  double minV = lua_get_real(L, "minV");
  double maxV = lua_get_real(L, "maxV");
  double step = lua_get_real(L, "step");

  probability_matrix_map[fileName] = new Transition_rate_matrix(
      number_of_states_map[fileName], minV, maxV, step);

  modigliani_base::Real length = floor((maxV - minV) / step + 0.5) + 1;
  for (modigliani_base::Size i = 1; i <= number_of_states_map[fileName]; ++i) {
    for (modigliani_base::Size j = 1; j <= number_of_states_map[fileName];
        ++j) {
      for (modigliani_base::Size k = 0; k < length; k++) {
        modigliani_base::Real voltage = minV + step * k;
        lua_getglobal(L, "get_probability");
        lua_pushnumber(L, i);
        lua_pushnumber(L, j);
        lua_pushnumber(L, voltage);
        lua_call(L, 3, 1);
        double base_probability = lua_tonumber(L, -1);
        lua_pop(L, 1);
        lua_getglobal(L, "get_q10");
        lua_pushnumber(L, i);
        lua_pushnumber(L, j);
        lua_call(L, 2, 1);
        double prob_q10 = lua_tonumber(L, -1);
        lua_pop(L, 1);
        double probability = TemperatureRateRelation(
            temperature, base_temperature_map[fileName], prob_q10)
            * base_probability * time_step;
        probability_matrix_map[fileName]->SetTransitionProbability(voltage, i,
                                                                   j,
                                                                   probability);
      }
    }
  }
  lua_close(L);
}

inline modigliani_base::ReturnEnum
Lua_based_stochastic_voltage_gated_channel::StepCurrent() {
  switch (simulation_mode()) {
    case BINOMIALPOPULATION: {
      return (channels_ptr_->BinomialStep(voltage_));
    }
      break;
    case SINGLECHANNEL: {
      return (channels_ptr_->SingleChannelStep(voltage_));
    }
      break;
    case GILLESPIE: {
      return (channels_ptr_->GillespieStep(voltage_));
    }
      break;
    case DETERMINISTIC: {
      return (channels_ptr_->DeterministicStep(voltage_));
    }

      break;
    default:
      std::cerr
          << "Lua_based_stochastic_voltage_gated_channel::StepCurrent"
          << " - ERROR : Unsupported simulation mode."
          << std::endl;
      return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
      break;
  }
  return (modigliani_base::ReturnEnum::FAIL);
}

inline modigliani_base::Real Lua_based_stochastic_voltage_gated_channel::
OpenChannels() const {
  return (channels_ptr_->NumOpen());
}

inline modigliani_base::Real Lua_based_stochastic_voltage_gated_channel::
ComputeConductance() {
  return (set_conductance(channels_ptr_->NumOpen() * conductivity_));
}

inline modigliani_base::Real Lua_based_stochastic_voltage_gated_channel::
ComputeTimeConstant() const {
  return (channels_ptr_->ComputeChannelStateTimeConstant(voltage_));
}

modigliani_base::Real Lua_based_stochastic_voltage_gated_channel::lua_get_real(
    lua_State* L, string name) {
  lua_getglobal(L, name.c_str());
  modigliani_base::Real ret = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return (ret);
}
}  // namespace modigliani_core
