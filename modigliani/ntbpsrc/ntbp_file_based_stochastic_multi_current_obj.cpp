/*
 * NTBP_file_based_stochastic_multi_current_o.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "ntbp_file_based_stochastic_multi_current_obj.h"

bool NTBP_file_based_stochastic_multi_current_o::initTableLookUp = false;
map<string, NTBP_transition_rate_matrix_o*> NTBP_file_based_stochastic_multi_current_o::probability_matrix_map;
map<string, int> NTBP_file_based_stochastic_multi_current_o::number_of_states_map;
map<string, double> NTBP_file_based_stochastic_multi_current_o::base_temperature_map;
map<string, vector<int> > NTBP_file_based_stochastic_multi_current_o::open_states_map;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

NTBP_file_based_stochastic_multi_current_o::NTBP_file_based_stochastic_multi_current_o(NTreal newArea,
		NTreal newDensity, NTreal newConductivity, NTreal newVBase,
		NTreal newReversalPotential, NTreal newTimeStep, NTreal newTemperature,
		string fileName) :
		NTBP_multi_current_o(newReversalPotential /* in mV */,
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

	NT_ASSERT(number_of_states_map[fileName]>0);

	if (false == initTableLookUp) {
		initTableLookUp = true;
	}
	channelsPtr = new NTBP_ion_channels_o(_numChannels(),
			number_of_states_map[fileName], probability_matrix_map[fileName],
			newTimeStep);

	for (unsigned int i = 0; i < open_states_map[fileName].size(); i++) {
		channelsPtr->setAsOpenState(open_states_map[fileName][i]);
	}

}
//
///* ***      COPY AND ASSIGNMENT	***/
//NTBP_file_based_stochastic_multi_current_o::NTBP_file_based_stochastic_multi_current_o(
//		const NTBP_hranvier_sodium_multi_current_o & original) :
//	q10h(original.q10h), q10m(original.q10m), NTBP_multi_current_o(
//			original._reversalPotential(), original._density(),
//			original._area(), original._conductivity()) {
//	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//}
//
//const NTBP_hranvier_sodium_multi_current_o&
//NTBP_file_based_stochastic_multi_current_o::operator=(
//		const NTBP_hranvier_sodium_multi_current_o & right) {
//	if (this == &right)
//		return *this; // Gracefully handle self assignment
//	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//	return *this;
//}
//
/* ***      DESTRUCTOR		***/
NTBP_file_based_stochastic_multi_current_o::~NTBP_file_based_stochastic_multi_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */

void NTBP_file_based_stochastic_multi_current_o::load_file(string fileName,
		double temperature, double time_step) {
	cout << "Loading probabilities from " << fileName << endl;
	Json::Value root; // will contains the root value after parsing.
	Json::Reader reader;
	ifstream config_doc;
	config_doc.open(fileName.c_str(), ifstream::in);
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

	open_states_map[fileName] = vector<int>();
	const Json::Value open_states = root["open_states"];
	for (unsigned int index = 0; index < open_states.size(); ++index) { // Iterates over the sequence elements.
		open_states_map[fileName].push_back(open_states[index].asInt());
	}

	double minV = root.get("minV", 0).asDouble();
	double maxV = root.get("maxV", 0).asDouble();
	double step = root.get("step", 0).asDouble();

	const Json::Value transitions = root["transitions"];

	probability_matrix_map[fileName] = new NTBP_transition_rate_matrix_o(
			number_of_states_map[fileName], minV, maxV, step);


	for (unsigned int index = 0; index < transitions.size(); ++index) { // Iterates over the sequence elements.
		double q10 = transitions[index].get("q10", 1).asDouble();
		double base_probability =
				transitions[index].get("probability", 0).asDouble();
		double probability = NTBP_TemperatureRateRelation(temperature,
				base_temperature_map[fileName] /* C */, q10) * base_probability
				* time_step;

		//NT_ASSERT(probability>0 && probability<=1);
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
inline NTreturn NTBP_file_based_stochastic_multi_current_o::StepCurrent() {
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
		cerr
				<< "NTBP_file_based_stochastic_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return (NT_PARAM_UNSUPPORTED);
		break;
	}
	return (NT_FAIL);
}

/**  */
/** No descriptions */
inline NTreal NTBP_file_based_stochastic_multi_current_o::OpenChannels() const {
	return (channelsPtr->NumOpen());
}

/**  */
/** No descriptions */
inline NTreal NTBP_file_based_stochastic_multi_current_o::OpenChannelsRatio() const {
	NTreal open = OpenChannels();
	return (((NTreal) (open * 100)) / NumChannels());
}

/**  */
/** No descriptions */
inline NTreal NTBP_file_based_stochastic_multi_current_o::NumChannelsInState(
		NTsize state) const {
	return (channelsPtr->numChannelsInState(state));
}

inline NTreal NTBP_file_based_stochastic_multi_current_o::ComputeConductance() {
	return (Set_conductance(channelsPtr->NumOpen() * conductivity));
}

inline NTreal NTBP_file_based_stochastic_multi_current_o::ComputeChannelStateTimeConstant() const {
	return (channelsPtr->ComputeChannelStateTimeConstant(voltage));
}

void NTBP_file_based_stochastic_multi_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}

