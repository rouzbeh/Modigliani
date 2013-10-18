/**
 * @file membrane_current.cc
 * @brief Membrane_current class implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Mohammad Ali Neishabouri
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

#include "modigliani_core/membrane_current.h"

namespace modigliani_core {

Membrane_current::Membrane_current(
    modigliani_base::Real newReversalPotential)
    : Object() {
  _init(newReversalPotential);
}

void Membrane_current::_init(modigliani_base::Real newReversalPotential) {
  voltage_;
  conductance_;
  simulation_mode_ = DETERMINISTIC;
  reversal_potential_ = newReversalPotential;
  current_;
  temperature_;
  q10_;
}

Membrane_current::~Membrane_current() {
}
}  // namespace modigliani_core
