/**
 * @file custom_cylindrical_compartment.h
 *
 * @brief Custom_cylindrical_compartment class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
 * Copyright (C) 2013 Ali Neishabouri
 *
 * @section LICENSE
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_CUSTOM_CYLINDRICAL_COMPARTMENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_CUSTOM_CYLINDRICAL_COMPARTMENT_H_

#include <vector>
#include <string>
#include "modigliani_core/cylindrical_compartment.h"

namespace modigliani_core {

  using modigliani_base::Real;

  struct custom_current {
    Membrane_current *current_ptr;
    Real inside_concentration;
    Real outside_concentration;
    Real reversal_potential;
    bool track;
  };

/**
 * @brief This class is used to take concentrations into account.
 *
 * This compartment can track ionic concentrations
 * to change reversal potentials in real time.
 */
  class Custom_cylindrical_compartment:public Cylindrical_compartment {
  public:
    /**
     * @brief Construct a new cylindrical compartment
     *
     * @param length Length in [@f$\si{\micro\meter}@f$]
     * @param diameter Diameter in [@f$\si{\micro\meter}@f$]
     * @param cM Membrane capacitance in
     * [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
     * @param rA Axoplasmic resistance in
     * [@f$\si{\ohm\centi\meter}@f$]
     * @param newTemperature Temperature in [@f$\si{\celsius}@f$]
     * @param vouvinratio Ratio of external volume to internal volume.
     * The default value is taken from Ng, L.J. (2013). Computer
     * Modeling of Mild Axonal Injury. Neural Comput.
     */
    Custom_cylindrical_compartment(const modigliani_base::Real length,
                                   const modigliani_base::Real diameter,
                                   const modigliani_base::Real cM,
                                   const modigliani_base::Real rA,
                                   const modigliani_base::Real newTemperature,
                                   const modigliani_base::Real vouvinratio = 8);

     Custom_cylindrical_compartment(const Custom_cylindrical_compartment &
                                    original) = delete;

     Custom_cylindrical_compartment & operator=(const
                                                Custom_cylindrical_compartment &
                                                right) = delete;

     virtual ~Custom_cylindrical_compartment();

    /**
     * @brief Adds a new current to the compartment.
     *
     * The reversal potential for currents added using this method is
     * updated according to the relevant concentrations.
     *
     * @param currentPtr Pointer to the Membrane_current object
     * @param concentration_inside Inside concentration in
     * [@f$\si{\milli M}@f$]
     * @param concentration_outside Outside concentration in
     * [@f$\si{\milli M}@f$]
     */
    virtual modigliani_base::
      ReturnEnum AttachCurrentWithConcentrations(Membrane_current * currentPtr,
                                                 Real concentration_inside,
                                                 Real concentration_outside);

    /**
     * @brief Adds a new current to the compartment. Sets relevant
     * concentrations to zero.
     *
     * The reversal potential for currents added using this method in
     * not updated.
     *
     * @param currentPtr Pointer to the Membrane_current object
     * @param type Current type
     */
    virtual modigliani_base::ReturnEnum AttachCurrent(Membrane_current *
                                                      currentPtr,
                                                      CurrentType type =
                                                      IONIC) override;

    /**
     * @brief Runs one step of simulation and update ionic
     * concentrations accordingly
     * @param newVM Membrane potential in [@f$\si{\milli\volt}@f$]
     */
    virtual modigliani_base::ReturnEnum Step
      (const modigliani_base::Real newVM) override;

    /**
     * @brief Opens an output file, writes the header, and use it
     * to write data at each step.
     *
     * @param output_file_name Name of output file
     * @return Success status
     * @warning Call after having attached all currents.
     */
    virtual modigliani_base::ReturnEnum SetupOutput(std::
                                                    string output_file_name)
      override;

    /**
     * @brief Writes the membrane potential, reversal potential,
     * inside and outside concentrations and current flwoing through
     * all attached Membrane_current instances to a binary file.
     *
     * @return Success status
     * @warning Call after having attached all currents.
     */
    virtual modigliani_base::ReturnEnum WriteOutput() const override;

    /**
     * @brief Returns the internal ionic concentration.
     *
     * @param currentIndex Index of the current
     * @return Inside concentration in [@f$\si{\milli M}@f$]
     * @warning Current indexed [1..m]
     */
    const Real InsideConcentration(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].inside_concentration);
    }
    /**
     * @brief Returns the external ionic concentration.
     *
     * @param currentIndex Index of the current
     * @return Outside concentration in [@f$\si{\milli M}@f$]
     * @warning Current indexed [1..m]
     */ const Real OutsideConcentration(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].outside_concentration);
    }
    /**
     * @brief Returns the reversal potential.
     *
     * @param currentIndex Index of the current
     * @return Reversal potential in [@f$\si{\milli\volt}@f$]
     * @warning Current indexed [1..m]
     */ const Real ReversalPotential(modigliani_base::Size currentIndex) const {
      assert((currentIndex > 0) && (currentIndex - 1 < NumberCurrents()));
      return (custom_current_vec_[currentIndex - 1].reversal_potential);
    }
    /**
     * @brief Sets the temperature
     *
     * Change of temperature also affects reversal potentials.
     *
     * @param newTemp Temperature in [@f$\si{celsius}@f$]
     * @return Success or failure
     */ virtual modigliani_base::ReturnEnum set_temperature
     (modigliani_base::Real newTemp) override {
      Membrane_compartment::set_temperature(newTemp);
      nernst_multiplier = 1000 * modigliani_base::R *
        (newTemp + modigliani_base::ZERO_CELSIUS) / modigliani_base::F;
      return (modigliani_base::ReturnEnum::SUCCESS);
    }
    /**
     * @brief Sets the internal ionic concentration.
     *
     * @param currentIndex Index of the current
     * @param new_concentration Concentration in [@f$\si{\milli M}@f$]
     * @warning Current indexed [1..m]
     */ void SetInsideConcentration(modigliani_base::Size currentIndex,
                                    Real new_concentration);

    /**
     * @brief Sets the external ionic concentration.
     *
     * @param currentIndex Index of the current
     * @param new_concentration Concentration in [@f$\si{\milli M}@f$]
     * @warning Current indexed [1..m]
     */
    void SetOutsideConcentration(modigliani_base::Size currentIndex,
                                 Real new_concentration);

     private: std::vector < custom_current > custom_current_vec_;
    const modigliani_base::Real volumeratio;
     modigliani_base::Real nernst_multiplier;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_CUSTOM_CYLINDRICAL_COMPARTMENT_H_
