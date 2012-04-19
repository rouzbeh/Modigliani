/*
 * NTBP_lua_based_deterministic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef _mcore_lua_based_deterministic_MULTI_CURRENT_O_H_
#define _mcorelua_based_deterministic_MULTI_CURRENT_O_H_

#include "Multi_current.h"
#include "Ion_channels.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

namespace mcore {
class Lua_based_deterministic_multi_current: public Multi_current {
public:
	Lua_based_deterministic_multi_current(NTreal newArea,
			NTreal newDensity, NTreal newConductivity, NTreal newVBase,
			NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature,
			string fileName);

	Lua_based_deterministic_multi_current(
			const Lua_based_deterministic_multi_current & original);

	virtual ~Lua_based_deterministic_multi_current();

	const Lua_based_deterministic_multi_current&
	operator=(const Lua_based_deterministic_multi_current & right);

	virtual NTreturn step_current() override	;
	virtual NTreal open_channels() const override;
	virtual NTreal compute_conductance() override;
	virtual NTreal NumChannelsInState(NTsize state) const override;
	void ShowParam() const override;

	string lua_script;
private:
	NTreal baseTemp;
	lua_State* L;
};}
#endif /* _mcore_lua_based_deterministic_multi_current_o_H_ */
