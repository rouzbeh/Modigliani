/**\file Custom_cylindrical_compartment.cpp - Custom_cylindrical_compartment class implementation
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

#include "Custom_cylindrical_compartment.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Custom_cylindrical_compartment */
Custom_cylindrical_compartment::Custom_cylindrical_compartment(
		mbase::Mreal newLength, mbase::Mreal newDiameter, mbase::Mreal newCm, mbase::Mreal newRa,
		mbase::Mreal newTemperature) :
		Cylindrical_compartment(newLength, newDiameter, newTemperature), voltage(
				0) {
	Set_cM(newCm);
	Set_rA(newRa);
}

/* ***      COPY AND ASSIGNMENT	***/
Custom_cylindrical_compartment::Custom_cylindrical_compartment(
		const Custom_cylindrical_compartment & original) :
		Cylindrical_compartment(original._length(), original._diameter()) {
	// add assignment code here
}

const Custom_cylindrical_compartment&
Custom_cylindrical_compartment::operator=(
		const Custom_cylindrical_compartment & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	return (*this);
}

/* ***      DESTRUCTOR		***/
Custom_cylindrical_compartment::~Custom_cylindrical_compartment() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown

 void
 Custom_cylindrical_compartment::() const
 {
 }
 */

mbase::Mreal Custom_cylindrical_compartment::getVoltage() {
	return (voltage);
}

void Custom_cylindrical_compartment::setVoltage(mbase::Mreal newVoltage) {
	voltage = newVoltage;
}
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
