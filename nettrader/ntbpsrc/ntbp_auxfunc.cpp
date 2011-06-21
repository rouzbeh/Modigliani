/**\file ntbp_auxfunc.cpp - NTBP auxiliary function implementation for heterogeneous object creation
 * by Ahmed Aldo Faisal &copy; created 22.4.2005  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998-2005 Ahmed Aldo Faisal    
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

/* $Id: ntbp_auxfunc.cpp,v 1.1 2005/04/25 13:55:27 face Exp $
 * $Log: ntbp_auxfunc.cpp,v $
 * Revision 1.1  2005/04/25 13:55:27  face
 * *** empty log message ***
 *
 */

#include "ntbp_auxfunc.h"

NTreal NTBP_corrected_channel_density(NTreal chDensity, NTreal compArea) {
	NTreal chPerCompartment = compArea * chDensity;
	NTreal pChFloor = (ceil(chPerCompartment) - chPerCompartment);

	NT_uniform_rnd_dist_o uniRnd;
	/* compute number of channels, such that average density is achieved */
	NTreal indChDensity = (uniRnd.RndVal() > pChFloor ? ceil(chPerCompartment)
			/ compArea : floor(chPerCompartment) / compArea);

	return indChDensity;
}
