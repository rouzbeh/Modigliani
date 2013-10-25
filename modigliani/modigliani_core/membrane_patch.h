/**
 * @file membrane_patch.h
 * @brief Membrane_patch class header
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
#ifndef MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_PATCH_H_
#define MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_PATCH_H_

#include "modigliani_core/membrane_compartment.h"

namespace modigliani_core {
/**
 * @brief Membrane_patch class
 */
  class Membrane_patch:public Membrane_compartment {
  public:
    /**
     * @brief Constructor
     * @param area Membrane surface area in
     * [@f$\si{\micro\meter\squared}@f$]
     * @param newCM Membrane capcacitance in
     * [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
     */
    Membrane_patch(modigliani_base::Real area,
                   modigliani_base::Real newCM);

    Membrane_patch(const Membrane_patch & original) = delete;

    Membrane_patch & operator=(const Membrane_patch & right) = delete;

    virtual ~Membrane_patch();

    /**
     * @brief Runs one step of simulation
     * @return Success or failure
     */
    modigliani_base::ReturnEnum Step();

    /**
     * @brief Runs an initial step, needed if time staggering of the
     * differential equations solution is desired.
     * @return Success or failure
     */
    modigliani_base::ReturnEnum InitialStep();

    /**
     * @brief Returns total memrane current
     * @return Current in [@f$\si{\nano\ampere}@f$]
     */
    modigliani_base::Real MembraneCurrent(modigliani_base::Size
                                           currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (Current(currentIndex)->current());
    }
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_PATCH_H_
