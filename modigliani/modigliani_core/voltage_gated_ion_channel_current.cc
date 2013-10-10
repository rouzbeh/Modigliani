/**
 * \file voltage_gated_ion_channel_current.cc
 * \brief Voltage_gated_ion_channel_current class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 *
 * \section LICENSE
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

#include "modigliani_core/voltage_gated_ion_channel_current.h"

namespace modigliani_core {
Voltage_gated_ion_channel_current::Voltage_gated_ion_channel_current(
    modigliani_base::Real newReversalPotential,
    modigliani_base::Real newDensity,
    modigliani_base::Real newArea,
    modigliani_base::Real newConductivity)
    : Membrane_current(newReversalPotential), ratesComputed(false) {
  density_ = newDensity;
  area_ = newArea;
  conductivity_ = newConductivity;
  num_channels_ = area_ * density_;
}

Voltage_gated_ion_channel_current::~Voltage_gated_ion_channel_current() {
}
}  // namespace modigliani_core
