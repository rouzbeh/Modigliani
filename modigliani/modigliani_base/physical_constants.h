/**
 * @file physical_constants.h
 * Defintions of physical constants
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_
#define MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_

#include <modigliani_base/main.h>

namespace modigliani_base {
/* Data from the IUPAC 2003 website */
const Real ZERO_CELSIUS = Real(273.15) /* [K] */;
const Real ZERO_KELVIN = Real(-273.15) /* [C] */;
const Real kBoltzmann = Real(1.380658*10e-23);  // [J K^-1]
const Real PLANCK = Real(6.6260755*10e-34) /* [J s], Planck's constant (h) */;
const Real kProtonCharge = Real(1.60217733*10e-19);  // [C]

const Real R = 8.3144621;
const Real F = 96485.3415;
}
#endif  // MODIGLIANI_MODIGLIANI_BASE_PHYSICAL_CONSTANTS_H_
