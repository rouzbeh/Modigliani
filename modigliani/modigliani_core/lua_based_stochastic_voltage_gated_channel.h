/**
 * @file lua_based_stochastic_voltage_gated_channel.h
 * @author Ali Neishabouri
 */

#ifndef MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "modigliani_core/voltage_gated_ion_channel_current.h"
#include "modigliani_core/ion_channels.h"
#include "modigliani_core/aux_func.h"

#include <boost/filesystem.hpp>

#include <map>
using namespace std;

namespace modigliani_core {
///@brief Stochastic ion channel with transition probabilities read from a lua script
///
/// Transition probabilities are read for a range of membrane potentials by calling a lua
/// script. The actual simulations are carried inside the program.
class Lua_based_stochastic_voltage_gated_channel : public Voltage_gated_ion_channel_current {
  public:
    ///@brief Loads transition probabilities by running a lua script.
    ///@param newArea Membrane area
    ///@param newDensity Channel density
    ///@param newConductivity Max conductivity
    ///@param reversalPotential Current reversal potential
    ///@param newTimeStep Simulation time step
    ///@param newTemperature Simulation temperature
    ///@param fileName Lua script
    ///
    /// The lua script must define the following variables :
    /// - float base_temp
    /// - uint number_states
    /// - array(uint) open_states
    /// - float minV
    /// - float maxV
    /// - float step
    /// .
    /// The lua script must define the following functions :
    /// - get_probability(uint, uint, float)
    /// - get_q10(uint, uint)
    Lua_based_stochastic_voltage_gated_channel(
        modigliani_base::Real newArea, modigliani_base::Real newDensity,
        modigliani_base::Real newConductivity,
        modigliani_base::Real reversalPotential,
        modigliani_base::Real newTimeStep, modigliani_base::Real newTemperature,
        string fileName);

    Lua_based_stochastic_voltage_gated_channel(
        const Lua_based_stochastic_voltage_gated_channel & original) = delete;
    const Lua_based_stochastic_voltage_gated_channel&
    operator=(const Lua_based_stochastic_voltage_gated_channel & right) = delete;
    virtual ~Lua_based_stochastic_voltage_gated_channel();

    void reset() override {
      channels_ptr_->reset();
    }

    virtual modigliani_base::Real ComputeTimeConstant() const;
    virtual modigliani_base::Real ComputeConductance();

    ///@brief Calculates all transitions in ion channels.
    ///@return Status
    ///@warning Does not update conductance
    virtual modigliani_base::ReturnEnum StepCurrent();
    virtual modigliani_base::Real OpenChannels() const;
    void show_param() const;
    void printProb(string fileName) {
      for (modigliani_base::Real v = 20; v < 130; v += 0.005) {
        cout
            << probability_matrix_map[fileName]->GetTransitionProbability(v, 2,
                                                                          1)
            << std::endl;
      }
    }

  private:

    static void load_file(string fileName, double temperature,
                          double time_step);
    static map<string, Transition_rate_matrix*> probability_matrix_map;
    static map<string, modigliani_base::Size> number_of_states_map;
    static map<string, double> base_temperature_map;
    static map<string, std::vector<modigliani_base::Size> > open_states_map;
    Ion_channels* channels_ptr_;
    static modigliani_base::Real lua_get_real(lua_State* L, string name);
    static bool initTableLookUp;
    static std::vector<string> initialised_probability_matrices;
    modigliani_base::Real baseTemp;

};
}
#endif  // MODIGLIANI_MODIGLIANI_CORE_LUA_BASED_STOCHASTIC_MULTI_CURRENT_H_
