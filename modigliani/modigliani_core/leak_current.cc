/**
 * @file leak_current.cc
 * \brief Leak_current class implementation
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

#include "modigliani_core/leak_current.h"
namespace modigliani_core {

Leak_current::Leak_current(modigliani_base::Real newArea,
                           modigliani_base::Real newLeakConductance,
                           modigliani_base::Real newReversalPotential) :
    Membrane_current(newReversalPotential),
    area_(newArea),
    max_conductivity_(newLeakConductance) {
  assert(max_conductivity_ >= 0);
  assert(area_ >= 0);
  set_simulation_mode(DETERMINISTIC);
  UpdateConductance();
}

Leak_current::~Leak_current() {
}

}  // namespace modigliani_core
