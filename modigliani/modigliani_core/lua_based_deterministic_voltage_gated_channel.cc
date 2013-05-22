/**
 * @file lua_based_deterministic_voltage_gated_channel.cc
 * Contains the Lua_based_deterministic_voltage_gated_channel class implementation.
 * @author Ali Neishabouri
 */

#include "lua_based_deterministic_voltage_gated_channel.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
Lua_based_deterministic_voltage_gated_channel::Lua_based_deterministic_voltage_gated_channel(
    modigliani_base::Real newArea, modigliani_base::Real newDensity,
    modigliani_base::Real newConductivity,
    modigliani_base::Real newReversalPotential,
    modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
    string new_lua_script)
    : Voltage_gated_ion_channel_current(newReversalPotential /* in mV */,
                                        newDensity /* channels per mu^2 */,
                                        newArea /* in mu^2 */, newConductivity /* in mS per channel  */
                                        ) {

  UpdateNumChannels();  //TODO
  lua_script = new_lua_script;
  setTimeStep(newTimeStep);
  set_temperature(newTemperature);
  L = luaL_newstate();
  luaL_openlibs(L);

  boost::filesystem::path lua_path(lua_script);
  auto lua_path_parent = lua_path.parent_path();
  auto lua_path_common = lua_path.parent_path();
  lua_path_parent /= "?.lua";
  lua_path_common /= "common";
  lua_path_common /= "?.lua";
  SetLuaPath(L, lua_path_common.string());
  SetLuaPath(L, lua_path_parent.string());
  int status = luaL_dofile(L, lua_script.c_str());
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(
        stderr,
        "Lua_based_deterministic_voltage_gated_channel says : Couldn't load file %s: %s\n",
        lua_script.c_str(), lua_tostring(L, -1));
    exit(1);
  }
  stepV = _lua_get_real(L, "step");
  lua_getglobal(L, "set_timestep");
  /* the first argument */
  lua_pushnumber(L, newTimeStep);
  /* call the function with 1
   argument, return 0 result */
  lua_call(L, 1, 0);
}

/* ***      COPY AND ASSIGNMENT	***/
Lua_based_deterministic_voltage_gated_channel::Lua_based_deterministic_voltage_gated_channel(
    const Lua_based_deterministic_voltage_gated_channel & original)
    : Voltage_gated_ion_channel_current(original.reversal_potential(),
                                        original.density(), original.area(),
                                        original.conductivity()) {
  UpdateNumChannels();
  stepV = _lua_get_real(L, "step");
  setTimeStep(original.timeStep());
  set_temperature(original.temperature());
  lua_script = original.lua_script;
  L = luaL_newstate();
  luaL_openlibs(L);
  boost::filesystem::path lua_path(original.lua_script);
  auto lua_path_parent = lua_path.parent_path();
  auto lua_path_common = lua_path.parent_path();
  lua_path_parent /= "?.lua";
  lua_path_common /= "common";
  lua_path_common /= "?.lua";
  SetLuaPath(L, lua_path_common.string());
  SetLuaPath(L, lua_path_parent.string());
  int status = luaL_dofile(L, lua_script.c_str());
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(
        stderr,
        "Lua_based_deterministic_voltage_gated_channel says : Couldn't load file %s: %s\n",
        lua_script.c_str(), lua_tostring(L, -1));
    exit(1);
  }
  lua_getglobal(L, "set_timestep");
  /* the first argument */
  lua_pushnumber(L, timeStep());

  /* call the function with 1
   argument, return 0 result */
  lua_call(L, 1, 0);
}

Lua_based_deterministic_voltage_gated_channel&
Lua_based_deterministic_voltage_gated_channel::operator=(
    const Lua_based_deterministic_voltage_gated_channel & right) {
  if (this == &right) return (*this);  // Gracefully handle self assignment
  setTimeStep(right.timeStep());
  set_temperature(temperature());
  lua_script = right.lua_script;
  L = luaL_newstate();
  luaL_openlibs(L);
  boost::filesystem::path lua_path(lua_script);
  auto lua_path_parent = lua_path.parent_path();
  auto lua_path_common = lua_path.parent_path();
  lua_path_parent /= "?.lua";
  lua_path_common /= "common";
  lua_path_common /= "?.lua";
  SetLuaPath(L, lua_path_common.string());
  SetLuaPath(L, lua_path_parent.string());
  int status = luaL_dofile(L, lua_script.c_str());
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(
        stderr,
        "Lua_based_deterministic_voltage_gated_channel says : Couldn't load file %s: %s\n",
        lua_script.c_str(), lua_tostring(L, -1));
    exit(1);
  }
  lua_getglobal(L, "set_timestep");
  /* the first argument */
  lua_pushnumber(L, timeStep());

  /* call the function with 1
   argument, return 0 result */
  lua_call(L, 1, 0);
  return (*this);
}

/* ***      DESTRUCTOR		***/
Lua_based_deterministic_voltage_gated_channel::~Lua_based_deterministic_voltage_gated_channel() {
  lua_close(L);
}

/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline modigliani_base::ReturnEnum Lua_based_deterministic_voltage_gated_channel::StepCurrent() {
  switch (simulation_mode()) {
    case DETERMINISTIC: {
      lua_getglobal(L, "step_current");
      // the first argument
      lua_pushnumber(L, voltage_);

      // call the function with 1 argument, return 0 result
      lua_call(L, 1, 0);

      return (modigliani_base::ReturnEnum::SUCCESS);
    }

      break;
    default:
      std::cerr
          << "Lua_based_deterministic_voltage_gated_channel::StepCurrent - ERROR : Unsupported simulation mode."
          << std::endl;
      return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
      break;
  }
  return (modigliani_base::ReturnEnum::FAIL);
}

/**  */
/** No descriptions */
inline modigliani_base::Real Lua_based_deterministic_voltage_gated_channel::open_channels() const {
  lua_getglobal(L, "open_channels");

  // call the function with 0 argument, return 1 result
  lua_call(L, 0, 1);
  modigliani_base::Real count = lua_tonumber(L, -1);
  lua_pop(L, 1);

  return (count * num_channels());
}

/**  */
/** No descriptions */
modigliani_base::Real Lua_based_deterministic_voltage_gated_channel::num_channels_in_state(
    modigliani_base::Size __attribute__((__unused__)) state) const {
  std::cerr << "Deterministic channel does not have states" << std::endl;
  return (0);
}

inline modigliani_base::Real Lua_based_deterministic_voltage_gated_channel::ComputeConductance() {
  lua_getglobal(L, "compute_conductance");

  /* call the function with 0
   argument, return 1 result */
  lua_call(L, 0, 1);
  modigliani_base::Real conduc = lua_tonumber(L, -1);
  lua_pop(L, 1);
  M_ASSERT(conduc == conduc);
  return (set_conductance(
      conduc * max_conductivity() * area() /* muMeter^2 */* 1.0e-8));
}

void Lua_based_deterministic_voltage_gated_channel::show_param() const {
  cout << "Na channel parameters:" << std::endl;
  cout << "Single channel conductivity [nA]" << conductivity() << std::endl;
  cout << "Channel density [1/muMeter^2]" << area() << std::endl;
  cout << "MaxConductivity (all channels open) mSiemens/cm^2"
       << max_conductivity() << std::endl;
}

modigliani_base::Real Lua_based_deterministic_voltage_gated_channel::_lua_get_real(
    lua_State* L, string name) {
  lua_getglobal(L, name.c_str());
  modigliani_base::Real ret = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return (ret);
}

