/**
 * \file custom_cylindrical_compartment.cc
 *
 * \brief Custom_cylindrical_compartment class implementation
 *
 * \author Ahmed Aldo Faisal &copy; created 26.3.2001
 *
 * \version:  1
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Ali Neishabouri
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

#include "custom_cylindrical_compartment.h"

using namespace modigliani_core;
using modigliani_base::Real;

/* ***      CONSTRUCTORS	***/
/** Create a Custom_cylindrical_compartment */
Custom_cylindrical_compartment::Custom_cylindrical_compartment(
    const modigliani_base::Real newLength,
    const modigliani_base::Real newDiameter, const modigliani_base::Real newCm,
    const modigliani_base::Real newRa,
    const modigliani_base::Real newTemperature)
    : Cylindrical_compartment(newLength, newDiameter, newCm, newRa,
                              newTemperature) {
  current_vec_ = std::vector<custom_current>(0);
}

/* ***      DESTRUCTOR		***/
Custom_cylindrical_compartment::~Custom_cylindrical_compartment() {
}

/* ***  PUBLIC                                    ***   */
//    modigliani_base::ReturnEnum Step(const modigliani_base::Real newVM /* mV */);
//    modigliani_base::ReturnEnum Step();
modigliani_base::ReturnEnum Custom_cylindrical_compartment::AttachCurrentWithConcentrations(
    Membrane_current * currentPtr, Real concentration_inside,
    Real concentration_outside) {

  currentPtr->set_voltage(vm());
  currentPtr->setTimeStep(timeStep());
  currentPtr->set_temperature(temperature_);

  custom_current new_current;
  new_current.current_ptr = currentPtr;
  new_current.inside_concentration = concentration_inside;
  new_current.outside_concentration = concentration_outside;
  new_current.track = true;
  current_vec_.push_back(new_current);
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Custom_cylindrical_compartment::AttachCurrent(
    Membrane_current * currentPtr, NTBPcurrentType type) {
  currentPtr->set_voltage(vm());
  currentPtr->setTimeStep(timeStep());
  currentPtr->set_temperature(temperature_);

  custom_current new_current;
  new_current.current_ptr = currentPtr;
  new_current.inside_concentration = 0;
  new_current.outside_concentration = 0;
  new_current.track = false;
  current_vec_.push_back(new_current);
  return (modigliani_base::ReturnEnum::SUCCESS);
}

modigliani_base::ReturnEnum Custom_cylindrical_compartment::Step(
    const modigliani_base::Real newVM) {
  vm_ = newVM;
  //for every current
  for (modigliani_base::Size it = 0; it < current_vec_.size(); it++) {
    Membrane_current* current = current_vec_[it].current_ptr;
    current->Step(vm_);
    if ((current_vec_[it]).track) {
      // Count how many ions got in, in picomoles
      modigliani_base::Size ions_picomoles = current->current() * timeStep()
          / 96485.3415;
      current_vec_[it].inside_concentration -= ions_picomoles * 6 / volume_;
      // TODO(Ali): What to do with outside concentration?
      current->set_reversal_potential(
          26.64
              * log(
                  current_vec_[it].outside_concentration
                      / current_vec_[it].inside_concentration));
    }
    // TODO(Ali): pumps!
  }

  return (modigliani_base::ReturnEnum::SUCCESS);
}
