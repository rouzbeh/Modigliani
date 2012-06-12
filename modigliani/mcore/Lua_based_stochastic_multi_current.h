/**
 * @file Lua_based_stochastic_multi_current.h
 * @author Ali Neishabouri
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

namespace mcore {
class Lua_based_stochastic_multi_current: public Multi_current {
public:
	Lua_based_stochastic_multi_current(mbase::Real newArea,
			mbase::Real newDensity, mbase::Real newConductivity,
			mbase::Real reversalPotential, mbase::Real newTimeStep,
			mbase::Real newTemperature, string fileName);
	virtual ~Lua_based_stochastic_multi_current();

	static void load_file(string fileName, double temperature,
			double time_step);
	static map<string, Transition_rate_matrix*> probability_matrix_map;
	static map<string, mbase::Size_t> number_of_states_map;
	static map<string, double> base_temperature_map;
	static map<string, std::vector<int> > open_states_map;
	mbase::Real num_channels_in_state(mbase::Size_t state) const;
	mbase::Real ComputeChannelStateTimeConstant() const;

	virtual mbase::Mreturn step_current();
	virtual mbase::Real open_channels() const;
	virtual mbase::Real compute_conductance();
	void show_param() const;
	void printProb(string fileName) {
		for (mbase::Real v = 20; v < 130; v += 0.005) {
			cout
					<< probability_matrix_map[fileName]->getTransitionProbability(
							v, 2, 1) << std::endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static mbase::Real lua_get_ntreal(lua_State* L, string name);
	static bool initTableLookUp;
	static std::vector<string> initialised_probability_matrices;
	mbase::Real baseTemp;

};
}
#endif /* _mcore_lua_based_stochastic_MULTI_CURRENT_O_H_ */
