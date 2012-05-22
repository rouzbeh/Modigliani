/**\file ntbp_cylindrical_compartment_obj.cpp - Cylindrical_compartment class implementation
 * by Ahmed Aldo Faisal &copy; created 26.3.2001  
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

#include "Cylindrical_compartment.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Cylindrical_compartment */
Cylindrical_compartment::Cylindrical_compartment(
		mbase::Real newLength /* in muMeter */, mbase::Real newDiameter /* in muMeter */, mbase::Real newTemperature)
:
			Membrane_compartment(M_PI * newDiameter * newLength /* in muMeter^2 */, newTemperature) {
	length = newLength;
	diameter = newDiameter;
}

/* ***      COPY AND ASSIGNMENT	***/
Cylindrical_compartment::Cylindrical_compartment(
		const Cylindrical_compartment & original) :
	Membrane_compartment(original._area()) {
	// add assignment code here
}

const Cylindrical_compartment&
Cylindrical_compartment::operator=(
		const Cylindrical_compartment & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Cylindrical_compartment::~Cylindrical_compartment() {
	for (auto it=currentVec.begin(); it!=currentVec.end(); ++it){
		delete *it;
	}
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 void
 Cylindrical_compartment::() const
 {
 }
 */

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
