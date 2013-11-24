/**
 * @file membrane_current.h
 * @brief Membrane_current class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_CURRENT_H_

#include <assert.h>

#include "modigliani_base/types.h"
#include "modigliani_core/object.h"

namespace modigliani_core {

/**
 *  @brief Membrane_current class
 *
 *  Represents the current accross a membrane, with its
 *  generalisable properties. It computes the
 *  actual currents based on the Nernst-Equation accross
 *  the membrane neglecting diffusion/concentration effects
 *  and the conductivity values provided by the hh_current
 *  or channel_current classes.
 */
class Membrane_current : public Object {
  public:
  /**
   * @brief Constructs a current
   * @param reversalPotential Reversal potential in [@f$\si{\milli\volt}@f$]
   */
  explicit Membrane_current(modigliani_base::Real reversalPotential);
  Membrane_current(const Membrane_current & original) = delete;
  Membrane_current & operator=(const Membrane_current & right) = delete;
  virtual ~Membrane_current();

  /**
   * @brief Returns maximum conductivity
   * @return Maximum conductivity in
   * [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
   */
  virtual modigliani_base::Real MaxConductivity() const = 0;

  /**
   * @brief Returns momentary conductance
   * @return Conductance in  [@f$\si{\milli\siemens}@f$]
   */
  modigliani_base::Real conductance() const {
    return (conductance_);
  }

  /**
   * @brief Sets the reversal potential
   * @param eRev Reversal potential in  [@f$\si{\milli\volt}@f$]
   */
  void set_reversal_potential(modigliani_base::Real eRev /* mV */) {
    reversal_potential_ = eRev;
  }

  /**
   * @brief Returns the reversal potential
   * @return Reversal potential in  [@f$\si{\milli\volt}@f$]
   */
  modigliani_base::Real reversal_potential() const {
    return (reversal_potential_);
  }

  /**
   * @brief Returns the temperature
   * @return Temperature in  [@f$\si{\celsius}@f$]
   */    
  modigliani_base::Real temperature() const {
    return (temperature_);
  }

  /**
   * @brief Sets the temperature
   * @param newTemp Temperature in  [@f$\si{\celsius}@f$]
   */
  modigliani_base::ReturnEnum set_temperature(modigliani_base::Real newTemp) {
    assert(newTemp > modigliani_base::ZERO_KELVIN);
    temperature_ = newTemp;
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  /**
   * @brief Returns the Q10 factor
   * @return Q10
   */
  modigliani_base::Real q10() const {
    return (q10_);
  }

  /**
   * @brief Sets the Q10 factor
   * @param newQ10 Q10
   */
  virtual modigliani_base::ReturnEnum set_q10(modigliani_base::Real newQ10) {
    q10_ = newQ10;
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  /**
   * @brief Returns the simulation mode
   * @return Simulation mode
   */
  enum StochasticType simulation_mode() const {
    return (simulation_mode_);
  }

  /**
   * @brief Sets simulation mode
   * @param newMode Simulation mode
   */
  virtual void set_simulation_mode(enum StochasticType newMode) {
    simulation_mode_ = newMode;
  }

  /**
   * @brief Sets the membrane potential
   * @param newVoltage Membrane potential in  [@f$\si{\milli\volt}@f$]
   */
  void set_voltage(modigliani_base::Real newVoltage) {
    voltage_ = newVoltage;
  }

  /**
   * @brief Runs one step of the simulation, updates the conductance
   * and the current flowing through.
   * @param newVm Membrane potential [@f$\si{\milli\volt}@f$]
   * @return Success or Failure
   */
  modigliani_base::ReturnEnum Step(modigliani_base::Real newVm) {
    // ComputeRateConstants(newVm); /* UpdateRateConstantsAND*/
    voltage_ = newVm;
    modigliani_base::ReturnEnum result = StepCurrent();
    if (result != modigliani_base::ReturnEnum::SUCCESS) {
      std::cerr << "membrane_current::Step - ERROR : Current step failed at "
                << newVm << "mV. " << std::endl;
      return (modigliani_base::ReturnEnum::FAIL);
    }
    ComputeConductance();
    ComputeCurrent(newVm);
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  /**
   * @brief Computes, sets and returns the current going through
   * @param vM Membrane potential in [@f$\si{\milli\volt}@f$]
   */
  modigliani_base::Real ComputeCurrent(
      modigliani_base::Real vM) {
    return (set_current(conductance()* 1000.0 /* mA/nA */
                        * (vM - reversal_potential())));
  }

  /**
   * @brief Runs one step of the simulation
   * @return Success or failure
   */
  virtual modigliani_base::ReturnEnum StepCurrent() = 0;

  /**
   * @brief Computes and returns conductance
   * @return Conductance in [@f$\si{\milli\siemens}@f$]
   */
  virtual modigliani_base::Real ComputeConductance() = 0;

  /**
   * @brief Computes and returns the rate constants
   *
   * If not overriden, returns 0.
   * @return Time Constant in [@f$\si{\per\milli\second}@f$}]
   */
  virtual modigliani_base::Real ComputeTimeConstant() {
    return (0.0);
  }

  /**
   * @brief Returns the current
   * @return Current in [@f$\si{\nano\ampere}@f$}]
   */
  modigliani_base::Real current() const {
    return (current_);
  }
  
  protected:
  /**
   * @brief Sets the current
   * @param newVal Current in [@f$\si{\nano\ampere}@f$}]
   */
  modigliani_base::Real set_current(
      modigliani_base::Real newVal) {
    return (current_ = newVal);
  }
  
  /**
   * @brief Sets the conductance
   * @param newVal Current in [@f$\si{\milli\siemens}@f$]
   */
  modigliani_base::Real set_conductance(
      modigliani_base::Real newVal) {
    return (conductance_ = newVal);
  }

  /**
   * @brief Membrane potential in [@f$\si{\milli\volt}@f$]
   */
  modigliani_base::Real voltage_;

  /**
   * @brief Temperature in [@f$\si{\celsius}@f$]
   */
  modigliani_base::Real temperature_;

  private:
  void _init(modigliani_base::Real newReversalPotential);
  modigliani_base::Real current_;  // in nanoAmpere
  modigliani_base::Real conductance_;  // in mSiemens
  modigliani_base::Real reversal_potential_;  // in mV
  // the Q_10 value for temperature dependent reaction kinetics
  modigliani_base::Real q10_;
  enum StochasticType simulation_mode_;
};
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_CURRENT_H_
