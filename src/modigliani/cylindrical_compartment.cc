/**
 * @file cylindrical_compartment.cc
 *
 * @brief Cylindrical_compartment class implementation
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

#include "modigliani/cylindrical_compartment.h"

namespace modigliani {
Cylindrical_compartment::Cylindrical_compartment(
    const modigliani::Real newLength,
    const modigliani::Real newDiameter, const modigliani::Real newCm,
    const modigliani::Real newRa,
    const modigliani::Real newTemperature)
    : Membrane_compartment(M_PI * newDiameter * newLength,
                           newTemperature, newCm, newRa),
      length_(newLength), diameter_(
          newDiameter), volume_(modigliani::pi_ *
                                (newDiameter *
                                 newDiameter / 4) * newLength) {
}

Cylindrical_compartment::~Cylindrical_compartment() {
}
}  // namespace modigliani
