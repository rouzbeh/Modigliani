/**\file ntbp_channels_current_obj.cpp - Channels_current class implementation
 * by Ahmed Aldo Faisal &copy; created 16.3.2001  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
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

/* $Id: ntbp_channels_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_channels_current_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "Channels_current.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Channels_current */
Channels_current::Channels_current(NTreal newReversalPotential, //in mV
		NTreal newDensity, // channels per mumeter^2
		NTreal newArea, // in mumeter^2
		NTreal newConductivity // in mSiemens per channel
		) :
		Membrane_current(newReversalPotential) {

	SetSimulationMode(NTBP_SINGLECHANNEL);
	density = newDensity;
	area = newArea;
	conductivity = newConductivity;
}

/* ***      COPY AND ASSIGNMENT	***/
Channels_current::Channels_current(const Channels_current & original) :
		Membrane_current(original._reversalPotential()) {
	// add assignment code here
}

const Channels_current&
Channels_current::operator=(const Channels_current & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Channels_current::~Channels_current() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown

 void
 Channels_current::() const
 {
 }
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
