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
	Lua_based_stochastic_multi_current(modigliani_base::Real newArea,
			modigliani_base::Real newDensity, modigliani_base::Real newConductivity,
			modigliani_base::Real reversalPotential, modigliani_base::Real newTimeStep,
			modigliani_base::Real newTemperature, string fileName);
	virtual ~Lua_based_stochastic_multi_current();

	static void load_file(string fileName, double temperature,
			double time_step);
	static map<string, Transition_rate_matrix*> probability_matrix_map;
	static map<string, modigliani_base::Size> number_of_states_map;
	static map<string, double> base_temperature_map;
	static map<string, std::vector<int> > open_states_map;
	modigliani_base::Real num_channels_in_state(modigliani_base::Size state) const;
	modigliani_base::Real ComputeChannelStateTimeConstant() const;

	virtual modigliani_base::ReturnEnum step_current();
	virtual modigliani_base::Real open_channels() const;
	virtual modigliani_base::Real compute_conductance();
	void show_param() const;
	void printProb(string fileName) {
		for (modigliani_base::Real v = 20; v < 130; v += 0.005) {
			cout
					<< probability_matrix_map[fileName]->getTransitionProbability(
							v, 2, 1) << std::endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static modigliani_base::Real lua_get_ntreal(lua_State* L, string name);
	static bool initTableLookUp;
	static std::vector<string> initialised_probability_matrices;
	modigliani_base::Real baseTemp;

};
}
#endif /* _mcore_lua_based_stochastic_MULTI_CURRENT_O_H_ */
