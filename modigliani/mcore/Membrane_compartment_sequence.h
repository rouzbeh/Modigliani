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

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"
/* Parent includes */
#include "Membrane.h"
/* NT includes */
#include "Tridiag_solver.h"
#include "Cylindrical_compartment.h"
#include "ntsrc/nt_error_obj.h"
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
	NTreturn PushBack(Cylindrical_compartment * compartPtr);
	NTreturn Init();
	NTreturn InitialStep();
	NTreturn step();
	void ShowVoltage() {
		cerr << "Voltage [";
		for (NTsize ll = 0; ll < _numCompartments(); ll++) {
			cout << compartmentVec[ll]->_vM() << "\t";
			cerr << compartmentVec[ll]->_vM() << " ";
		}
		cerr << "]" << endl;
	}
	NTreal MembraneVoltage(NTsize compartmentId /* 1..numCompartments*/) {
		return (compartmentVec[compartmentId - 1]->_vM());
	}
	NTreturn InjectCurrent(NTreal current /* in nA */, NTsize compartmentId);
	NTsize _numCompartments() const {
		return (numCompartments);
	}
	/**  */
	/**  */
	NTreal CompartmentSequenceChannelStateTimeConstant() const;
	void ShowHinesMatrix();
	NTreal AttachedCurrent(NTsize compIndex, NTsize currIndex) {
		NT_ASSERT(compIndex > 0);
		return (compartmentVec[compIndex - 1]->AttachedCurrent(currIndex));
	}
	vector<NTreal> open_channels(NTsize currIndex) const;
	vector<NTreal> OpenChannelsRatio(NTsize currIndex) const;
	vector<NTreal> NumChannels(NTsize currIndex) const;
	vector<NTreal> NumChannelsInState(NTsize currIndex, NTsize state) const;
	vector<NTreal> _vVec() const;
	NTreturn WriteMembranePotential(ostream & file) const;
	NTreturn WriteCompartmentData(ostream* file, NTsize to_print) const;
	NTreturn WriteCurrent(ostream & file,
			NTsize currentIndex /* 1..numCurrents in compartment */) const;
	NTreturn WriteCurrent(ostream & file,
			NTsize currentIndex /* 1..numCurrents in compartment */,
			vector<NTsize> to_print) const;
	NTreturn WriteMembranePotentialASCII(ostream & file) const;
	NTreturn WriteCurrentAscii(ostream & file,
			NTsize currentIndex /* 1..numCurrents in compartment */) const;
	Cylindrical_compartment* ReturnCompartmentVec(NTsize index);
	/**  */
	bool GillespieStep();
	/**  */
	vector<NTreal> GiveCurrent(NTsize index);
	/* ***  Data                 ***/
	vector<Cylindrical_compartment*> compartmentVec;

protected:
	/* ***  Methods              ***/
	/** CRAP not working */
	vector<NTreal> ZadorPearlmutterSolveTriDiag(vector<NTreal> lNewVec,
			vector<NTreal> dNewVec, vector<NTreal> uNewVec,
			vector<NTreal> rNewVec) const;
	/** CRAP not working */
	vector<NTreal> MascagniSolveTriDiag(vector<NTreal> lNewVec,
			vector<NTreal> dNewVec, vector<NTreal> uNewVec,
			vector<NTreal> rNewVec) const;
	/** WORKING ! */
	vector<NTreal> NumericalRecipesSolveTriDiag(const vector<NTreal> & l,
			const vector<NTreal> & d, const vector<NTreal> & u,
			const vector<NTreal> & r) const;
	/* ***  Data                 ***/

private:
	/* ***  Methods              ***/
	/* ***  Data                 ***/
	vector<NTreal> lVec;
	vector<NTreal> dVec;
	vector<NTreal> uVec;

//This should be in each compartment
//vector <NTreal> vVec;
	vector<NTreal> rVec;
	NTsize numCompartments;
	bool initialised;
	bool swCrankNicholson;
};
}
#endif /* _ntbp_membrane_compartment_sequence.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
