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
	Lua_based_deterministic_multi_current(mbase::Mreal newArea,
			mbase::Mreal newDensity, mbase::Mreal newConductivity, mbase::Mreal newVBase,
			mbase::Mreal reversalPotential, mbase::Mreal newTimeStep, mbase::Mreal newTemperature,
			string fileName);

	Lua_based_deterministic_multi_current(
			const Lua_based_deterministic_multi_current & original);

	virtual ~Lua_based_deterministic_multi_current();

	const Lua_based_deterministic_multi_current&
	operator=(const Lua_based_deterministic_multi_current & right);

	virtual mbase::Mreturn step_current() override	;
	virtual mbase::Mreal open_channels() const override;
	virtual mbase::Mreal compute_conductance() override;
	virtual mbase::Mreal NumChannelsInState(mbase::Msize state) const override;
	void ShowParam() const override;

	string lua_script;
private:
	mbase::Mreal baseTemp;
	lua_State* L;
};}
#endif /* _mcore_lua_based_deterministic_multi_current_o_H_ */
