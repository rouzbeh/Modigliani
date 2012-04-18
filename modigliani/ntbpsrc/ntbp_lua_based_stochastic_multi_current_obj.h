/*
 * NTBP_lua_based_stochastic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_lua_based_stochastic_MULTI_CURRENT_O_H_
#define NTBP_lua_based_stochastic_MULTI_CURRENT_O_H_


#include "ntbp_multi_current_obj.h"
#include "ntbp_ion_channels_obj.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <map>
using namespace std;


class NTBP_lua_based_stochastic_multi_current_o: public NTBP_multi_current_o {
public:
	NTBP_lua_based_stochastic_multi_current_o(NTreal newArea, NTreal newDensity, NTreal newConductivity,
			NTreal newVBase, NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature, string fileName);
	virtual ~NTBP_lua_based_stochastic_multi_current_o();

	static void load_file(string fileName, double temperature, double time_step);
	static map<string,NTBP_transition_rate_matrix_o*> probability_matrix_map;
	static map<string,NTsize> number_of_states_map;
	static map<string,double> base_temperature_map;
	static map<string,vector<int> > open_states_map;
	NTreal NumChannelsInState(NTsize state) const override;
	NTreal ComputeChannelStateTimeConstant() const override;

	virtual NTreturn step_current() override;
	virtual NTreal open_channels() const override;
	virtual NTreal compute_conductance() override;
	void ShowParam() const override;
	void printProb(string fileName){
		for (NTreal v =20; v<130; v+=0.005){
			cout << probability_matrix_map[fileName]->getTransitionProbability(v, 2, 1) << endl;
		}
	}

	NTBP_ion_channels_o * channelsPtr;

private:
	static NTreal lua_get_ntreal(lua_State* L, string name);
	static bool initTableLookUp;
	static vector<string> initialised_probability_matrices;
	NTreal lua_get_ntreal(string method);
	NTreal baseTemp;

};

#endif /* NTBP_lua_based_stochastic_MULTI_CURRENT_O_H_ */
