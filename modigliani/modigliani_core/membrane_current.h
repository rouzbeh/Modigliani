/**
 * @file membrane_current.h
 * Mmbrane_current_o class header
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _modigliani_core_membrane_current_h_
#define _modigliani_core_membrane_current_h_

#include "modigliani_base/main.h"
#include "modigliani_base/types.h"
#include "object.h"
#include "modigliani_base/uniform_rnd_dist.h"

namespace modigliani_core {

/**
 *  @short Membrane_current class
 *
 *  Represents the current accross a membrane, with its
 *  generalisable properties. It computes the
 *  actual currents based on the Nernst-Equation accross
 *  the membrane neglecting diffusion/concentration effects
 *  and the conductivity values provided by the hh_current
 *  or channel_current classes.
 *  @bug unknown
 *  @warning unknown
 */

class Membrane_current : public Object {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Membrane_current(modigliani_base::Real reversalPotential /* in mV */);
    Membrane_current(const Membrane_current & original);
    Membrane_current & operator=(const Membrane_current & right);
    virtual ~Membrane_current();

    /* ***  Methods              ***/
    /* in mSiemens/cm^2 */
    virtual modigliani_base::Real _maxConductivity() const = 0;
    /** momentary conductance in muFarad */
    modigliani_base::Real _conductance() const {
      return (conductance);
    }
    /** reversal potential in mV */
    void Set_reversalPotential(modigliani_base::Real eRev /* mV */) {
      reversalPotential = eRev;
    }
    /** reversal potential in mV */
    modigliani_base::Real _reversalPotential() const {
      return (reversalPotential);
    }
    /** temperature in Celsius */
    modigliani_base::Real _temperature() const {
      return (temperature);
    }
    /** Set temperature in Celsius */
    modigliani_base::ReturnEnum Set_temperature(modigliani_base::Real newTemp) {
      M_ASSERT(newTemp > modigliani_base::ZERO_KELVIN);
      temperature = newTemp;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /** Q10  */
    modigliani_base::Real _q10() const {
      return (q10);
    }
    /** Set Q10 */
    virtual modigliani_base::ReturnEnum Set_q10(modigliani_base::Real newQ10) {
      q10 = newQ10;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /** Simulation mode */
    enum NTBPstochasticType _simulationMode() const {
      return (simulationMode);
    }
    /** Set simulation mode */
    virtual void SetSimulationMode(enum NTBPstochasticType newMode) {
      simulationMode = newMode;
    }

    modigliani_base::ReturnEnum step(modigliani_base::Real newVm /* in mV */) {
      //ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
      voltage = newVm;
      step_current();
      compute_conductance();
      compute_current(newVm);
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /* in nA */
    modigliani_base::Real compute_current(
        modigliani_base::Real vM /* in mV */) {
      return (Set_current(_conductance() /* mSiemens */* 1000.0 /* mA/nA */* (vM
      /* mV */- _reversalPotential()/* mV */)));
    }
    virtual modigliani_base::ReturnEnum step_current() = 0;
    /** compute and return conductance in mSiemens */
    virtual modigliani_base::Real compute_conductance() = 0;
    /** compute the rate constants ( in ms^-1 ) */
    //virtual void ComputeRateConstants(modigliani_base::Real vM /* in mV */) = 0;
    /** Number of open ionic channels */
    virtual modigliani_base::Real open_channels() const = 0;
    /** Total number of ionic channels */
    virtual modigliani_base::Real NumChannels() const = 0;

    virtual modigliani_base::Real num_channels_in_state(
        modigliani_base::Size __attribute__((__unused__)) state) const {
      std::cerr
          << "NTBP_membrane_current_o::NumChannels() - Error : Not Implemented."
          << std::endl;
      return (-42);
    }
    /** Number of open over total number of channels */
    virtual modigliani_base::Real OpenChannelsRatio() const {
      std::cerr
          << "NTBP_membrane_current_o::OpenChannelsRatio() - Error : Not Implemented."
          << std::endl;
      return (-42);
    }
    virtual void show_param() const {
      std::cerr
          << "NTBP_membrane_current_o::ShowParam - Warning : Not Implemented."
          << std::endl;
    }
    /* Additional sweeps by the Gillespie Algorithm make the following method necessary */
    virtual modigliani_base::Real ComputeChannelStateTimeConstant() const {
      std::cerr
          << "NTBP_membrane_current_o::ComputeChannelStateTimeConstant - Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
          << std::endl;
      return (0);
    }
    modigliani_base::Real ChannelStateTimeConstant() const {
      return (ComputeChannelStateTimeConstant());
    }
    bool GillespieStep() {/*2DO is this necessary here*/
      step_current();
      return (ComputeGillespieStep());
    }
    virtual bool ComputeGillespieStep() {
      std::cerr
          << "NTBP_membrane_current_o::ComputeGillespieStep- Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
          << std::endl;
      return (modigliani_base::ReturnEnum::NOT_DERIVED);
    }

    modigliani_base::Real Get_voltage() {
      return (voltage);
    }
    void set_voltage(modigliani_base::Real newVoltage) {
      voltage = newVoltage;
    }

    modigliani_base::Real current() const {
      return (current_);
    }

    /* ***  Data                 ***/

  protected:
    /* ***  Methods              ***/
    modigliani_base::Real Set_current(
        modigliani_base::Real newVal /* in nA */) {
      return (current_ = newVal);
    }
    modigliani_base::Real Set_conductance(
        modigliani_base::Real newVal /* in mSiemens */) {
      return (conductance = newVal);
    }
    /* ***  Data                 ***/
    modigliani_base::Uniform_rnd_dist uniform;
    modigliani_base::Real voltage;
    modigliani_base::Real temperature;  // in Celsius
  private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    modigliani_base::Real current_;  // in nanoAmpere
    modigliani_base::Real conductance;  // in mSiemens
    modigliani_base::Real reversalPotential;  // in mV
    modigliani_base::Real q10;  // the Q_10 value for temperature dependent reaction kinetics
    enum NTBPstochasticType simulationMode;
};
}
#endif /* _modigliani_core_membrane_current.h_ */

/* File skeleton generated by GenNTObj version 0.7. */
