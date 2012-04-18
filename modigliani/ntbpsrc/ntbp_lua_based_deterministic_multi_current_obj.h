/*
 * NTBP_lua_based_deterministic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_lua_based_deterministic_MULTI_CURRENT_O_H_
#define NTBP_lua_based_deterministic_MULTI_CURRENT_O_H_

#include "ntbp_multi_current_obj.h"
#include "ntbp_ion_channels_obj.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

class NTBP_lua_based_deterministic_multi_current_o: public NTBP_multi_current_o {
public:
	NTBP_lua_based_deterministic_multi_current_o(NTreal newArea,
			NTreal newDensity, NTreal newConductivity, NTreal newVBase,
			NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature,
			string fileName);

	NTBP_lua_based_deterministic_multi_current_o(
			const NTBP_lua_based_deterministic_multi_current_o & original);

	virtual ~NTBP_lua_based_deterministic_multi_current_o();

	const NTBP_lua_based_deterministic_multi_current_o&
	operator=(const NTBP_lua_based_deterministic_multi_current_o & right);

	virtual NTreturn step_current() override;
	virtual NTreal open_channels() const override;
	virtual NTreal compute_conductance() override;
	virtual NTreal NumChannelsInState(NTsize state) const override;
	void ShowParam() const override;

	string lua_script;
private:
	NTreal baseTemp;
	lua_State* L;
};

#endif /* NTBP_lua_based_deterministic_multi_current_o_H_ */
