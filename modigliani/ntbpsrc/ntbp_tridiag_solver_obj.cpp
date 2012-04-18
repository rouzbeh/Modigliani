/**\file ntbp_tridiag_solver_obj.cpp - NTBP_tridiag_solver_o class implementation 
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

/* $Id: ntbp_tridiag_solver_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: ntbp_tridiag_solver_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.1  2001/10/03 14:37:52  face
 * *** empty log message ***
 *

 */
#include "ntbp_tridiag_solver_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_tridiag_solver_o */
NTBP_tridiag_solver_o::NTBP_tridiag_solver_o(NTsize numNewCompartments) {
	NT_ASSERT(numNewCompartments > 0);
	numCompartments = numNewCompartments;
	initialised = false;
	cerr << "OBSOLETE code called";
}

/* ***      COPY AND ASSIGNMENT	***/
NTBP_tridiag_solver_o::NTBP_tridiag_solver_o(
		const NTBP_tridiag_solver_o __attribute__((unused)) & original) :
		NT_o() {
	// add assignment code here
	cerr << "OBSOLETE code called";
}

const NTBP_tridiag_solver_o&
NTBP_tridiag_solver_o::operator=(const NTBP_tridiag_solver_o & right) {
	if (this == &right)
		return (*this); // Gracefully handle self assignment
	// add assignment code here
	cerr << "OBSOLETE code called";
	return (*this);
}

/* ***      DESTRUCTOR		***/
NTBP_tridiag_solver_o::~NTBP_tridiag_solver_o() {
	cerr << "OBSOLETE code called";
}

/* ***  PUBLIC                                    ***   */
/*
 NTreturn
 NTBP_tridiag_solver_o::Setup(vector <NTreal> newLVec, vector <NTreal> newUVec)
 {
 cerr << "OBSOLETE code called";
 NT_ASSERT(newLVec.size() == numCompartments);
 lOrigVec = newLVec;

 NT_ASSERT(newUVec.size() == numCompartments);
 uOrigVec = newUVec;

 initialised = true;
 return (NT_SUCCESS);
 }
 */

/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
vector<NTreal> NTBP_tridiag_solver_o::Solve(vector<NTreal> dVec,
		vector<NTreal> vVec, vector<NTreal> rVec) {
	cerr << "OBSOLETE code called";
	vector<NTreal> lVec;
	vector<NTreal> uVec;
	NT_ASSERT( true == initialised);
	NT_ASSERT(lVec.size() == numCompartments);
	NT_ASSERT(dVec.size() == numCompartments);
	NT_ASSERT(uVec.size() == numCompartments);
	NT_ASSERT(vVec.size() == numCompartments);
	NT_ASSERT(rVec.size() == numCompartments);
	NTsize m = numCompartments;
	/* forward elimination */
	//dVec[0] = dVec[0];
	uVec[0] = uVec[0] / dVec[0];
	rVec[0] = rVec[0] / dVec[0];
	NTsize i = 1;
	for (i = 1; i < m; i++) {
		dVec[i] = dVec[i] - lVec[i] * vVec[i - 1];
		rVec[i] = (rVec[i] - lVec[i] * vVec[i - 1]) / dVec[i];
		uVec[i] = uVec[i] / dVec[i];
	}

	//	dVec[m-1] -= lVec[m-1] * vVec[m-2];
	//rVec[m-1] = (rVec[m-1] - lVec[m-1] * vVec[m-2])/dVec[m-1];

	/* backward substitution */
	vVec[m - 1] = rVec[m - 1];
	for (i = m - 2; (i < m - 1); i--) {
		vVec[i] = rVec[i] - uVec[i] * vVec[i + 1];
	}

	return (vVec);
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
/**  */
NTreturn NTBP_tridiag_solver_o::Resize(NTsize numNewCompartments) {
	NT_ASSERT(numNewCompartments > 0);
	numCompartments = numNewCompartments;
	return (NT_SUCCESS);
}
