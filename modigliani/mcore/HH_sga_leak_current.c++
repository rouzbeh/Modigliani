/**\file HH_sga_leak_currentbj.cpp - HH_sga_leak_current class implementation
 * by Ahmed Aldo Faisal &copy; created 19.3.2001  
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

/* $Id: ntbp_hh_sga_leak_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_hh_sga_leak_current_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.3  2003/08/14 16:43:27  face
 * *** empty log message ***
 *
 * Revision 1.2  2003/06/19 11:05:03  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "HH_sga_leak_current.h"
using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a HH_sga_leak_current */
HH_sga_leak_current::HH_sga_leak_current(mbase::Real newArea,
		mbase::Real newLeakConductance, mbase::Real newReversalPotential /* mV */) :
		HH_current(newReversalPotential /* mV */,
				newLeakConductance /* in mSiemens/cm^2 */,
				newArea /* in muMeter^2 */) {
	UpdateConductance();
}

/* ***      COPY AND ASSIGNMENT	***/
HH_sga_leak_current::HH_sga_leak_current(const HH_sga_leak_current & original) :
		HH_current(original._reversalPotential(), original._maxConductivity(),
				original._area()) {
	// add assignment code here
}

const HH_sga_leak_current&
HH_sga_leak_current::operator=(const HH_sga_leak_current & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
HH_sga_leak_current::~HH_sga_leak_current() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 void
 HH_sga_leak_current::() const
 {
 }
 */

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */