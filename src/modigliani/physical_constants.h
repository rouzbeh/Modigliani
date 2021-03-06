/**
 * @file physical_constants.h
 * @brief Defintions of physical constants.
 *
 * Data from IUPAC 2003 website.
 *
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_
#define MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_

#include <modigliani/obj.h>

namespace modigliani {
  const Real ZERO_CELSIUS = Real(273.15);  /* [K] */
  const Real ZERO_KELVIN = Real(-273.15);  /* [C] */
  const Real kBoltzmann = Real(1.380658 * 10e-23);  // [J K^-1]
  const Real PLANCK =
      Real(6.6260755 * 10e-34);  /* [J s], Planck's constant (h) */
  const Real kProtonCharge = Real(1.60217733 * 10e-19);  // [C]
  
  const Real R = 8.3144621;
  const Real F = 96485.3415;
}  // namespace modigliani
#endif  // MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_
