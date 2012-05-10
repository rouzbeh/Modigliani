/**\file Membrane_compartment_sequence.h - Membrane_compartment_sequence class header
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

#ifndef _mcore_membrane_compartment_sequence_h_
#define _mcore_membrane_compartment_sequence_h_

/* Parent includes */
#include "Membrane.h"
/* NT includes */
#include "Tridiag_solver.h"
#include "Cylindrical_compartment.h"
/* other includes */
#include <fstream>
#include <vector>
#include <tnt/tnt_cmat.h>
/** @short Membrane_compartment_sequence class
 \bug unknown
 \warning unknown
 */
namespace mcore {
class Membrane_compartment_sequence: public Membrane {
public:
	/***   Constructors, Copy/Assignment and Destructor  ***/
// see .cpp comments : Membrane_compartment_sequence();
// RESTRUCTURING GOING ON HERE
//transforming everything in  here into AddCompartment(NTBP_com..._o * compPtr) to allow
//polymorphic usage of different compartments
//
	Membrane_compartment_sequence();
	Membrane_compartment_sequence(
			const Membrane_compartment_sequence & original);
	const Membrane_compartment_sequence & operator=(
			const Membrane_compartment_sequence & right);
	virtual ~Membrane_compartment_sequence();
	/* ***  Methods              ***/
	mbase::Mreturn PushBack(Cylindrical_compartment * compartPtr);
	mbase::Mreturn Init();
	mbase::Mreturn InitialStep();
	mbase::Mreturn step();
	void ShowVoltage() {
		std::cerr << "Voltage [";
		for (mbase::Msize ll = 0; ll < _numCompartments(); ll++) {
			std::cout << compartmentVec[ll]->_vM() << "\t";
			std::cerr << compartmentVec[ll]->_vM() << " ";
		}
		std::cerr << "]" << std::endl;
	}
	mbase::Mreal MembraneVoltage(mbase::Msize compartmentId /* 1..numCompartments*/) {
		return (compartmentVec[compartmentId - 1]->_vM());
	}
	mbase::Mreturn InjectCurrent(mbase::Mreal current /* in nA */, mbase::Msize compartmentId);
	mbase::Msize _numCompartments() const {
		return (numCompartments);
	}
	/**  */
	/**  */
	mbase::Mreal CompartmentSequenceChannelStateTimeConstant() const;
	void ShowHinesMatrix();
	mbase::Mreal AttachedCurrent(mbase::Msize compIndex, mbase::Msize currIndex) {
		M_ASSERT(compIndex > 0);
		return (compartmentVec[compIndex - 1]->AttachedCurrent(currIndex));
	}
	std::vector<mbase::Mreal> open_channels(mbase::Msize currIndex) const;
	std::vector<mbase::Mreal> OpenChannelsRatio(mbase::Msize currIndex) const;
	std::vector<mbase::Mreal> NumChannels(mbase::Msize currIndex) const;
	std::vector<mbase::Mreal> NumChannelsInState(mbase::Msize currIndex, mbase::Msize state) const;
	std::vector<mbase::Mreal> _vVec() const;
	mbase::Mreturn WriteMembranePotential(std::ostream & file) const;
	mbase::Mreturn WriteCompartmentData(std::ostream* file, mbase::Msize to_print) const;
	mbase::Mreturn WriteCurrent(std::ostream & file,
			mbase::Msize currentIndex /* 1..numCurrents in compartment */) const;
	mbase::Mreturn WriteCurrent(std::ostream & file,
			mbase::Msize currentIndex /* 1..numCurrents in compartment */,
			std::vector<mbase::Msize> to_print) const;
	mbase::Mreturn WriteMembranePotentialASCII(std::ostream & file) const;
	mbase::Mreturn WriteCurrentAscii(std::ostream & file,
			mbase::Msize currentIndex /* 1..numCurrents in compartment */) const;
	Cylindrical_compartment* ReturnCompartmentVec(mbase::Msize index);
	/**  */
	bool GillespieStep();
	/**  */
	std::vector<mbase::Mreal> GiveCurrent(mbase::Msize index);
	/* ***  Data                 ***/
	std::vector<Cylindrical_compartment*> compartmentVec;

protected:
	/* ***  Methods              ***/
	/** CRAP not working */
	std::vector<mbase::Mreal> ZadorPearlmutterSolveTriDiag(std::vector<mbase::Mreal> lNewVec,
			std::vector<mbase::Mreal> dNewVec, std::vector<mbase::Mreal> uNewVec,
			std::vector<mbase::Mreal> rNewVec) const;
	/** CRAP not working */
	std::vector<mbase::Mreal> MascagniSolveTriDiag(std::vector<mbase::Mreal> lNewVec,
			std::vector<mbase::Mreal> dNewVec, std::vector<mbase::Mreal> uNewVec,
			std::vector<mbase::Mreal> rNewVec) const;
	/** WORKING ! */
	std::vector<mbase::Mreal> NumericalRecipesSolveTriDiag(const std::vector<mbase::Mreal> & l,
			const std::vector<mbase::Mreal> & d, const std::vector<mbase::Mreal> & u,
			const std::vector<mbase::Mreal> & r) const;
	/* ***  Data                 ***/

private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	std::vector<mbase::Mreal> lVec;
	std::vector<mbase::Mreal> dVec;
	std::vector<mbase::Mreal> uVec;

//This should be in each compartment
//std::vector <mbase::Mreal> vVec;
	std::vector<mbase::Mreal> rVec;
	mbase::Msize numCompartments;
	bool initialised;
	bool swCrankNicholson;
};
}
#endif /* _ntbp_membrane_compartment_sequence.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
