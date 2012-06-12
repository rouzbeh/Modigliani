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


/* $Id: nt_physical_constants.h,v 1.1 2003/01/30 17:13:50 face Exp $
 * $Log: nt_physical_constants.h,v $
 * Revision 1.1  2003/01/30 17:13:50  face
 * *** empty log message ***
 *
 */


#ifndef _MMATH_PHYSICAL_CONSTAMS_H_
#define _MMATH_PHYSCIAL_CONSTAMS_H_

#include "main.h"

namespace mbase{

/* Data from the IUPAC 2003 website */
const Real ZERO_CELSIUS = Real(273.15) /* [K] */;
const Real ZERO_KELVIN = Real(-273.15) /* [C] */;
const Real BOLTZMANN = Real(1.380658*10e-23) /* [J K^-1], Boltzmann's constant (k, k_B) */;
const Real PLANCK = Real(6.6260755*10e-34) /* [J s], Planck's constant (h) */;
const Real PROTON_CHARGE = Real(1.60217733*10e-19) /* [C], elementary charge of a proton */;

}
#endif /* _MMATH_PHYSICAL_CONSTAMS_H_ */


