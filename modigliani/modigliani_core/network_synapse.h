///
/// \file network_synapse.h
/// \author Ali Neishabouri &copy; created 24/01/2013
/// \version   0.1
/// Copyright (C) 2013 Ali Neishabouri

#ifndef NETWORK_SYNAPSE_H_
#define NETWORK_SYNAPSE_H_

#include "membrane_current.h"
#include "membrane_compartment.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace modigliani_core {

///
/// \brief This class acts as a simplified synapse for use in neuronal network simulations.
///
/// The synapse has access to the membrane voltage of the source
/// neuron at each step. The voltage is given to a lua script,
/// and the conductance is read from the same script.
class Network_synapse : public Membrane_current {

  public:
    ///
    /// \brief The constructor initialises the lua interpreter.
    ///
    /// The lua script needs to supply the following methods :
    /// - void set_timestep(float)
    /// - void set_parameter(float)
    /// - void step_current(float)
    /// - float comput_conductacne(void)
    Network_synapse(const modigliani_base::Real new_reversal_potential,
                    Membrane_compartment * const src,
                    const modigliani_base::Real newTimeStep,
                    const std::string lua_file,
                    const modigliani_base::Real new_strength);
    virtual ~Network_synapse();

    Network_synapse(const Network_synapse & original) = delete;
    Network_synapse & operator=(const Network_synapse & right) = delete;

    modigliani_base::ReturnEnum StepCurrent();
    /** compute and return conductance in mSiemens */
    modigliani_base::Real ComputeConductance();
    modigliani_base::Real MaxConductivity() const;
    virtual void reset() override {
      exit(2);
    }
  protected:
  private:
    Membrane_compartment* source;
    modigliani_base::Real v_input;
    std::string _lua_script;
    lua_State* _L;
    //modigliani_base::Real avgSynBkCond; /* ns */
};

} /* namespace modigliani_core */
#endif /* NETWORK_SYNAPSE_H_ */
