/**
 * @file lua_based_stochastic_multi_current.h
 * @author Ali Neishabouri
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "modigliani_core/multi_current.h"
#include "modigliani_core/ion_channels.h"

#include <map>
using namespace std;

namespace modigliani_core {
class Lua_based_stochastic_multi_current : public Multi_current {
  public:
    Lua_based_stochastic_multi_current(modigliani_base::Real newArea,
                                       modigliani_base::Real newDensity,
                                       modigliani_base::Real newConductivity,
                                       modigliani_base::Real reversalPotential,
                                       modigliani_base::Real newTimeStep,
                                       modigliani_base::Real newTemperature,
                                       string fileName);

    Lua_based_stochastic_multi_current(
        const Lua_based_stochastic_multi_current & original) = delete;
    const Lua_based_stochastic_multi_current&
    operator=(
        const Lua_based_stochastic_multi_current & right) = delete;
    virtual ~Lua_based_stochastic_multi_current();

    static void load_file(string fileName, double temperature,
        double time_step);
    static map<string, Transition_rate_matrix*> probability_matrix_map;
    static map<string, modigliani_base::Size> number_of_states_map;
    static map<string, double> base_temperature_map;
    static map<string, std::vector<modigliani_base::Size> > open_states_map;
    modigliani_base::Real ComputeChannelStateTimeConstant() const;
    virtual modigliani_base::Real ComputeConductance();

    virtual modigliani_base::ReturnEnum StepCurrent();
    virtual modigliani_base::Real open_channels() const;
    void show_param() const;
    void printProb(string fileName) {
      for (modigliani_base::Real v = 20; v < 130; v += 0.005) {
        cout
        << probability_matrix_map[fileName]->getTransitionProbability(
            v, 2, 1) << std::endl;
      }
    }

    private:
    static modigliani_base::Real lua_get_real(lua_State* L, string name);
    static bool initTableLookUp;
    static std::vector<string> initialised_probability_matrices;
    modigliani_base::Real baseTemp;

  };}
#endif  // MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_
