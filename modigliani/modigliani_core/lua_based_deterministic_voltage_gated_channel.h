/**
 * @file lua_based_deterministic_voltage_gated_channel.h
 * @author Ali Neishabouri
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

class Lua_based_deterministic_voltage_gated_channel : public Voltage_gated_ion_channel_current {
    /**
     * This class contains can read a lua script and interpret it to simualte
     * an ion channel deterministically.
     */
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
    virtual modigliani_base::Real open_channels() const;
    virtual modigliani_base::Real ComputeConductance() override;
    virtual modigliani_base::Real num_channels_in_state(
        modigliani_base::Size state) const;

    virtual void reset() override{
      exit(2);
    }

    void show_param() const;
    string lua_script;
  private:
    static modigliani_base::Real _lua_get_real(lua_State* L, string name);
    modigliani_base::Real stepV;
    lua_State* L;
};
}
#endif /* _modigliani_core_lua_based_deterministic_voltage_gated_channel_o_H_ */
