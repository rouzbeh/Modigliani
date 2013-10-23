/**
 * @file lua_based_stochastic_voltage_gated_channel.h
 * @brief Lua_based_stochastic_voltage_gated_channel class header
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
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_
#define MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_

#include <boost/filesystem.hpp>
#include <map>
#include <string>
#include <vector>
#include "modigliani_core/voltage_gated_ion_channel_current.h"
#include "modigliani_core/ion_channels.h"
#include "modigliani_core/aux_func.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using std::string;
using std::ofstream;

namespace modigliani_core {
/**
 * @brief Stochastic ion channel with transition probabilities read from a lua script
 * 
 * Transition probabilities are read for a range of membrane potentials by calling a lua
 * script. The actual simulations are carried inside the program.
 */
class Lua_based_stochastic_voltage_gated_channel : public
Voltage_gated_ion_channel_current {
  public:
/**
 * @brief Loads transition probabilities by running a lua script.
 * @param newArea Membrane area in [@f$\si{\micro\meter\squared}@f$]
 * @param newDensity Channel density [@f$\si{\per\micro\meter\squared}@f$]
 * @param newConductivity Max conductivity
 * [@f$\si{\milli\siemens}@f$] per channel
 * @param reversalPotential Current reversal potential [@f$\si{\milli\volt}@f$]
 * @param newTimeStep Simulation time step [@f$\si{\milli\second}@f$]
 * @param newTemperature Simulation temperature [@f$\si{\celsius}@f$]
 * @param fileName Lua script
 * 
 *  The lua script must define the following variables :
 *  - float base_temp
 *  - uint number_states
 *  - array(uint) open_states
 *  - float minV
 *  - float maxV
 *  - float step
 *  .
 *  The lua script must define the following functions :
 *  - get_probability(uint, uint, float)
 *  - get_q10(uint, uint)
 *  .
 * See @ref SGA_sodium.lua for an example of such a lua script.
 */
    Lua_based_stochastic_voltage_gated_channel(
        modigliani_base::Real newArea, modigliani_base::Real newDensity,
        modigliani_base::Real newConductivity,
        modigliani_base::Real reversalPotential,
        modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
        string fileName);

    Lua_based_stochastic_voltage_gated_channel(
        const Lua_based_stochastic_voltage_gated_channel & original) = delete;
    const Lua_based_stochastic_voltage_gated_channel&
    operator=(const Lua_based_stochastic_voltage_gated_channel & right) =
        delete;
    virtual ~Lua_based_stochastic_voltage_gated_channel();

/**
 * @brief Returns channels time constant
 * @return Time constant in [@f$\si{\per\second}@f$]
 */
    virtual modigliani_base::Real ComputeTimeConstant() const;

/**
 * @brief Sets the conductance of the membrane current to the value
 * given by the number of open channels.
 * @return Conductance in [@f$\si{\milli\siemens}@f$]
 */
    virtual modigliani_base::Real ComputeConductance() override;

/**
 * @brief Calculates all transitions in ion channels.
 * @return Status
 * @warning Does not update conductance
 */
    virtual modigliani_base::ReturnEnum StepCurrent() override;

/**
 * @brief Returns number of open ionic channels
 * @return Number of open channels
 */
    virtual modigliani_base::Real OpenChannels() const override;

  private:
    static void load_file(string fileName, double temperature,
                          double time_step);
    static std::map<string, Transition_rate_matrix*> probability_matrix_map;
    static std::map<string, modigliani_base::Size> number_of_states_map;
    static std::map<string, double> base_temperature_map;
    static std::map<string, std::vector<modigliani_base::Size> > open_states_map;
    Ion_channels* channels_ptr_;
    static modigliani_base::Real lua_get_real(lua_State* L, string name);
    static bool initTableLookUp;
    static std::vector<string> initialised_probability_matrices;
    modigliani_base::Real baseTemp;
};
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_VOLTAGE_GATED_CHANNEL_H_
