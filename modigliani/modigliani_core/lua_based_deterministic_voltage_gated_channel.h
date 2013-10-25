/**
 * @file lua_based_deterministic_voltage_gated_channel.h
 * @brief Lua_based_deterministic_voltage_gated_channel class header
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_DETERMINISTIC_VOLTAGE_GATED_CHANNEL_H_
#define MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_DETERMINISTIC_VOLTAGE_GATED_CHANNEL_H_

#include <string>
#include "modigliani_core/voltage_gated_ion_channel_current.h"
#include "modigliani_core/ion_channels.h"
#include "modigliani_core/aux_func.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace modigliani_core {
  /**
   * @class Lua_based_deterministic_voltage_gated_channel
   * @brief This class contains can read a lua script and interpret it to simualte
   * an ion channel deterministically.
   */
  class Lua_based_deterministic_voltage_gated_channel:
    public Voltage_gated_ion_channel_current {
  public:
   /**
    * @brief Reads the lua script and constructs a new ion channel
    * current object.
    * @param newArea Membrane surface area in
    * [@f$\si{\micro\meter\squared}@f$]
    * @param newDensity Channel density in
    * [@f$\si{\per\micro\meter\squared}@f$]
    * @param newConductivity Single channel conductance in
    * [@f$\si{\milli\siemens}@f$]
    * @param reversalPotential Current reversal potential in
    * [@f$\si{\milli\volt}@f$]
    * @param newTimeStep Simulation timestep in
    * [@f$\si{\milli\second}@f$]
    * @param newTemperature Simulation temperature in
    * [@f$\si{\celsius}@f$]
    * @param fileName Lua script
    * 
    *  The lua script must define the following variables :
    *  - float step
    *  .
    *  The lua script must define the following functions :
    *  - set_timestep(double)
    *  - set_temp(double)
    *  - step_current(double)
    *  - double open_channels()
    *  - double compute_conductance()
    *  .
    * See @ref SGA_sodium.lua for an example of such a lua script.
    */
    Lua_based_deterministic_voltage_gated_channel(modigliani_base::Real newArea,
                                                  modigliani_base::
                                                  Real newDensity,
                                                  modigliani_base::
                                                  Real newConductivity,
                                                  modigliani_base::
                                                  Real reversalPotential,
                                                  modigliani_base::
                                                  Real newTimeStep,
                                                  modigliani_base::
                                                  Real newTemperature,
                                                  std::string fileName);

    Lua_based_deterministic_voltage_gated_channel
        (const
         Lua_based_deterministic_voltage_gated_channel
         & original) = delete;

     virtual ~Lua_based_deterministic_voltage_gated_channel();

     Lua_based_deterministic_voltage_gated_channel
         & operator=(const
                     Lua_based_deterministic_voltage_gated_channel
                     & right) = delete;

    /**
     * @brief Runs one step of simulation
     * @return Success or failure
     */
    virtual modigliani_base::ReturnEnum StepCurrent() override;

    virtual modigliani_base::Real OpenChannels() const;

    virtual modigliani_base::Real ComputeConductance() override;

     std::string lua_script;

  private:
    static modigliani_base::Real _lua_get_real(lua_State * L, std::string name);
     modigliani_base::Real stepV;
    lua_State *L;
  };
/**
 * @example SGA_sodium.lua
 * @brief This is an example of a lua script defining an ion channel.
 * In this case it is the classical Hodgkin-Huxley sodium channel.
 *
 * See also @ref common/timestep.lua and common/8_state_sodium.lua.
*/
/**
 * @example common/timestep.lua
 * @brief Auxiliary lua file
*/
/**
 * @example common/8_state_sodium.lua
 * @brief Auxiliary lua file
*/
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_DETERMINISTIC_VOLTAGE_GATED_CHANNEL_H_
