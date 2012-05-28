/*
 * NTBP_lua_based_deterministic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef _mcore_lua_based_deterministic_MULTI_CURRENT_O_H_
#define _mcore_lua_based_deterministic_MULTI_CURRENT_O_H_

#include "Multi_current.h"
#include "Ion_channels.h"
#include "auxfunc.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

namespace mcore {
class Lua_based_deterministic_multi_current: public Multi_current {
public:
	Lua_based_deterministic_multi_current(mbase::Real newArea,
			mbase::Real newDensity, mbase::Real newConductivity, mbase::Real newVBase,
			mbase::Real reversalPotential, mbase::Real newTimeStep, mbase::Real newTemperature,
			string fileName);

	Lua_based_deterministic_multi_current(
			const Lua_based_deterministic_multi_current & original);

	virtual ~Lua_based_deterministic_multi_current();

	const Lua_based_deterministic_multi_current&
	operator=(const Lua_based_deterministic_multi_current & right);

	virtual mbase::Mreturn step_current() override	;
	virtual mbase::Real open_channels() const override;
	virtual mbase::Real compute_conductance() override;
	virtual mbase::Real num_channels_in_state(mbase::Size_t state) const override;
	void show_param() const override;

	string lua_script;
private:
	static mbase::Real lua_get_ntreal(lua_State* L, string name);
	mbase::Real baseTemp;
	mbase::Real stepV;
	lua_State* L;
};}
#endif /* _mcore_lua_based_deterministic_multi_current_o_H_ */
