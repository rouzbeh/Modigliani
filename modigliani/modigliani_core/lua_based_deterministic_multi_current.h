/**
 * @file lua_based_deterministic_multi_current.h
 * @author Ali Neishabouri
 */

#ifndef _modigliani_core_lua_based_deterministic_MULTI_CURRENT_O_H_
#define _modigliani_core_lua_based_deterministic_MULTI_CURRENT_O_H_

#include "voltage_gated_ion_channel_current.h"
#include "ion_channels.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

namespace modigliani_core {
class Lua_based_deterministic_multi_current: public Voltage_gated_ion_channel_current {
public:
	Lua_based_deterministic_multi_current(modigliani_base::Real newArea,
			modigliani_base::Real newDensity, modigliani_base::Real newConductivity,
			modigliani_base::Real reversalPotential, modigliani_base::Real newTimeStep,
			modigliani_base::Real newTemperature, string fileName);

	Lua_based_deterministic_multi_current(
			const Lua_based_deterministic_multi_current & original);

	virtual ~Lua_based_deterministic_multi_current();

	Lua_based_deterministic_multi_current& operator=(
			const Lua_based_deterministic_multi_current & right);

	virtual modigliani_base::ReturnEnum StepCurrent() override;
	virtual modigliani_base::Real open_channels() const;
	virtual modigliani_base::Real ComputeConductance() override;
	virtual modigliani_base::Real num_channels_in_state(modigliani_base::Size state) const;
  virtual modigliani_base::Real ComputeTimeConstant() const override;

	void show_param() const;

	string lua_script;
private:
	static modigliani_base::Real lua_get_ntreal(lua_State* L, string name);
	modigliani_base::Real baseTemp;
	modigliani_base::Real stepV;
	lua_State* L;
};
}
#endif /* _modigliani_core_lua_based_deterministic_multi_current_o_H_ */
