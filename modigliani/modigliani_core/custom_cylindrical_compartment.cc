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
}

/* ***      DESTRUCTOR		***/
Custom_cylindrical_compartment::~Custom_cylindrical_compartment() {
}

/* ***  PUBLIC                                    ***   */
//modigliani_base::ReturnEnum AttachCurrent(Membrane_current * currentPtr,
//        NTBPcurrentType type = NTBP_IONIC);
//    modigliani_base::ReturnEnum Step(const modigliani_base::Real newVM /* mV */);
//    modigliani_base::ReturnEnum Step();
//    modigliani_base::ReturnEnum InjectCurrent(
//        modigliani_base::Real current /* in nA */);
//    modigliani_base::Real AttachedConductance(
//        modigliani_base::Size currentIndex) {
//      M_ASSERT((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
//      return (current_vec_[currentIndex - 1]->conductance());
//    }
modigliani_base::ReturnEnum Custom_cylindrical_compartment::AttachCurrentWithConcentrations(
    Membrane_current * currentPtr, Real concentration_inside,
    Real concentration_outside) {
  currentPtr->set_voltage(vm_);
  currentPtr->setTimeStep(timeStep());
  currentPtr->set_temperature(temperature_);

  current_vec_.push_back(currentPtr);

  return (modigliani_base::ReturnEnum::SUCCESS);
}
