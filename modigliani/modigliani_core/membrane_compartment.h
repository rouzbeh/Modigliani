/**
 * @file membrane_compartment.h
 * Membrane_compartment class header
 * @author Ahmed Aldo Faisal &copy; created 16.3.2001  
 * @version   0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _modigliani_core_membrane_compartment_h_
#define _modigliani_core_membrane_compartment_h_

#include "modigliani_core/object.h"
#include "modigliani_core/membrane_current.h"
/* other includes */
#include <vector>

namespace modigliani_core {

/**
 * @short membrane_compartment class acts as a "container + glue" for the various membrane_current
 classes that can be attached to it.

 \bug unknown
 \warning 1. rA, rM and cM are set to (arbitrary) standard values, overwrite in derived class
 2. leak current IS incorported into the COMPARTMENT VECTOR
 */
class Membrane_compartment : public Object {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Membrane_compartment(modigliani_base::Real area /* in muMeter^2 */,
                         modigliani_base::Real newTemperature = 6.3);
    Membrane_compartment(const Membrane_compartment & original) = delete;
    Membrane_compartment & operator=(const Membrane_compartment & right) = delete;

    virtual ~Membrane_compartment();
    /* ***  Methods              ***/
    modigliani_base::ReturnEnum AttachCurrent(Membrane_current * currentPtr,
                                              NTBPcurrentType type = NTBP_IONIC);
    modigliani_base::ReturnEnum step(modigliani_base::Real newVM /* mV */);
    modigliani_base::ReturnEnum InjectCurrent(
        modigliani_base::Real current /* in nA */);
    modigliani_base::Real AttachedCurrent(modigliani_base::Size currentIndex) {
      M_ASSERT((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]->current());
    }
    modigliani_base::Real AttachedConductance(
        modigliani_base::Size currentIndex) {
      M_ASSERT((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]->conductance());
    }
    modigliani_base::Real AttachedReversalPotential(
        modigliani_base::Size currentIndex) {
      M_ASSERT((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]->reversal_potential());
    }
    /**  membrane time constant at instaneous membrane conductivity in ms */
    modigliani_base::Real TimeConstant() {
      return ((cm() / total_conductance()) * area() * 1.0e8);
    }
    const Membrane_current * Current(modigliani_base::Size currentIndex) {
      M_ASSERT((currentIndex > 0) && (currentIndex - 1 < current_vec_.size()));
      return (current_vec_[currentIndex - 1]);
    }
    /* in muMeter^2 */
    modigliani_base::Real area() /* in muMeter^2 */const {
      return (area_);
    }
    /* in mV */
    modigliani_base::Real vm() /* in mV muMeter */const {
      return (vm_);
    }
    void set_vm(modigliani_base::Real newVoltage) {
      vm_ = newVoltage;
    }
    /** Set specific membrane capacitance in muF / cm^2 */
    void SetCM(modigliani_base::Real newCm /* in muF / cm^2 */) {
      cm_ = newCm;
      compartment_membrane_capacitance_ = CompartmentMembraneCapacitance();
    }
    /* in muF / cm^2 */
    modigliani_base::Real cm() const {
      return (cm_);
    }
    /** Set specific axoplasmic resistivity in Ohm cm */
    void Set_rA(modigliani_base::Real newRa /* in Ohm cm */) {
      ra_ = newRa;
    }
    /* in Ohm cm */
    modigliani_base::Real ra() const {
      return (ra_);
    }
    /* Set temperature [Celsius] in compartment and for all currents within compartment (affects future attached ones also) */
    modigliani_base::ReturnEnum set_temperature(
        modigliani_base::Real newTemp /* in Celsius */) {
      temperature_ = newTemp;
      for (modigliani_base::Size i = 0; i < current_vec_.size(); i++)
        current_vec_[i]->set_temperature(newTemp);
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /* in Celsius */
    modigliani_base::Real temperature() const {
      return (temperature_);
    }
    /* in muF */
    modigliani_base::Real compartment_membrane_capacitance() const {
      return (compartment_membrane_capacitance_);
    }
    /* in muF */
    modigliani_base::Real CompartmentMembraneCapacitance() const;
    /* in nA */
    modigliani_base::Real CompartmentMembraneNetCurrent() const;
    /** in 1/mSec or 1 kHz*/
    modigliani_base::Real CompartmentChannelStateTimeConstant() const;
    /**  */
    bool GillespieStep();
    /**  */
    void show_param() const;
    modigliani_base::Size NumberCurrents() const;
    Membrane_current const * GetCurrent(modigliani_base::Size i) const;
    /* ***  Data                 ***/

  protected:
    /* ***  Methods              ***/
    modigliani_base::Real total_conductance() const;
    modigliani_base::Real WeightedConductance() const;  // OBSOLETE?
    /* ***  Data                 ***/
    modigliani_base::Real i_inj_;  // injected current into compartment in nA
    std::vector<Membrane_current *> current_vec_;

  private:
    modigliani_base::Real cm_;  // membrane capacity in muFarad/cm^2
    modigliani_base::Real ra_;  // axoplasmatic resistance in Ohm cm
    modigliani_base::Real area_;  // in muMeter^2
    modigliani_base::Real temperature_;  // in Celsius
    modigliani_base::Real compartment_membrane_capacitance_;  // in muFarad
    modigliani_base::Real vm_;  // membrane voltage in mV
};
}
#endif /* _modigliani_core_membrane_compartment.h_ */

/* File skeleton generated by GenNTObj version 0.7. */

