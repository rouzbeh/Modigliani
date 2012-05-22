/*
 * NTBP_lua_based_stochastic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef _mcore_lua_based_stochastic_MULTI_CURRENT_O_H_
#define _mcore_lua_based_stochastic_MULTI_CURRENT_O_H_


#include "Multi_current.h"
#include "Ion_channels.h"
#include "auxfunc.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <map>
using namespace std;

namespace mcore{
class Lua_based_stochastic_multi_current: public Multi_current {
public:
	Lua_based_stochastic_multi_current(mbase::Mreal newArea, mbase::Mreal newDensity, mbase::Mreal newConductivity,
			mbase::Mreal newVBase, mbase::Mreal reversalPotential, mbase::Mreal newTimeStep, mbase::Mreal newTemperature, string fileName);
	virtual ~Lua_based_stochastic_multi_current();

	static void load_file(string fileName, double temperature, double time_step);
	static map<string,NTBP_transition_rate_matrix_o*> probability_matrix_map;
	static map<string,mbase::Msize> number_of_states_map;
	static map<string,double> base_temperature_map;
	static map<string,std::vector<int> > open_states_map;
	mbase::Mreal num_channels_in_state(mbase::Msize state) const override;
	mbase::Mreal ComputeChannelStateTimeConstant() const override;

	virtual mbase::Mreturn step_current() override;
	virtual mbase::Mreal open_channels() const override;
	virtual mbase::Mreal compute_conductance() override;
	void show_param() const override;
	void printProb(string fileName){
		for (mbase::Mreal v =20; v<130; v+=0.005){
			cout << probability_matrix_map[fileName]->getTransitionProbability(v, 2, 1) << std::endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static mbase::Mreal lua_get_ntreal(lua_State* L, string name);
	static bool initTableLookUp;
	static std::vector<string> initialised_probability_matrices;
	mbase::Mreal baseTemp;

};
}
#endif /* _mcore_lua_based_stochastic_MULTI_CURRENT_O_H_ */
