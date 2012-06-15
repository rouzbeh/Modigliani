/**
 * @file membrane_current.cc
 * Membrane_current class implementation
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

#include "modigliani_core/membrane_current.h"

namespace modigliani_core {

/* ***      CONSTRUCTORS	***/
/** Create a Membrane_current */
Membrane_current::Membrane_current(
    modigliani_base::Real newReversalPotential /* in mV */)
    : Object() {
  simulationMode = NTBP_DETERMINISTIC;
  reversalPotential = newReversalPotential;  //in mV
  current_ = 0.0;  // nanoAmpere
  temperature = 6.3;  // SGA values
  q10 = 2.0;  // SGA values
}

/* ***      COPY AND ASSIGNMENT	***/
Membrane_current::Membrane_current(
    const Membrane_current __attribute__((unused)) & original)
    : Object() {
  // add assignment code here
}

Membrane_current&
Membrane_current::operator=(const Membrane_current & right) {
  if (this == &right) return (*this);  // Gracefully handle self assignment
  // add assignment code here
  return (*this);
}

/* ***      DESTRUCTOR		***/
Membrane_current::~Membrane_current() {
}
}
