/**
 * @file leak_current.h
 * @brief Leak_current class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 *
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_LEAK_CURRENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_LEAK_CURRENT_H_
#include <assert.h>
#include "modigliani/membrane_current.h"

namespace modigliani {
/**
 * @brief Simulates a leak current
 */
class Leak_current : public Membrane_current {
  public:
/**
 * @brief Constructs a leak current instance
 * @param newArea Membrane surface area in
 * [@f$\si{\micro\meter\squared}@f$]
 * @param newLeakConductance Leak conductance in
 * [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
 * @param newReversalPotential Reversal potential in [@f$\si{\milli\volt}@f$]
 */
    Leak_current(modigliani::Real newArea,
                 modigliani::Real newLeakConductance,
                 modigliani::Real newReversalPotential);

    Leak_current(const Leak_current & original) = delete;
    Leak_current & operator=(const Leak_current & right) = delete;
    virtual ~Leak_current();

/**
 * @brief Runs one step of the simulation
 *
 * This function simply returns SUCCESS
 * @return SUCCESS
 */
    modigliani::ReturnEnum StepCurrent() {
      return (modigliani::ReturnEnum::SUCCESS);
    }

/**
 * @brief Compute and return conductance
 * @return Conductance in [@f$\si{\milli\siemens}@f$]
 */
    void UpdateConductance() {
      set_conductance(MaxConductivity() * area_ * 1.0e-8);
    }

/**
 * @brief Return leak conductance
 * @return Leak conductance in [@f$\si{\milli\siemens}@f$]
 * @warning function return constant value (leak!) set in
 * constructor)
 */
    modigliani::Real ComputeConductance() override {
      return (conductance());
    }

/**
 * @brief Return leak conductanceleak conductivity
 * @return Leak conductivity in [@f$\si{\milli\siemens\per\centi\meter\squared}@f$]
 */
    modigliani::Real MaxConductivity() const {
      return (max_conductivity_);
    }

  private:
    // in mSiemens / centiMeter^2
    const modigliani::Real max_conductivity_;
    // in mumeter^2
    const modigliani::Real area_;
    // in mSiemens per channel
    modigliani::Real conductivity_;
};
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_CORE_LEAK_CURRENT_H_
