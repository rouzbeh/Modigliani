/*
 * NTBP_file_based_multi_current_o.h
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#ifndef NTBP_FILE_BASED_MULTI_CURRENT_O_H_
#define NTBP_FILE_BASED_MULTI_CURRENT_O_H_

#include "ntbp_multi_current_obj.h"
#include "ntbp_ion_channels_obj.h"
#include "json/json.h"

#include <fstream>
#include <iostream>
using namespace std;


class NTBP_file_based_multi_current_o: public NTBP_multi_current_o {
public:
	NTBP_file_based_multi_current_o(NTreal newArea, NTreal newDensity, NTreal newConductivity,
			NTreal newVBase, NTreal reversalPotential, NTreal newTimeStep, NTreal newTemperature, string fileName);
	virtual ~NTBP_file_based_multi_current_o();

	static void load_file(string fileName, double temperature, double time_step);
	static map<string,NTBP_transition_rate_matrix_o*> probability_matrix_map;
	static map<string,int> number_of_states_map;
	static map<string,double> base_temperature_map;
	static map<string,vector<int> > open_states_map;
	NTreturn StepCurrent();
	NTreal OpenChannels() const;
	NTreal OpenChannelsRatio() const;
	NTreal ComputeConductance();
	NTreal ComputeChannelStateTimeConstant() const;
	void ShowParam() const;

private:
	static bool initTableLookUp;
	static vector<string> initialised_probability_matrices;
	NTreal baseTemp;

};

#endif /* NTBP_FILE_BASED_MULTI_CURRENT_O_H_ */
