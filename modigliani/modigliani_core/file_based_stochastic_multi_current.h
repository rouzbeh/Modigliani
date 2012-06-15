/**
 * @file file_based_stochastic_multi_current.h
 * @author man210
 */

#ifndef NTBP_file_based_stochastic_MULTI_CURRENT_O_H_
#define NTBP_file_based_stochastic_MULTI_CURRENT_O_H_

#include "multi_current.h"
#include "ion_channels.h"

#ifdef __MINGW32__
#include <jsoncpp/json.h>
#else
#include <json/json.h>
#endif
#include <fstream>
#include <iostream>

namespace modigliani_core {
class File_based_stochastic_multi_current: public Multi_current {
public:
	File_based_stochastic_multi_current(modigliani_base::Real newArea,
			modigliani_base::Real newDensity, modigliani_base::Real newConductivity,
			modigliani_base::Real reversalPotential, modigliani_base::Real newTimeStep,
			modigliani_base::Real newTemperature, std::string fileName);
	virtual ~File_based_stochastic_multi_current();

	static void load_file(std::string fileName, double temperature,
			double time_step);
	static std::map<std::string, Transition_rate_matrix*> probability_matrix_map;
	static std::map<std::string, int> number_of_states_map;
	static std::map<std::string, double> base_temperature_map;
	static std::map<std::string, std::vector<int> > open_states_map;
	virtual modigliani_base::ReturnEnum step_current();
	virtual modigliani_base::Real open_channels() const;
	virtual modigliani_base::Real compute_conductance();
	modigliani_base::Real num_channels_in_state(modigliani_base::Size state) const;
	modigliani_base::Real ComputeChannelStateTimeConstant() const;
	void show_param() const;
	void printProb(std::string fileName) {
		for (modigliani_base::Real v = 20; v < 130; v += 0.005) {
			std::cout
					<< probability_matrix_map[fileName]->getTransitionProbability(
							v, 2, 1) << std::endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static bool initTableLookUp;
	static std::vector<std::string> initialised_probability_matrices;
	modigliani_base::Real baseTemp;

};
}
#endif /* NTBP_file_based_stochastic_MULTI_CURRENT_O_H_ */
