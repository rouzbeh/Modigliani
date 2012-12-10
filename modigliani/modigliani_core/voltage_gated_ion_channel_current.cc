/**
 * @file voltage_gated_ion_channel_current
 * Voltage_gated_ion_channel_current class implementation
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001
 * @author Ali Neishabouri &copy; created 16.3.2012
 * @version   2
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

#include "voltage_gated_ion_channel_current.h"

using namespace modigliani_core;

/* ***      CONSTRUCTORS	***/
/** Create a Voltage_gated_ion_channel_current */
Voltage_gated_ion_channel_current::Voltage_gated_ion_channel_current(
    modigliani_base::Real newReversalPotential,  //in mV
    modigliani_base::Real newDensity,  // channels per mumeter^2
    modigliani_base::Real newArea,  // in mumeter^2
    modigliani_base::Real newConductivity  // in mSiemens per channel
    )
    : Membrane_current(newReversalPotential), ratesComputed(false) {
  density_ = newDensity;
  area_ = newArea;
  conductivity_ = newConductivity;
  num_channels_ = area_ * density_;
}

/* ***      COPY AND ASSIGNMENT	***/
Voltage_gated_ion_channel_current::Voltage_gated_ion_channel_current(
    const Voltage_gated_ion_channel_current & original)
    : Membrane_current(original.reversal_potential()), ratesComputed(false) {
  density_ = original.density();
  area_ = original.area();
  conductivity_ = original.conductivity();
  num_channels_ = area_ * density_;
}

Voltage_gated_ion_channel_current&
Voltage_gated_ion_channel_current::operator=(
    const Voltage_gated_ion_channel_current & right) {
  if (this == &right) return (*this);  // Gracefully handle self assignment
  // add assignment code here
  return (*this);
}

/* ***      DESTRUCTOR		***/
Voltage_gated_ion_channel_current::~Voltage_gated_ion_channel_current() {
}
