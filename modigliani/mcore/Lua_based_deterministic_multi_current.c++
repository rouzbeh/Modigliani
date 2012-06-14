/**
 * @file Lua_based_deterministic_multi_current.c++
 * @author Ali Neishabouri
 */

#include "Lua_based_deterministic_multi_current.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
Lua_based_deterministic_multi_current::Lua_based_deterministic_multi_current(
		modigliani_base::Real newArea, modigliani_base::Real newDensity,
		modigliani_base::Real newConductivity, modigliani_base::Real newReversalPotential,
		modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
		string new_lua_script) :
		Multi_current(newReversalPotential /* in mV */,
				newDensity /* channels per mu^2 */, newArea /* in mu^2 */,
				newConductivity /* in mS per channel  */
				) {

	UpdateNumChannels(); //TODO
	lua_script = new_lua_script;
	setTimeStep(newTimeStep);
	Set_temperature(newTemperature);
	L = luaL_newstate();
	luaL_openlibs(L);
	luaL_dofile(L, lua_script.c_str());
	stepV = lua_get_ntreal(L, "step");
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

Lua_based_deterministic_multi_current&
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
inline modigliani_base::ReturnEnum Lua_based_deterministic_multi_current::step_current() {
	modigliani_base::Real rounded_voltage = floor((voltage / stepV) + 0.5) * stepV;
	switch (_simulationMode()) {
	case NTBP_DETERMINISTIC: {
		lua_getglobal(L, "step_current");
		/* the first argument */
		lua_pushnumber(L, rounded_voltage);

		/* call the function with 1
		 argument, return 0 result */
		lua_call(L, 1, 0);

		return (modigliani_base::ReturnEnum::SUCCESS);
	}

		break;
	default:
		std::cerr
				<< "Lua_based_deterministic_multi_current::StepCurrent - ERROR : Unsupported simulation mode."
				<< std::endl;
		return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
		break;
	}
	return (modigliani_base::ReturnEnum::FAIL);
}

/**  */
/** No descriptions */
inline modigliani_base::Real Lua_based_deterministic_multi_current::open_channels() const {
	lua_getglobal(L, "open_channels");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	modigliani_base::Real count = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return (count * NumChannels());
}

/**  */
/** No descriptions */
modigliani_base::Real Lua_based_deterministic_multi_current::num_channels_in_state(
		modigliani_base::Size __attribute__((__unused__)) state) const {
	std::cerr << "Deterministic channel does not have states" << std::endl;
	return (0);
}

inline modigliani_base::Real Lua_based_deterministic_multi_current::compute_conductance() {
	lua_getglobal(L, "compute_conductance");

	/* call the function with 0
	 argument, return 1 result */
	lua_call(L, 0, 1);
	modigliani_base::Real conduc = lua_tonumber(L, -1);
	lua_pop(L, 1);
	M_ASSERT(conduc == conduc);
	return (Set_conductance(
			conduc * _maxConductivity() * _area() /* muMeter^2 */* 1.0e-8));
}

void Lua_based_deterministic_multi_current::show_param() const {
	cout << "Na channel parameters:" << std::endl;
	cout << "Single channel conductivity [nA]" << _conductivity() << std::endl;
	cout << "Channel density [1/muMeter^2]" << _area() << std::endl;
	cout << "MaxConductivity (all channels open) mSiemens/cm^2"
			<< _maxConductivity() << std::endl;
}

modigliani_base::Real Lua_based_deterministic_multi_current::lua_get_ntreal(lua_State* L,
		string name) {
	lua_getglobal(L, name.c_str());
	modigliani_base::Real ret = lua_tonumber(L, -1);
	lua_pop(L, 1);
	return (ret);
}

