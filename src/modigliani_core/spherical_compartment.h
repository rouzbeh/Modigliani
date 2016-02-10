/**
 * \file spherical_compartment.h
 * \brief Spherical_compartment class header
 *
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

#ifndef  MODIGLIANI_MODIGLIANI_CORE_SPHERICAL_COMPARTMENT_H_
#define  MODIGLIANI_MODIGLIANI_CORE_SPHERICAL_COMPARTMENT_H_

#include <boost/property_tree/ptree.hpp>
#include <cmath>

#include "modigliani_core/membrane_compartment.h"

namespace modigliani_core {
/**
 * @brief This class simulates a spherical Hodgkin-Huxley neuron
 */
class Spherical_compartment : public Membrane_compartment {
  public:
/**
 * @brief Constructs a new spherical compartment
 * @param radius Radius in [@f$\si{\micro\meter}@f$]
 * @param cM Membrane capacitance in
 * [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
 */
    Spherical_compartment(modigliani_base::Real radius,
                          modigliani_base::Real cM,
                          modigliani_base::Real newTemperature);

/**
 * @brief Constructs a new spherical compartment using parameters
 * given in a ptree.
 * @param compartment_parameters ptree containing parameters necessary
 * for the compartment. The ptree is expected to contain the following
 * values :
 *   - double radius Radius in [@f$\si{\micro\meter}@f$]
 *   - double Cm Membrane capacitance in
 * [@f$\si{\micro\farad\per\centi\meter\squared}@f$]
 *   - double Ra Axoplasmic resistance in [@f$\si{\ohm\centi\meter}@f$]
 */
    explicit Spherical_compartment(boost::property_tree::ptree
        compartment_parameters);

    Spherical_compartment(
        const Spherical_compartment & original) = delete;

    Spherical_compartment & operator=(
        const Spherical_compartment & right) = delete;

    virtual ~Spherical_compartment();

/**
 * @brief Returns the radius.
 * @return Radius in [@f$\si{\micro\meter}@f$]
 */
    modigliani_base::Real radius() const {
      return (radius_);
    }

/**
 * @brief Returns the length.
 * @return Radius in [@f$\si{\micro\meter}@f$]
 */
    modigliani_base::Real length() const {
      return (radius_);
    }

  private:
    const modigliani_base::Real radius_;
};
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_SPHERICAL_COMPARTMENT_H_
