/**
 * @file Membrane_compartment_sequence.h
 * Membrane_compartment_sequence class header
 * @author Ahmed Aldo Faisal &copy; created 26.3.2001
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

#ifndef _mcore_membrane_compartment_sequence_h_
#define _mcore_membrane_compartment_sequence_h_

/* Parent includes */
#include "Membrane.h"
/* NT includes */
#include "Cylindrical_compartment.h"
/* other includes */
#include <fstream>
#include <vector>
#include <tnt/tnt_cmat.h>

namespace mcore {
/** @short Membrane_compartment_sequence class
 \bug unknown
 \warning unknown
 */
class Membrane_compartment_sequence: public Membrane {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
// see .cpp comments : Membrane_compartment_sequence();
// RESTRUCTURING GOING ON HERE
//transforming everything in  here into AddCompartment(NTBP_com..._o * compPtr) to allow
//polymorphic usage of different compartments
//
	Membrane_compartment_sequence();
	Membrane_compartment_sequence(const Membrane_compartment_sequence & original) = delete;
	const Membrane_compartment_sequence & operator=(
			const Membrane_compartment_sequence & right) = delete;
	virtual ~Membrane_compartment_sequence();
	/* ***  Methods              ***/
	mbase::Mreturn PushBack(Cylindrical_compartment * compartPtr);
	mbase::Mreturn Init();
	mbase::Mreturn InitialStep();
	mbase::Mreturn step();
	void ShowVoltage() {
		std::cerr << "Voltage [";
		for (mbase::Size_t ll = 0; ll < _numCompartments(); ll++) {
			std::cout << compartmentVec[ll]->_vM() << "\t";
			std::cerr << compartmentVec[ll]->_vM() << " ";
		}
		std::cerr << "]" << std::endl;
	}
	mbase::Real MembraneVoltage(mbase::Size_t compartmentId /* 1..numCompartments*/) {
		return (compartmentVec[compartmentId - 1]->_vM());
	}
	mbase::Mreturn InjectCurrent(mbase::Real current /* in nA */, mbase::Size_t compartmentId);
	mbase::Size_t _numCompartments() const {
		return (numCompartments);
	}
	/**  */
	/**  */
	mbase::Real CompartmentSequenceChannelStateTimeConstant() const;
	void ShowHinesMatrix();
	mbase::Real AttachedCurrent(mbase::Size_t compIndex, mbase::Size_t currIndex) {
		M_ASSERT(compIndex > 0);
		return (compartmentVec[compIndex - 1]->AttachedCurrent(currIndex));
	}
	std::vector<mbase::Real> open_channels(mbase::Size_t currIndex) const;
	std::vector<mbase::Real> OpenChannelsRatio(mbase::Size_t currIndex) const;
	std::vector<mbase::Real> NumChannels(mbase::Size_t currIndex) const;
	std::vector<mbase::Real> NumChannelsInState(mbase::Size_t currIndex, mbase::Size_t state) const;
	std::vector<mbase::Real> _vVec() const;
	mbase::Mreturn WriteMembranePotential(std::ostream & file) const;
	mbase::Mreturn WriteCompartmentData(std::ostream* file, mbase::Size_t to_print) const;
	mbase::Mreturn WriteCurrent(std::ostream & file,
			mbase::Size_t currentIndex /* 1..numCurrents in compartment */) const;
	mbase::Mreturn WriteCurrent(std::ostream & file,
			mbase::Size_t currentIndex /* 1..numCurrents in compartment */,
			std::vector<mbase::Size_t> to_print) const;
	mbase::Mreturn WriteMembranePotentialASCII(std::ostream & file) const;
	mbase::Mreturn WriteCurrentAscii(std::ostream & file,
			mbase::Size_t currentIndex /* 1..numCurrents in compartment */) const;
	Cylindrical_compartment* ReturnCompartmentVec(mbase::Size_t index);
	/**  */
	bool GillespieStep();
	/**  */
	std::vector<mbase::Real> GiveCurrent(mbase::Size_t index);
	/* ***  Data                 ***/
	std::vector<Cylindrical_compartment*> compartmentVec;

protected:
	/* ***  Methods              ***/
	/** CRAP not working */
	std::vector<mbase::Real> ZadorPearlmutterSolveTriDiag(std::vector<mbase::Real> lNewVec,
			std::vector<mbase::Real> dNewVec, std::vector<mbase::Real> uNewVec,
			std::vector<mbase::Real> rNewVec) const;
	/** CRAP not working */
	std::vector<mbase::Real> MascagniSolveTriDiag(std::vector<mbase::Real> lNewVec,
			std::vector<mbase::Real> dNewVec, std::vector<mbase::Real> uNewVec,
			std::vector<mbase::Real> rNewVec) const;
	/** WORKING ! */
	std::vector<mbase::Real> NumericalRecipesSolveTriDiag(const std::vector<mbase::Real> & l,
			const std::vector<mbase::Real> & d, const std::vector<mbase::Real> & u,
			const std::vector<mbase::Real> & r) const;
	/* ***  Data                 ***/

private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	std::vector<mbase::Real> lVec;
	std::vector<mbase::Real> dVec;
	std::vector<mbase::Real> uVec;

//This should be in each compartment
//std::vector <mbase::Real> vVec;
	std::vector<mbase::Real> rVec;
	mbase::Size_t numCompartments;
	bool initialised;
	bool swCrankNicholson;
};
}
#endif /* _ntbp_membrane_compartment_sequence.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
