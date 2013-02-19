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
    Membrane_current(const Membrane_current & original) = delete;
    Membrane_current & operator=(const Membrane_current & right) = delete;
    virtual ~Membrane_current();

    /* ***  Methods              ***/
    /// in mSiemens/cm^2
    virtual modigliani_base::Real max_conductivity() const = 0;

    /** momentary conductance in muFarad */
    modigliani_base::Real conductance() const {
      return (conductance_);
    }
    /** reversal potential in mV */
    void set_reversal_potential(modigliani_base::Real eRev /* mV */) {
      reversal_potential_ = eRev;
    }
    /** reversal potential in mV */
    modigliani_base::Real reversal_potential() const {
      return (reversal_potential_);
    }
    /** temperature in Celsius */
    modigliani_base::Real temperature() const {
      return (temperature_);
    }
    /** Set temperature in Celsius */
    modigliani_base::ReturnEnum set_temperature(modigliani_base::Real newTemp) {
      M_ASSERT(newTemp > modigliani_base::ZERO_KELVIN);
      temperature_ = newTemp;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /** Q10  */
    modigliani_base::Real q10() const {
      return (q10_);
    }
    bool GillespieStep() {/*2DO is this necessary here*/
      modigliani_base::ReturnEnum result = StepCurrent();
      if (result != modigliani_base::ReturnEnum::SUCCESS) {
        std::cerr
            << "membrane_current::GillespieStep - ERROR : Current step failed."
            << std::endl;
        return (modigliani_base::ReturnEnum::FAIL);
      }
      return (ComputeGillespieStep());
    }
    virtual bool ComputeGillespieStep() {
      std::cerr
          << "NTBP_membrane_current_o::ComputeGillespieStep- Error : Method should be overridden by a stochastic current class or not be called for a deterministic current class."
          << std::endl;
      return (modigliani_base::ReturnEnum::NOT_DERIVED);
    }

    /** Set Q10 */
    virtual modigliani_base::ReturnEnum set_q10(modigliani_base::Real newQ10) {
      q10_ = newQ10;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /** Simulation mode */
    enum StochasticType simulation_mode() const {
      return (simulation_mode_);
    }
    /** Set simulation mode */
    virtual void set_simulation_mode(enum StochasticType newMode) {
      simulation_mode_ = newMode;
    }
    void set_voltage(modigliani_base::Real newVoltage) {
      voltage_ = newVoltage;
    }

    modigliani_base::ReturnEnum Step(modigliani_base::Real newVm /* in mV */) {
      //ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
      voltage_ = newVm;

      modigliani_base::ReturnEnum result = StepCurrent();
      if (result != modigliani_base::ReturnEnum::SUCCESS) {
        std::cerr << "membrane_current::Step - ERROR : Current step failed at "
                  << newVm << "mV. " << std::endl;
        return (modigliani_base::ReturnEnum::FAIL);
      }
      ComputeConductance();
      compute_current(newVm);
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /* in nA */
    modigliani_base::Real compute_current(
        modigliani_base::Real vM /* in mV */) {
      return (set_current(conductance() /* mSiemens */* 1000.0 /* mA/nA */* (vM
      /* mV */- reversal_potential()/* mV */)));
    }
    virtual modigliani_base::ReturnEnum StepCurrent() = 0;
    /** compute and return conductance in mSiemens */
    virtual modigliani_base::Real ComputeConductance() = 0;
    /** compute the rate constants ( in ms^-1 ) */

    virtual modigliani_base::Real ComputeTimeConstant() {
      return (0.0);
    }

    virtual void show_param() const {
      std::cerr
          << "NTBP_membrane_current_o::ShowParam - Warning : Not Implemented."
          << std::endl;
    }

    modigliani_base::Real current() const {
      return (current_);
    }

    /* ***  Data                 ***/

  protected:
    /* ***  Methods              ***/
    modigliani_base::Real set_current(
        modigliani_base::Real newVal /* in nA */) {
      return (current_ = newVal);
    }
    modigliani_base::Real set_conductance(
        modigliani_base::Real newVal /* in mSiemens */) {
      return (conductance_ = newVal);
    }
    /* ***  Data                 ***/
    modigliani_base::Real voltage_;
    modigliani_base::Real temperature_;  // in Celsius
  private:
    /* ***  Methods              ***/
    void _init(modigliani_base::Real newReversalPotential);
    /* ***  Data                 ***/
    modigliani_base::Real current_;  // in nanoAmpere
    modigliani_base::Real conductance_;  // in mSiemens
    modigliani_base::Real reversal_potential_;  // in mV
    modigliani_base::Real q10_;  // the Q_10 value for temperature dependent reaction kinetics
    enum StochasticType simulation_mode_;
};
}
#endif /* _modigliani_core_membrane_current.h_ */
