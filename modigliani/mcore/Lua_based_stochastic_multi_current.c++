/*
 * Lua_based_stochastic_multi_current.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "Lua_based_stochastic_multi_current.h"

using namespace mcore;

bool Lua_based_stochastic_multi_current::initTableLookUp = false;
map<string, NTBP_transition_rate_matrix_o*> Lua_based_stochastic_multi_current::probability_matrix_map;
map<string, mbase::Msize> Lua_based_stochastic_multi_current::number_of_states_map;
map<string, double> Lua_based_stochastic_multi_current::base_temperature_map;
map<string, std::vector<int> > Lua_based_stochastic_multi_current::open_states_map;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

Lua_based_stochastic_multi_current::Lua_based_stochastic_multi_current(
		mbase::Real newArea, mbase::Real newDensity, mbase::Real newConductivity,
		mbase::Real newVBase, mbase::Real newReversalPotential, mbase::Real newTimeStep,
		mbase::Real newTemperature, string fileName) :
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
//Lua_based_stochastic_multi_current::Lua_based_stochastic_multi_current(
//		const NTBP_hranvier_sodium_multi_current_o & original) :
//	q10h(original.q10h), q10m(original.q10m), NTBP_multi_current_o(
//			original._reversalPotential(), original._density(),
//			original._area(), original._conductivity()) {
//	channelsPtr = new NTBP_ion_channels_o(original._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//}
//
//const NTBP_hranvier_sodium_multi_current_o&
//Lua_based_stochastic_multi_current::operator=(
//		const NTBP_hranvier_sodium_multi_current_o & right) {
//	if (this == &right)
//		return (*this); // Gracefully handle self assignment
//	channelsPtr = new NTBP_ion_channels_o(right._numChannels(), 8);
//	channelsPtr->setAsOpenState(4);
//	return (*this);
//}
//
/* ***      DESTRUCTOR		***/
Lua_based_stochastic_multi_current::~Lua_based_stochastic_multi_current() {
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */
void Lua_based_stochastic_multi_current::load_file(string fileName,
		double temperature, double time_step) {
	cout << "Getting probabilities from " << fileName << std::endl;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, fileName.c_str());

	base_temperature_map[fileName] = lua_get_ntreal(L, "base_temp");

	number_of_states_map[fileName] = lua_get_ntreal(L, "number_states");

	open_states_map[fileName] = std::vector<int>();

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

	mbase::Real length = floor((maxV - minV) / step + 0.5) + 1;
	for (mbase::Msize i = 1; i <= number_of_states_map[fileName]; ++i) {
		for (mbase::Msize j = 1; j <= number_of_states_map[fileName]; ++j) {
			for (mbase::Msize k = 0; k < length; k++) {
				mbase::Real voltage = minV + step * k;
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
inline mbase::Mreturn Lua_based_stochastic_multi_current::step_current() {
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
				<< "Lua_based_stochastic_multi_current::StepCurrent - ERROR : Unsupported simulation mode."
				<< std::endl;
		return (mbase::M_PARAM_UNSUPPORTED);
		break;
	}
	return (mbase::M_FAIL);
}

/**  */
/** No descriptions */
inline mbase::Real Lua_based_stochastic_multi_current::open_channels() const {
	return (channelsPtr->NumOpen());
}

/**  */
/** No descriptions */
inline mbase::Real Lua_based_stochastic_multi_current::num_channels_in_state(
		mbase::Msize state) const {
	return (channelsPtr->numChannelsInState(state));
}

inline mbase::Real Lua_based_stochastic_multi_current::compute_conductance() {
	return (Set_conductance(channelsPtr->NumOpen() * conductivity));
}

inline mbase::Real Lua_based_stochastic_multi_current::ComputeChannelStateTimeConstant() const {
	return (channelsPtr->ComputeChannelStateTimeConstant(voltage));
}

void Lua_based_stochastic_multi_current::show_param() const {
	cout << "Na channel parameters:" << std::endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << std::endl;
	cout << "Channel density [1/muMeter^2]" << _area() << std::endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << std::endl;
}

mbase::Real Lua_based_stochastic_multi_current::lua_get_ntreal(lua_State* L,
		string name) {
	lua_getglobal(L, name.c_str());
	mbase::Real ret = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return (ret);
}
