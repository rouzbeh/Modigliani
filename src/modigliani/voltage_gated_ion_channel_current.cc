/**
 * @file voltage_gated_ion_channel_current.cc
 * @brief Voltage_gated_ion_channel_current class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani/voltage_gated_ion_channel_current.h"

namespace modigliani {
Voltage_gated_ion_channel_current::Voltage_gated_ion_channel_current(
    modigliani::Real newReversalPotential,
    modigliani::Real newDensity,
    modigliani::Real newArea,
    modigliani::Real newConductivity)
    : Membrane_current(newReversalPotential), ratesComputed(false) {
  density_ = newDensity;
  area_ = newArea;
  conductivity_ = newConductivity;
  num_channels_ = area_ * density_;
}

Voltage_gated_ion_channel_current::~Voltage_gated_ion_channel_current() {
}
}  // namespace modigliani
