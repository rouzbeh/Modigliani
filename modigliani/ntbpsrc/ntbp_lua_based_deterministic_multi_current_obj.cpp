/*
 * NTBP_lua_based_deterministic_multi_current_o.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "ntbp_lua_based_deterministic_multi_current_obj.h"

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

NTBP_lua_based_deterministic_multi_current_o::NTBP_lua_based_deterministic_multi_current_o(NTreal newArea,
		NTreal newDensity, NTreal newConductivity, NTreal newVBase,
		NTreal newReversalPotential, NTreal newTimeStep, NTreal newTemperature,
		string new_lua_script) :
		NTBP_multi_current_o(newReversalPotential /* in mV */,
				newDensity /* channels per mu^2 */, newArea /* in mu^2 */,
				newConductivity /* in mS per channel  */, newVBase /* mV */
				) {

	UpdateNumChannels(); //TODO
	lua_script = new_lua_script;
	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, lua_script.c_str());
	lua_getglobal(L, "set_timestep");
	/* the first argument */
	lua_pushnumber(L, newTimeStep);

	/* call the function with 1
	 argument, return 0 result */
	lua_call(L, 1, 0);
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_lua_based_deterministic_multi_current_o::NTBP_lua_based_deterministic_multi_current_o(
		const NTBP_lua_based_deterministic_multi_current_o & original) :
		NTBP_multi_current_o(original._reversalPotential(), original._density(),
				original._area(), original._conductivity()) {
	UpdateNumChannels();

	setTimeStep(original._timeStep());
	Set_temperature(original._temperature());
	lua_script = original.lua_script;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, lua_script.c_str());
	lua_getglobal(L, "set_timestep");
	/* the first argument */
	lua_pushnumber(L, _timeStep());

	/* call the function with 1
	 argument, return 0 result */
	lua_call(L, 1, 0);
}

const NTBP_lua_based_deterministic_multi_current_o&
NTBP_lua_based_deterministic_multi_current_o::operator=(
		const NTBP_lua_based_deterministic_multi_current_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	setTimeStep(right._timeStep());
	Set_temperature(_temperature());
	lua_script = right.lua_script;
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, lua_script.c_str());
	lua_getglobal(L, "set_timestep");
	/* the first argument */
	lua_pushnumber(L, _timeStep());

	/* call the function with 1
	 argument, return 0 result */
	lua_call(L, 1, 0);
	return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_lua_based_deterministic_multi_current_o::~NTBP_lua_based_deterministic_multi_current_o() {
	lua_close(L);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn NTBP_lua_based_deterministic_multi_current_o::StepCurrent() {
	switch (_simulationMode()) {
	case NTBP_DETERMINISTIC: {
		lua_getglobal(L, "step_current");
		/* the first argument */
		lua_pushnumber(L, voltage);

		/* call the function with 1
		 argument, return 0 result */
		lua_call(L, 1, 0);

		return NT_SUCCESS;
	}

		break;
	default:
		cerr
				<< "NTBP_lua_based_deterministic_multi_current_o::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return NT_PARAM_UNSUPPORTED;
		break;
	}
	return NT_FAIL;
}

/**  */
/** No descriptions */
inline NTreal NTBP_lua_based_deterministic_multi_current_o::OpenChannels() const {
	lua_getglobal(L, "open_channels");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	NTreal count = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return count * NumChannels();
}

/**  */
/** No descriptions */
NTreal NTBP_lua_based_deterministic_multi_current_o::NumChannelsInState(
		NTsize __attribute__((__unused__)) state) const {
	cerr << "Deterministic channel does not have states" << endl;
	return 0;
}

/**  */
/** No descriptions */
inline NTreal NTBP_lua_based_deterministic_multi_current_o::OpenChannelsRatio() const {
	NTreal open = OpenChannels();
	return ((NTreal) (open * 100)) / NumChannels();
}

inline NTreal NTBP_lua_based_deterministic_multi_current_o::ComputeConductance() {
	lua_getglobal(L, "compute_conductance");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	NTreal conduc = lua_tonumber(L, -1);
	lua_pop(L, 1);
	NT_ASSERT(conduc == conduc);
	return Set_conductance(
			conduc * _maxConductivity() * _area() /* muMeter^2 */* 1.0e-8);
}

void NTBP_lua_based_deterministic_multi_current_o::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}
