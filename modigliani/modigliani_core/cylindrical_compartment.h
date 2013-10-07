/**
 * @file cylindrical_compartment.h
 * @brief Cylindrical_compartment class header
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_CYLINDRICAL_COMPARTMENT_H_
#define MODIGLIANI_MODIGLIANI_CORE_CYLINDRICAL_COMPARTMENT_H_

#include "modigliani_core/membrane_compartment.h"

namespace modigliani_core {

/**
 * @brief Cylindrical membrane compartment. Very useful to simulate axons
 *
 */
class Cylindrical_compartment : public Membrane_compartment {
  public:
/**
 * @brief Constructs a cylindrical compartment from dimensions and
 *    electrophysiological data.
 *
 * @param newLength length in [@f$\mu m@f$]
 * @param newDiameter diameter in [@f$\mu m@f$]
 * @param newCm Membrane capacitance in [@f$\mu F / cm^2@f$]
 * @param newRa Axoplasmic resistance in [@f$\Omega cm@f$]
 * @param newTemperature Temperature in [C]
 */
    Cylindrical_compartment(const modigliani_base::Real newLength,
                            const modigliani_base::Real newDiameter,
                            const modigliani_base::Real newCm,
                            const modigliani_base::Real newRa,
                            const modigliani_base::Real newTemperature);

    Cylindrical_compartment(const Cylindrical_compartment & original) = delete;

    Cylindrical_compartment & operator=(const Cylindrical_compartment & right)
        = delete;

    virtual ~Cylindrical_compartment();

/**
 * @brief length constant in muMeter for instantenous membrane
 * conductance
 * @return Length constant in [@f$\mu m@f$]
 */
    virtual modigliani_base::Real ActiveLengthConstant() const {
      return (diameter() / (4.0 * ra() * 1.0e4 * WeightedConductance()));
    }

/**
 * @brief Returns the cross-sectional area of the cylinder
 * @return Cross-sectional area in [@f$\mu m^2@f$]
 */
    modigliani_base::Real _CrosssectionalArea() const {
      return (radius() * radius() * M_PI);
    }

/**
 * @return Diameter in [@f$\mu m@f$]
 */
    modigliani_base::Real diameter() const {
      return (diameter_);
    }

/**
 * @return Radus of the cylinder in [@f$\mu m@f$]
 */
    modigliani_base::Real radius() const {
      return (diameter() / 2.0);
    }

/**
 * @return length in [@f$\mu m@f$]
 */
    modigliani_base::Real length() const {
      return (length_);
    }

/**
 * @return volume in [@f$\mu m^3@f$]
 */
    modigliani_base::Real volume() const {
      return (volume_);
    }

  private :
    // in muMeter
    const modigliani_base::Real diameter_;
    // in muMeter
    const modigliani_base::Real length_;
    // in muMeter^3
    const modigliani_base::Real volume_;
  };
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_CYLINDRICAL_COMPARTMENT_H_
