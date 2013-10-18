/**
 * @file custom_cylindrical_compartment.h
 *
 * @brief This class is used to take concentrations into account
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Ali Neishabouri
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

#ifndef _modigliani_core_custom_cylindrical_compartment_h_
#define _modigliani_core_custom_cylindrical_compartment_h_

#include "cylindrical_compartment.h"

namespace modigliani_core {

using modigliani_base::Real;

struct custom_current {
    Membrane_current* current_ptr;
    Real inside_concentration;
    Real outside_concentration;
    Real reversal_potential;
    bool track;
};
/**
 * @brief This compartment can track ionic concentrations
 * to change reversal potentials.
 */
class Custom_cylindrical_compartment : public Cylindrical_compartment {
  public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Custom_cylindrical_compartment(
        const modigliani_base::Real length /* muMeter */,
        const modigliani_base::Real diameter /* muMeter */,
        const modigliani_base::Real cM /* muFarad/cm^2 */,
        const modigliani_base::Real rA /* ohm cm */,
        const modigliani_base::Real newTemperature,
        const modigliani_base::Real vouvinratio = 8); //Ng, L.J. (2013). Computer Modeling of Mild Axonal Injury. Neural Comput.

    Custom_cylindrical_compartment(
        const Custom_cylindrical_compartment & original) = delete;

    Custom_cylindrical_compartment & operator=(
        const Custom_cylindrical_compartment & right) = delete;

    virtual ~Custom_cylindrical_compartment();

    /* ***  Methods              ***/
    virtual modigliani_base::ReturnEnum AttachCurrentWithConcentrations(
        Membrane_current * currentPtr, Real concentration_inside,
        Real concentration_outside);

    modigliani_base::ReturnEnum AttachCurrent(
        Membrane_current * currentPtr, CurrentType type = IONIC);

    virtual modigliani_base::ReturnEnum Step(const modigliani_base::Real newVM);

    /**
     * @brief Opens an output file, writes the header, and use it
     * to write data at each step.
     *
     * \param output_file_name
     * \return Success status
     * \warning Call after having attached all currents.
     */
    virtual modigliani_base::ReturnEnum SetupOutput(
        std::string output_file_name) override;

    virtual modigliani_base::ReturnEnum WriteOutput() const override;

    const Real InsideConcentration(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].inside_concentration);
    }

    const Real OutsideConcentration(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].outside_concentration);
    }

    const Real ReversalPotential(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].reversal_potential);
    }

    virtual modigliani_base::ReturnEnum  set_temperature(modigliani_base::Real newTemp) override {
      Membrane_compartment::set_temperature(newTemp);
      nernst_multiplier = 1000 * modigliani_base::R * (newTemp + modigliani_base::ZERO_CELSIUS) / modigliani_base::F;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }

    void SetInsideConcentration(modigliani_base::Size currentIndex,
                                Real new_concentration);

    void SetOutsideConcentration(modigliani_base::Size currentIndex,
                                 Real new_concentration);


private :
    std::vector<custom_current> custom_current_vec_;
    const modigliani_base::Real volumeratio;
    modigliani_base::Real nernst_multiplier;
};
}  // namespace modigliani_core
#endif /* _modigliani_core_custom_cylindrical_compartment.h_ */
