/**
 * @file leak_current.h
 * Leak_current class header
 * @author Ahmed Aldo Faisal &copy; created 19.3.2001
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

#ifndef _modigliani_core_leak_current_h_
#define _modigliani_core_leak_current_h_

#include "membrane_current.h"

namespace modigliani_core {
/**
 * @short Leak_current class
 * \bug unknown
 * \warning unknown
 */
class Leak_current : public Membrane_current {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Leak_current(modigliani_base::Real newArea /* in mu^2 */,
                 modigliani_base::Real newLeakConductance = 0.3 /* mS/cm^2 */,
                 modigliani_base::Real newReversalPotential = 10.613 /* mV */);

    Leak_current(const Leak_current & original) = delete;
    Leak_current & operator=(const Leak_current & right) = delete;
    virtual ~Leak_current();

    virtual void reset() override {
    }
    ;
    /** in mSiemens */
    modigliani_base::ReturnEnum StepCurrent() {
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /** compute and return conductance in mSiemens */
    void UpdateConductance() {
      set_conductance(max_conductivity() * area_ * 1.0e-8);
    }
    /** Return leak conductance in mSiemens  (note: function return constant value (leak!) set in constructor) */
    modigliani_base::Real ComputeConductance() override {
      return (conductance());
    }
    void Show() {
      std::cout << "g_Leak [mSiemens]=" << conductance();
    }

    /** in mSiemens/cm^2 */
    modigliani_base::Real max_conductivity() const {
      return (max_conductivity_);
    }
    /** in muMeter^2 */
    modigliani_base::Real area() const {
      return (area_);
    }
  protected:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
  private:
    /* ***  Methods              ***/
    /* ***  Data                 ***/
    /// in mSiemens / centiMeter^2
    const modigliani_base::Real max_conductivity_;
    /// in mumeter^2
    const modigliani_base::Real area_;
    /// in mSiemens per channel
    modigliani_base::Real conductivity_;
};
}
#endif /* _modigliani_core_leak_current_h_ */
