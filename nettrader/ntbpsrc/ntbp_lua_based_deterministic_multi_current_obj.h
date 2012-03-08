/*
 * NTBP_lua_based_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_LUA_BASED_MULTI_CURRENT_O_H_
#define NTBP_LUA_BASED_MULTI_CURRENT_O_H_


#include "ntbp_multi_current_obj.h"
#include "ntbp_ion_channels_obj.h"

extern "C"{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
using namespace std;


class NTBP_lua_based_multi_current_o: public NTBP_multi_current_o {
public:
	NTBP_lua_based_multi_current_o(NTreal newArea, NTreal newDensity, NTreal newConductivity,
			NTreal newVBase, NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature, string fileName);
	virtual ~NTBP_lua_based_multi_current_o();

	NTreturn StepCurrent();
	NTreal OpenChannels() const;
	NTreal OpenChannelsRatio() const;
	NTreal ComputeConductance();
	virtual NTreal NumChannelsInState(NTsize state) const;
	void ShowParam() const;

private:
	NTreal baseTemp;
	lua_State* L;
};

#endif /* NTBP_lua_based_multi_current_o_H_ */
