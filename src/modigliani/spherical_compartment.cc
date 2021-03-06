/**
 * @file spherical_compartment.cc
 * Spherical_compartment class implementation
 * @author Ali Neishabouri
 * @version   0.6
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

#include "modigliani/spherical_compartment.h"

namespace modigliani {
Spherical_compartment::Spherical_compartment(modigliani::Real newRadius,
                                             modigliani::Real newCm,
                                             modigliani::Real newTemperature)
  : Membrane_compartment
    (4 * M_PI * newRadius * newRadius, newTemperature,
    newCm, 35.4), radius_(newRadius) {}

Spherical_compartment::Spherical_compartment(
  boost::property_tree::ptree compartment_parameters)
  : Membrane_compartment(
    4 * M_PI * compartment_parameters.get<double>("radius")
    * compartment_parameters.get<double>("radius"),
    6.3, compartment_parameters.get<double>("Cm"),
    compartment_parameters.get<double>("Ra")), radius_(
    compartment_parameters.get<double>("radius")) {}

Spherical_compartment::~Spherical_compartment() {}
} // namespace modigliani
