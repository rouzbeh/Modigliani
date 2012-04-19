/*
 * File_based_stochastic_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_file_based_stochastic_MULTI_CURRENT_O_H_
#define NTBP_file_based_stochastic_MULTI_CURRENT_O_H_

#include "Multi_current.h"
#include "Ion_channels.h"

#include <json/json.h>
#include <fstream>
#include <iostream>

namespace mcore {
class File_based_stochastic_multi_current: public Multi_current {
public:
	File_based_stochastic_multi_current(NTreal newArea,
			NTreal newDensity, NTreal newConductivity, NTreal newVBase,
			NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature,
			string fileName);
	virtual ~File_based_stochastic_multi_current();

	static void load_file(string fileName, double temperature,
			double time_step);
	static map<string, NTBP_transition_rate_matrix_o*> probability_matrix_map;
	static map<string, int> number_of_states_map;
	static map<string, double> base_temperature_map;
	static map<string, vector<int> > open_states_map;
	virtual NTreturn step_current()
override	;
	virtual NTreal open_channels() const override;
	virtual NTreal compute_conductance() override;
	NTreal NumChannelsInState(NTsize state) const;
	NTreal ComputeChannelStateTimeConstant() const;
	void ShowParam() const;
	void printProb(string fileName) {
		for (NTreal v =20; v<130; v+=0.005) {
			cout << probability_matrix_map[fileName]->getTransitionProbability(v, 2, 1) << endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static bool initTableLookUp;
	static vector<string> initialised_probability_matrices;
	NTreal baseTemp;

};}
#endif /* NTBP_file_based_stochastic_MULTI_CURRENT_O_H_ */
