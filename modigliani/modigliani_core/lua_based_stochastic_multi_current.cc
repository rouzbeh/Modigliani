/**
 * @file lua_based_stochastic_multi_current.cc
 * @author Ali Neishabouri
 */

#include "lua_based_stochastic_multi_current.h"

using namespace modigliani_core;

bool Lua_based_stochastic_multi_current::initTableLookUp = false;
map<string, Transition_rate_matrix*> Lua_based_stochastic_multi_current::probability_matrix_map;
map<string, modigliani_base::Size> Lua_based_stochastic_multi_current::number_of_states_map;
map<string, double> Lua_based_stochastic_multi_current::base_temperature_map;
map<string, std::vector<modigliani_base::Size> > Lua_based_stochastic_multi_current::open_states_map;

/* ***      CONSTRUCTORS	***/
Lua_based_stochastic_multi_current::Lua_based_stochastic_multi_current(
    modigliani_base::Real newArea, modigliani_base::Real newDensity,
    modigliani_base::Real newConductivity,
    modigliani_base::Real newReversalPotential,
    modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
    string fileName)
    : Multi_current(newReversalPotential /* in mV */,
                    newDensity /* channels per mu^2 */, newArea /* in mu^2 */,
                    newConductivity /* in mS per channel  */
                    ) {

  UpdateNumChannels();  // TODO(Ali)

  setTimeStep(newTimeStep);
  set_temperature(newTemperature);
  if (number_of_states_map[fileName] == 0) {
    load_file(fileName, newTemperature, newTimeStep);
  }
  baseTemp = base_temperature_map[fileName];

  M_ASSERT(number_of_states_map[fileName]>0);

  if (false == initTableLookUp) {
    initTableLookUp = true;
  }
  channels_ptr_ = new Ion_channels(num_channels(), number_of_states_map[fileName],
                                 probability_matrix_map[fileName], newTimeStep);
  for (unsigned int i = 0; i < open_states_map[fileName].size(); i++) {
    channels_ptr_->SetAsOpenState(open_states_map[fileName][i]);
  }
}

/* ***      DESTRUCTOR		***/
Lua_based_stochastic_multi_current::~Lua_based_stochastic_multi_current() {
  delete channels_ptr_;
  channels_ptr_ = 0;
}

/* ***  PUBLIC                                    ***   */
void Lua_based_stochastic_multi_current::load_file(string fileName,
                                                   double temperature,
                                                   double time_step) {
  cout << "Getting probabilities from " << fileName << std::endl;
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  luaL_dofile(L, fileName.c_str());

  base_temperature_map[fileName] = lua_get_real(L, "base_temp");

  number_of_states_map[fileName] = lua_get_real(L, "number_states");

  open_states_map[fileName] = std::vector<modigliani_base::Size>();

  lua_getglobal(L, "open_states");
  /* table is in the stack at index 't' */
  lua_pushnil(L); /* first key */
  while (lua_next(L, -2) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    int found = lua_tonumber(L, -1);
    open_states_map[fileName].push_back(found);
    lua_pop(L, 1);
  }

  double minV = lua_get_real(L, "minV");
  double maxV = lua_get_real(L, "maxV");
  double step = lua_get_real(L, "step");

  //const Json::Value transitions = root["transitions"];

  probability_matrix_map[fileName] = new Transition_rate_matrix(
      number_of_states_map[fileName], minV, maxV, step);

  modigliani_base::Real length = floor((maxV - minV) / step + 0.5) + 1;
  for (modigliani_base::Size i = 1; i <= number_of_states_map[fileName]; ++i) {
    for (modigliani_base::Size j = 1; j <= number_of_states_map[fileName];
        ++j) {
      for (modigliani_base::Size k = 0; k < length; k++) {
        modigliani_base::Real voltage = minV + step * k;
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
        double probability = TemperatureRateRelation(
            temperature, base_temperature_map[fileName] /* C */, prob_q10)
            * base_probability * time_step;
        probability_matrix_map[fileName]->setTransitionProbability(voltage, i,
                                                                   j,
                                                                   probability);
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
inline modigliani_base::ReturnEnum Lua_based_stochastic_multi_current::StepCurrent() {
  switch (simulation_mode()) {
    case BINOMIALPOPULATION: {
      return (channels_ptr_->BinomialStep(voltage_));
    }
      break;
    case SINGLECHANNEL: {
      return (channels_ptr_->Step(voltage_));
    }
      break;
    case GILLESPIE: {
      return (channels_ptr_->GillespieStep(voltage_));
    }
      break;
    case DETERMINISTIC: {
      return (channels_ptr_->DeterministicStep(voltage_));
    }

      break;
    default:
      std::cerr
          << "Lua_based_stochastic_multi_current::StepCurrent - ERROR : Unsupported simulation mode."
          << std::endl;
      return (modigliani_base::ReturnEnum::PARAM_UNSUPPORTED);
      break;
  }
  return (modigliani_base::ReturnEnum::FAIL);
}

/**  */
/** No descriptions */
inline modigliani_base::Real Lua_based_stochastic_multi_current::open_channels() const {
  return (channels_ptr_->NumOpen());
}

inline modigliani_base::Real Lua_based_stochastic_multi_current::ComputeConductance() {
  return (set_conductance(channels_ptr_->NumOpen() * conductivity_));
}

inline modigliani_base::Real Lua_based_stochastic_multi_current::ComputeChannelStateTimeConstant() const {
  return (channels_ptr_->ComputeChannelStateTimeConstant(voltage_));
}

void Lua_based_stochastic_multi_current::show_param() const {
  cout << "Na channel parameters:" << std::endl;
  cout << "Single channel conductivity [nA]" << conductivity() << std::endl;
  cout << "Channel density [1/muMeter^2]" << area() << std::endl;
  cout << "MaxConductivity (all channels open) mSiemens/cm^2"
       << _maxConductivity() << std::endl;
}

modigliani_base::Real Lua_based_stochastic_multi_current::lua_get_real(
    lua_State* L, string name) {
  lua_getglobal(L, name.c_str());
  modigliani_base::Real ret = lua_tonumber(L, -1);
  lua_pop(L, 1);
  return (ret);
}
