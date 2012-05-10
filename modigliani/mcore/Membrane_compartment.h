/**\file Membrane_compartment.h - Membrane_compartment_o class header
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
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mcore_membrane_compartment_h_
#define _mcore_membrane_compartment_h_

#include "Object.h"
#include "Membrane_current.h"
/* other includes */
#include <vector>

/** @short NTBP_membrane_compartment_o class acts as a "container + glue" for the various membrane_current
 classes that can be attached to it.



 \bug unknown
 \warning 1. rA, rM and cM are set to (arbitrary) standard values, overwrite in derived class
 2. leak current IS incorported into the COMPARTMENT VECTOR
 */
namespace mcore {
class Membrane_compartment: public Object {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
	Membrane_compartment(mbase::Mreal area /* in muMeter^2 */, mbase::Mreal newTemperature =
			6.3);
	Membrane_compartment(const Membrane_compartment & original);
	const Membrane_compartment & operator=(const Membrane_compartment & right);
	virtual ~Membrane_compartment();
	/* ***  Methods              ***/
	mbase::Mreturn AttachCurrent(Membrane_current * currentPtr,
			NTBPcurrentType type);
	mbase::Mreturn step(mbase::Mreal newVM /* mV */);
	mbase::Mreturn InjectCurrent(mbase::Mreal current /* in nA */);
	mbase::Mreal AttachedCurrent(mbase::Msize currentIndex) {
		M_ASSERT((currentIndex > 0) && (currentIndex - 1 < currentVec.size()));
		return (currentVec[currentIndex - 1]->_current());
	}
	mbase::Mreal AttachedConductance(mbase::Msize currentIndex) {
		M_ASSERT((currentIndex > 0) && (currentIndex - 1 < currentVec.size()));
		return (currentVec[currentIndex - 1]->_conductance());
	}
	mbase::Mreal AttachedReversalPotential(mbase::Msize currentIndex) {
		M_ASSERT((currentIndex > 0) && (currentIndex - 1 < currentVec.size()));
		return (currentVec[currentIndex - 1]->_reversalPotential());
	}
	/**  membrane time constant at instaneous membrane conductivity in ms */
	mbase::Mreal TimeConstant() {
		return ((_cM() / total_conductance()) * _area() * 1.0e8);
	}
	const Membrane_current * Current(mbase::Msize currentIndex) {
		M_ASSERT((currentIndex > 0) && (currentIndex - 1 < currentVec.size()));
		return (currentVec[currentIndex - 1]);
	}
	/* in muMeter^2 */
	mbase::Mreal _area() /* in muMeter^2 */const {
		return (area);
	}
	/* in mV */
	mbase::Mreal _vM() /* in mV muMeter */const {
		return (vM);
	}
	void Set_vM(mbase::Mreal newVoltage) {
		vM = newVoltage;
	}
	/** Set specific membrane capacitance in muF / cm^2 */
	void Set_cM(mbase::Mreal newCm /* in muF / cm^2 */) {
		cM = newCm;
		compartmentMembraneCapacitance = CompartmentMembraneCapacitance();
	}
	/* in muF / cm^2 */
	mbase::Mreal _cM() const {
		return (cM);
	}
	/** Set specific axoplasmic resistivity in Ohm cm */
	void Set_rA(mbase::Mreal newRa /* in Ohm cm */) {
		rA = newRa;
	}
	/* in Ohm cm */
	mbase::Mreal _rA() const {
		return (rA);
	}
	/* Set temperature [Celsius] in compartment and for all currents within compartment (affects future attached ones also) */
	mbase::Mreturn Set_temperature(mbase::Mreal newTemp /* in Celsius */) {
		temperature = newTemp;
		for (mbase::Msize i = 0; i < currentVec.size(); i++)
			currentVec[i]->Set_temperature(newTemp);
		return (mbase::M_SUCCESS);
	}
	/* in Celsius */
	mbase::Mreal _temperature() const {
		return (temperature);
	}
	/* in muF */
	mbase::Mreal _compartmentMembraneCapacitance() const {
		return (compartmentMembraneCapacitance);
	}
	/* in muF */
	mbase::Mreal CompartmentMembraneCapacitance() const;
	/* in nA */
	mbase::Mreal CompartmentMembraneNetCurrent() const;
	/** in 1/mSec or 1 kHz*/
	mbase::Mreal CompartmentChannelStateTimeConstant() const;
	/**  */
	bool GillespieStep();
	/**  */
	void ShowParam() const;
	/* ***  Data                 ***/
	std::vector<Membrane_current *> currentVec;

	std::vector<Membrane_current *> ReturnCurrentVec(); //TODO: added

protected:
	/* ***  Methods              ***/
	mbase::Mreal total_conductance() const;
	mbase::Mreal WeightedConductance() const; // OBSOLETE?
	/* ***  Data                 ***/
	mbase::Mreal vM; // membrane voltage in mV
	mbase::Mreal iInj; // injected current into compartment in nA

private:
	/* ***  Methods              ***/
	mbase::Mreal cM; // membrane capacity in muFarad/cm^2
	mbase::Mreal rA; // axoplasmatic resistance in Ohm cm
	mbase::Mreal area; // in muMeter^2
	mbase::Mreal temperature; // in Celsius
	mbase::Mreal compartmentMembraneCapacitance; // in muFarad
	/* ***  Data                 ***/
};
}
#endif /* _mcore_membrane_compartment.h_ */

/* File skeleton generated by GenNTObj version 0.7. */

