/*
 * Lua_based_deterministic_multi_current_o.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: man210
 */

#include "Lua_based_deterministic_multi_current.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_hranvier_sodium_multi_current_o */

Lua_based_deterministic_multi_current::Lua_based_deterministic_multi_current(NTreal newArea,
		NTreal newDensity, NTreal newConductivity, NTreal newVBase,
		NTreal newReversalPotential, NTreal newTimeStep, NTreal newTemperature,
		string new_lua_script) :
		Multi_current(newReversalPotential /* in mV */,
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
Lua_based_deterministic_multi_current::Lua_based_deterministic_multi_current(
		const Lua_based_deterministic_multi_current & original) :
		Multi_current(original._reversalPotential(), original._density(),
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

const Lua_based_deterministic_multi_current&
Lua_based_deterministic_multi_current::operator=(
		const Lua_based_deterministic_multi_current & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
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
	return (*this);
}

/* ***      DESTRUCTOR		***/
Lua_based_deterministic_multi_current::~Lua_based_deterministic_multi_current() {
	lua_close(L);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline NTreturn Lua_based_deterministic_multi_current::step_current() {
	switch (_simulationMode()) {
	case NTBP_DETERMINISTIC: {
		lua_getglobal(L, "step_current");
		/* the first argument */
		lua_pushnumber(L, voltage);

		/* call the function with 1
		 argument, return 0 result */
		lua_call(L, 1, 0);

		return (NT_SUCCESS);
	}

		break;
	default:
		cerr
				<< "Lua_based_deterministic_multi_current::StepCurrent - ERROR : Unsupported simulation mode."
				<< endl;
		return (NT_PARAM_UNSUPPORTED);
		break;
	}
	return (NT_FAIL);
}

/**  */
/** No descriptions */
inline NTreal Lua_based_deterministic_multi_current::open_channels() const {
	lua_getglobal(L, "open_channels");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	NTreal count = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return (count * NumChannels());
}

/**  */
/** No descriptions */
NTreal Lua_based_deterministic_multi_current::NumChannelsInState(
		NTsize __attribute__((__unused__)) state) const {
	cerr << "Deterministic channel does not have states" << endl;
	return (0);
}

inline NTreal Lua_based_deterministic_multi_current::compute_conductance() {
	lua_getglobal(L, "compute_conductance");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	NTreal conduc = lua_tonumber(L, -1);
	lua_pop(L, 1);
	NT_ASSERT(conduc == conduc);
	return (Set_conductance(
			conduc * _maxConductivity() * _area() /* muMeter^2 */* 1.0e-8));
}

void Lua_based_deterministic_multi_current::ShowParam() const {
	cout << "Na channel parameters:" << endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << endl;
	cout << "Channel density [1/muMeter^2]" << _area() << endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << endl;
}

