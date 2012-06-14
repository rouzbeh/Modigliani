/**
 * @file Spherical_compartment.c++
 * Spherical_compartment class implementation
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

/* $Id: ntbp_spherical_compartment_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_spherical_compartment_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2003/04/08 15:13:36  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "Spherical_compartment.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Spherical_compartment */
Spherical_compartment::Spherical_compartment(modigliani_base::Real newRadius,
		modigliani_base::Real newCm) :
		Membrane_compartment(4 * M_PI * newRadius * newRadius) {
	radius = newRadius;
	SetCM(newCm);
	Set_rA(35.4);
}

/* ***      COPY AND ASSIGNMENT	***/
Spherical_compartment::Spherical_compartment(
		const Spherical_compartment & original) :
		Membrane_compartment(original.area()) {
	// add assignment code here
	radius = original.radius;
}

Spherical_compartment&
Spherical_compartment::operator=(const Spherical_compartment & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Spherical_compartment::~Spherical_compartment() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 void
 Spherical_compartment::() const
 {
 }
 */

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
