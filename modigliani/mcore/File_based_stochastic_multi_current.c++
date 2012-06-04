/*
 * File_based_stochastic_multi_current.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "File_based_stochastic_multi_current.h"

using namespace mcore;

bool File_based_stochastic_multi_current::initTableLookUp = false;
std::map<std::string, Transition_rate_matrix*> File_based_stochastic_multi_current::probability_matrix_map;
std::map<std::string, int> File_based_stochastic_multi_current::number_of_states_map;
std::map<std::string, double> File_based_stochastic_multi_current::base_temperature_map;
std::map<std::string, std::vector<int> > File_based_stochastic_multi_current::open_states_map;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

File_based_stochastic_multi_current::File_based_stochastic_multi_current(mbase::Real newArea,
		mbase::Real newDensity, mbase::Real newConductivity, mbase::Real newVBase,
		mbase::Real newReversalPotential, mbase::Real newTimeStep, mbase::Real newTemperature,
		std::string fileName) :
		Multi_current(newReversalPotential /* in mV */,
				newDensity /* channels per mu^2 */, newArea /* in mu^2 */,
				newConductivity /* in mS per channel  */, newVBase /* mV */
				) {

	UpdateNumChannels(); //TODO

	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	if (number_of_states_map[fileName] == 0) {
		load_file(fileName, newTemperature, newTimeStep);
	}
	baseTemp = base_temperature_map[fileName];

	M_ASSERT(number_of_states_map[fileName]>0);

	if (false == initTableLookUp) {
		initTableLookUp = true;
	}
	channelsPtr = new Ion_channels(_numChannels(),
			number_of_states_map[fileName], probability_matrix_map[fileName],
			newTimeStep);

	for (unsigned int i = 0; i < open_states_map[fileName].size(); i++) {
		channelsPtr->setAsOpenState(open_states_map[fileName][i]);
	}

}
//
///* ***      COPY AND ASSIGNMENT	***/
//File_based_stochastic_multi_current::File_based_stochastic_multi_current(
//		const NTBP_hranvier_sodium_multi_current_o & original) :
//	q10h(original.q10h), q10m(original.q10m), NTBP_multi_current_o(
//			original._reversalPotential(), original._density(),
//			original._area(), original._conductivity()) {
//	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//}
//
//const NTBP_hranvier_sodium_multi_current_o&
//File_based_stochastic_multi_current::operator=(
//		const NTBP_hranvier_sodium_multi_current_o & right) {
//	if (this == &right)
//		return (*this); // Gracefully handle self assignment
//	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//	return (*this);
//}
//
/* ***      DESTRUCTOR		***/
File_based_stochastic_multi_current::~File_based_stochastic_multi_current() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */

void File_based_stochastic_multi_current::load_file(std::string fileName,
		double temperature, double time_step) {
	std::cout << "Loading probabilities from " << fileName << std::endl;
	Json::Value root; // will contains the root value after parsing.
	Json::Reader reader;
	std::ifstream config_doc;
	config_doc.open(fileName.c_str(), std::ifstream::in);
	bool parsingSuccessful = reader.parse(config_doc, root);
	if (!parsingSuccessful) {
		// report to the user the failure and their locations in the document.
		std::cout << "Failed to parse configuration\n"
				<< reader.getFormatedErrorMessages();
		exit(1);
	}

	base_temperature_map[fileName] =
			root.get("base_temperature", 20).asDouble();
	number_of_states_map[fileName] = root.get("number_of_states", 0).asInt();

	open_states_map[fileName] = std::vector<int>();
	const Json::Value open_states = root["open_states"];
	for (unsigned int index = 0; index < open_states.size(); ++index) { // Iterates over the sequence elements.
		open_states_map[fileName].push_back(open_states[index].asInt());
	}

	double minV = root.get("minV", 0).asDouble();
	double maxV = root.get("maxV", 0).asDouble();
	double step = root.get("step", 0).asDouble();

	const Json::Value transitions = root["transitions"];

	probability_matrix_map[fileName] = new Transition_rate_matrix(
			number_of_states_map[fileName], minV, maxV, step);


	for (unsigned int index = 0; index < transitions.size(); ++index) { // Iterates over the sequence elements.
		double q10 = transitions[index].get("q10", 1).asDouble();
		double base_probability =
				transitions[index].get("probability", 0).asDouble();
		double probability = NTBP_temperature_rate_relation(temperature,
				base_temperature_map[fileName] /* C */, q10) * base_probability
				* time_step;

		//M_ASSERT(probability>0 && probability<=1);
		// Converted voltage is real_voltage
		double converted_voltage =
				transitions[index].get("voltage", 0).asDouble();
		probability_matrix_map[fileName]->setTransitionProbability(
				converted_voltage, transitions[index].get("start", 1).asInt(),
				transitions[index].get("stop", 1).asInt(), probability);
	}
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline mbase::Mreturn File_based_stochastic_multi_current::step_current() {
	switch (_simulationMode()) {
	case NTBP_BINOMIALPOPULATION: {
		return (channelsPtr->BinomialStep(voltage));
	}
		break;
	case NTBP_GILLESPIE: {
		return (channelsPtr->GillespieStep(voltage));
	}
		break;
	case NTBP_DETERMINISTIC: {
		return (channelsPtr->DeterministicStep(voltage));
	}

		break;
	default:
		std::cerr
				<< "File_based_stochastic_multi_current::StepCurrent - ERROR : Unsupported simulation mode."
				<< std::endl;
		return (mbase::M_PARAM_UNSUPPORTED);
		break;
	}
	return (mbase::M_FAIL);
}

/**  */
/** No descriptions */
inline mbase::Real File_based_stochastic_multi_current::open_channels() const {
	return (channelsPtr->NumOpen());
}

/**  */
/** No descriptions */
inline mbase::Real File_based_stochastic_multi_current::num_channels_in_state(
		mbase::Size_t state) const {
	return (channelsPtr->numChannelsInState(state));
}

inline mbase::Real File_based_stochastic_multi_current::compute_conductance() {
	return (Set_conductance(channelsPtr->NumOpen() * conductivity));
}

inline mbase::Real File_based_stochastic_multi_current::ComputeChannelStateTimeConstant() const {
	return (channelsPtr->ComputeChannelStateTimeConstant(voltage));
}

void File_based_stochastic_multi_current::show_param() const {
	std::cout << "Na channel parameters:" << std::endl;
	std::cout << "Single channel conductivity [nA]" << _conductivity() << std::endl;
	std::cout << "Channel density [1/muMeter^2]" << _area() << std::endl;
	std::cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << std::endl;
}
