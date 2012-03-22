/* nt_physical_constants.h - defintions of physical constants*/
/* by Ahmed A. Faisal, 22. 5. 1998(c) */

/*
 * NetTrader - finance management, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
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


/* $Id: nt_physical_constants.h,v 1.1 2003/01/30 17:13:50 face Exp $
 * $Log: nt_physical_constants.h,v $
 * Revision 1.1  2003/01/30 17:13:50  face
 * *** empty log message ***
 *
 */


#ifndef _NT_PHYSICAL_CONSTANTS_H_
#define _NT_PHYSCIAL_CONSTANTS_H_

#include "nt_main.h"

/* Data from the IUPAC 2003 website */
#define NT_0_CELSIUS NTreal(273.15) /* [K] */
#define NT_0_KELVIN NTreal(-273.15) /* [C] */
#define NT_BOLTZMANN NTreal(1.380658*10e-23) /* [J K^-1], Boltzmann's constant (k, k_B) */
#define NT_PLANCK NTreal(6.6260755*10e-34) /* [J s], Planck's constant (h) */
#define NT_PROTON_CHARGE NTreal(1.60217733*10e-19) /* [C], elementary charge of a proton */

#endif /* _NT_PHYSICAL_CONSTANTS_H_ */


