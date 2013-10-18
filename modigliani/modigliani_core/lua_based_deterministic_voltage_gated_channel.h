/**
 * @file lua_based_deterministic_voltage_gated_channel.h
 * @brief Lua_based_deterministic_voltage_gated_channel class header
 */
#ifndef _modigliani_core_lua_based_deterministic_MULTI_CURRENT_O_H_
#define _modigliani_core_lua_based_deterministic_MULTI_CURRENT_O_H_

#include "voltage_gated_ion_channel_current.h"
#include "ion_channels.h"
#include "aux_func.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

namespace modigliani_core {
  /**
   * @class Lua_based_deterministic_voltage_gated_channel
   * @brief This class contains can read a lua script and interpret it to simualte
   * an ion channel deterministically.
   */
class Lua_based_deterministic_voltage_gated_channel : public Voltage_gated_ion_channel_current {
  public:
    Lua_based_deterministic_voltage_gated_channel(
        modigliani_base::Real newArea, modigliani_base::Real newDensity,
        modigliani_base::Real newConductivity,
        modigliani_base::Real reversalPotential,
        modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
        string fileName);

    Lua_based_deterministic_voltage_gated_channel(
        const Lua_based_deterministic_voltage_gated_channel & original);

    virtual ~Lua_based_deterministic_voltage_gated_channel();

    Lua_based_deterministic_voltage_gated_channel& operator=(
        const Lua_based_deterministic_voltage_gated_channel & right);

    virtual modigliani_base::ReturnEnum StepCurrent() override;
    virtual modigliani_base::Real OpenChannels() const;
    virtual modigliani_base::Real ComputeConductance() override;
    
    void show_param() const;
    string lua_script;
  private:
    static modigliani_base::Real _lua_get_real(lua_State* L, string name);
    modigliani_base::Real stepV;
    lua_State* L;
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
}
#endif /* _modigliani_core_lua_based_deterministic_voltage_gated_channel_o_H_ */
