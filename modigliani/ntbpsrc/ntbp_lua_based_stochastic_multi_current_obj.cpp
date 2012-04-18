/*
 * NTBP_lua_based_stochastic_multi_current_o.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "ntbp_lua_based_stochastic_multi_current_obj.h"

bool NTBP_lua_based_stochastic_multi_current_o::initTableLookUp = false;
map<string, NTBP_transition_rate_matrix_o*> NTBP_lua_based_stochastic_multi_current_o::probability_matrix_map;
map<string, NTsize> NTBP_lua_based_stochastic_multi_current_o::number_of_states_map;
map<string, double> NTBP_lua_based_stochastic_multi_current_o::base_temperature_map;
map<string, vector<int> > NTBP_lua_based_stochastic_multi_current_o::open_states_map;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

NTBP_lua_based_stochastic_multi_current_o::NTBP_lua_based_stochastic_multi_current_o(
		NTreal newArea, NTreal newDensity, NTreal newConductivity,
		NTreal newVBase, NTreal newReversalPotential, NTreal newTimeStep,
		NTreal newTemperature, string fileName) :
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
//NTBP_lua_based_stochastic_multi_current_o::NTBP_lua_based_stochastic_multi_current_o(
//		const NTBP_hranvier_sodium_multi_current_o & original) :
//	q10h(original.q10h), q10m(original.q10m), NTBP_multi_current_o(
//			original._reversalPotential(), original._density(),
//			original._area(), original._conductivity()) {
//	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//}
//
//const NTBP_hranvier_sodium_multi_current_o&
//NTBP_lua_based_stochastic_multi_current_o::operator=(
//		const NTBP_hranvier_sodium_multi_current_o & right) {
//	if (this == &right)
//		return (*this); // Gracefully handle self assignment
//	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//	return (*this);
//}
//
/* ***      DESTRUCTOR		***/
NTBP_lua_based_stochastic_multi_current_o::~NTBP_lua_based_stochastic_multi_current_o() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
void NTBP_lua_based_stochastic_multi_current_o::load_file(string fileName,
		double temperature, double time_step) {
	cout << "Getting probabilities from " << fileName << endl;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, fileName.c_str());

	base_temperature_map[fileName] = lua_get_ntreal(L, "base_temp");

	number_of_states_map[fileName] = lua_get_ntreal(L, "number_states");

	open_states_map[fileName] = vector<int>();

	lua_getglobal(L, "open_states");
	/* table is in the stack at index 't' */
	lua_pushnil(L); /* first key */
	while (lua_next(L, -2) != 0) {
		/* uses 'key' (at index -2) and 'value' (at index -1) */
		int found = lua_tonumber(L, -1);
		open_states_map[fileName].push_back(found);
		lua_pop(L, 1);
	}

	double minV = lua_get_ntreal(L, "minV");
	double maxV = lua_get_ntreal(L, "maxV");
	double step = lua_get_ntreal(L, "step");

	//const Json::Value transitions = root["transitions"];

	probability_matrix_map[fileName] = new NTBP_transition_rate_matrix_o(
			number_of_states_map[fileName], minV, maxV, step);

	NTreal length = floor((maxV - minV) / step + 0.5) + 1;
	for (NTsize i = 1; i <= number_of_states_map[fileName]; ++i) {
		for (NTsize j = 1; j <= number_of_states_map[fileName]; ++j) {
			for (NTsize k = 0; k < length; k++) {
				NTreal voltage = minV + step * k;
				lua_getglobal(L, "get_probability");
				lua_pushnumber(L, i);
				lua_pushnumber(L, j);
				lua_pushnumber(L, voltage);
				lua_call(L, 3, 1);
				double base_probability = lua_tonumber(L, -1);
				lua_pop(L, 1);
				lua_getglobal(L, "get_q10");
				lua_pushnumber(L, i);
				lua_pushnumber(L, j);
				lua_call(L, 2, 1);
				double prob_q10 = lua_tonumber(L, -1);
				lua_pop(L, 1);
				double probability = NTBP_temperature_rate_relation(temperature,
						base_temperature_map[fileName] /* C */, prob_q10)
						* base_probability * time_step;
				probability_matrix_map[fileName]->setTransitionProbability(voltage,
						i , j , probability);
			}
		}
	}
	lua_close(L);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_lua_based_stochastic_multi_current_o::step_current() {
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
				<< "NTBP_lua_based_stochastic_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return (NT_PARAM_UNSUPPORTED);
		break;
	}
	return (NT_FAIL);
}

/**  */
/** No descriptions */
inline NTreal NTBP_lua_based_stochastic_multi_current_o::open_channels() const {
	return (channelsPtr->NumOpen());
}

/**  */
/** No descriptions */
inline NTreal NTBP_lua_based_stochastic_multi_current_o::NumChannelsInState(
		NTsize state) const {
	return (channelsPtr->numChannelsInState(state));
}

inline NTreal NTBP_lua_based_stochastic_multi_current_o::compute_conductance() {
	return (Set_conductance(channelsPtr->NumOpen() * conductivity));
}

inline NTreal NTBP_lua_based_stochastic_multi_current_o::ComputeChannelStateTimeConstant() const {
	return (channelsPtr->ComputeChannelStateTimeConstant(voltage));
}

void NTBP_lua_based_stochastic_multi_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}

NTreal NTBP_lua_based_stochastic_multi_current_o::lua_get_ntreal(lua_State* L,
		string name) {
	lua_getglobal(L, name.c_str());
	NTreal ret = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return (ret);
}
