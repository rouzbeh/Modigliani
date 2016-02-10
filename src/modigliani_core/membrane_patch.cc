/**
 * @file membrane_patch.cc
 * @brief Membrane_patch class implementation
 *
 * Copyright (C) 1998,1999,2000,2001,2002 Ahmed Aldo Faisal
 * Copyright 2011 Ali Neishabouri
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani_core/membrane_patch.h"

namespace modigliani_core {
  Membrane_patch::Membrane_patch(modigliani_base::Real newArea,
                                 modigliani_base::Real newCM)
  :Membrane_compartment(newArea, 6.3, newCM, 0) {
  } Membrane_patch::~Membrane_patch() {
  }

  inline modigliani_base::ReturnEnum Membrane_patch::Step() {
    /*       [mV] == [10^-3 ms nA /muF] */
    modigliani_base::Real deltaV = 1e-3 * timestep()
      * CompartmentMembraneNetCurrent() / CompartmentMembraneCapacitance();
    set_vm(vm() + deltaV);
    Membrane_compartment::Step(vm());
    return (modigliani_base::ReturnEnum::SUCCESS);
  }

  modigliani_base::ReturnEnum Membrane_patch::InitialStep() {
    set_timestep(timestep() / 2.0);
    Step();
    set_timestep(timestep() * 2.0);

    std::cerr
      << "NTBP_membrane_compartment_sequence_o::InitialStep() "
      << "- ERROR : not correctly implemented ? untested."
      << std::endl;
    return (modigliani_base::ReturnEnum::SUCCESS);
  }
}  // namespace modigliani_core
