/**
 * @file object.cc
 * @brief Bio-physics related main object implementation
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
#include "modigliani_core/object.h"

namespace modigliani_core {

modigliani_base::Real TemperatureRateRelation(modigliani_base::Real temp,
                                              modigliani_base::Real baseTemp,
                                              modigliani_base::Real q10) {
  return (pow(q10, (temp - baseTemp) / 10.0));
}

modigliani_base::Real
LengthConstantPassiveCable(modigliani_base::Real diameter,
                           modigliani_base::Real rAxoplasmic,
                           modigliani_base::Real gLeak) {
  return (1.0e6 * sqrt(1.0e-5 * diameter / (gLeak * rAxoplasmic * 4.0)));
}

Object::Object() {
  timestep_ = TIMESTEP;
}

Object::~Object() {
}

}  // namespace modigliani_core
