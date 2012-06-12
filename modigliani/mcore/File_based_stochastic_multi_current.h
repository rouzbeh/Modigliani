/**
 * @file File_based_stochastic_multi_current.h
 * @author man210
 */

#ifndef NTBP_file_based_stochastic_MULTI_CURRENT_O_H_
#define NTBP_file_based_stochastic_MULTI_CURRENT_O_H_

#include "Multi_current.h"
#include "Ion_channels.h"

#ifdef __MINGW32__
#include <jsoncpp/json.h>
#else
#include <json/json.h>
#endif
#include <fstream>
#include <iostream>

namespace mcore {
class File_based_stochastic_multi_current: public Multi_current {
public:
	File_based_stochastic_multi_current(mbase::Real newArea,
			mbase::Real newDensity, mbase::Real newConductivity,
			mbase::Real reversalPotential, mbase::Real newTimeStep,
			mbase::Real newTemperature, std::string fileName);
	virtual ~File_based_stochastic_multi_current();

	static void load_file(std::string fileName, double temperature,
			double time_step);
	static std::map<std::string, Transition_rate_matrix*> probability_matrix_map;
	static std::map<std::string, int> number_of_states_map;
	static std::map<std::string, double> base_temperature_map;
	static std::map<std::string, std::vector<int> > open_states_map;
	virtual mbase::Mreturn step_current();
	virtual mbase::Real open_channels() const;
	virtual mbase::Real compute_conductance();
	mbase::Real num_channels_in_state(mbase::Size_t state) const;
	mbase::Real ComputeChannelStateTimeConstant() const;
	void show_param() const;
	void printProb(std::string fileName) {
		for (mbase::Real v = 20; v < 130; v += 0.005) {
			std::cout
					<< probability_matrix_map[fileName]->getTransitionProbability(
							v, 2, 1) << std::endl;
		}
	}

	Ion_channels * channelsPtr;

private:
	static bool initTableLookUp;
	static std::vector<std::string> initialised_probability_matrices;
	mbase::Real baseTemp;

};
}
#endif /* NTBP_file_based_stochastic_MULTI_CURRENT_O_H_ */
