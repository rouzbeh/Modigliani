/**
 * @file Membrane_patch.c++
 * Membrane_patch class implementation
 * @author Ahmed Aldo Faisal &copy; created 21.1.2002  
 * NetTrader - visualisation, scientific and financial analysis and simulation system
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

#include "Membrane_patch.h"

using namespace mcore;

/* ***      CONSTRUCTORS	***/
/** Create a Membrane_patch */
Membrane_patch::Membrane_patch(modigliani_base::Real newArea /* [muM^2] */,
		modigliani_base::Real newCM /* muF/cm^2 */) :
		Membrane_compartment(newArea) {
	SetCM(newCM);
}

/* ***      DESTRUCTOR		***/
Membrane_patch::~Membrane_patch() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown

 void
 Membrane_patch::() const
 {
 }
 */

/** No descriptions */
/** @short
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
inline modigliani_base::ReturnEnum Membrane_patch::Step() {
	/*       [mV] == [10^-3 ms nA /muF] */
	modigliani_base::Real deltaV = 1e-3 /* mV/muV */* _timeStep()
			* CompartmentMembraneNetCurrent()
			/ CompartmentMembraneCapacitance();
	set_vm(vm() + deltaV);
	Membrane_compartment::step(vm());
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/** No descriptions */
/** @short Initial step, needed if time staggering of the differential equations solution is desired.
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
modigliani_base::ReturnEnum Membrane_patch::InitialStep() {
	update_timeStep(_timeStep() / 2.0);
	StepNTBP();
	Step();

	update_timeStep(_timeStep() * 2.0);
	StepNTBP();
	std::cerr
			<< "NTBP_membrane_compartment_sequence_o::InitialStep() - ERROR : not correctly implemented ? untested."
			<< std::endl;
	return (modigliani_base::ReturnEnum::SUCCESS);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
