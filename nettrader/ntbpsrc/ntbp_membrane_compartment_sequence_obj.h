/**\file ntbp_membrane_compartment_sequence_obj.h - NTBP_membrane_compartment_sequence_o class header 
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
  

/* $Id: ntbp_membrane_compartment_sequence_obj.h,v 1.2 2005/04/25 13:55:27 face Exp $ 
* $Log: ntbp_membrane_compartment_sequence_obj.h,v $
* Revision 1.2  2005/04/25 13:55:27  face
* *** empty log message ***
*
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.5  2003/08/24 10:43:01  face
* *** empty log message ***
*
* Revision 1.4  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.3  2002/03/08 15:13:26  face
* *** empty log message ***
*
* Revision 1.2  2001/10/12 09:18:36  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_membrane_compartment_sequence_obj_h_ 
#define _ntbp_membrane_compartment_sequence_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* Parent includes */
#include "ntbp_membrane_obj.h" 
/* NT includes */
#include "ntbp_tridiag_solver_obj.h" 
#include "ntbp_cylindrical_compartment_obj.h" 
#include "nt_error_obj.h"
/* other includes */
#include <fstream>
#include <vector>
#include <tnt/cmat.h>
/** @short NTBP_membrane_compartment_sequence_o class 
\bug unknown
\warning unknown 
*/


class NTBP_membrane_compartment_sequence_o : public NTBP_membrane_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
// see .cpp comments : NTBP_membrane_compartment_sequence_o();
// RESTRUCTURING GOING ON HERE
//transforming everything in  here into AddCompartment(NTBP_com..._o * compPtr) to allow
//polymorphic usage of different compartments
//
NTBP_membrane_compartment_sequence_o();
NTBP_membrane_compartment_sequence_o(const NTBP_membrane_compartment_sequence_o & original);
const NTBP_membrane_compartment_sequence_o & operator= (const NTBP_membrane_compartment_sequence_o & right);
virtual ~NTBP_membrane_compartment_sequence_o();
/* ***  Methods              ***/  
NTreturn PushBack(NTBP_cylindrical_compartment_o * compartPtr );
NTreturn Init();
NTreturn InitialStep();
NTreturn Step();
void ShowVoltage() 
{ 
	cerr << "Voltage [";
	for (NTsize ll =0; ll < _numCompartments(); ll++) {
		cout << compartmentVec[ll]->_vM() << "\t";
		cerr << compartmentVec[ll]->_vM() << " ";
	}
	cerr << "]"<< endl;
}
NTreal MembraneVoltage(NTsize compartmentId /* 1..numCompartments*/){ return compartmentVec[compartmentId-1]->_vM(); }
NTreturn InjectCurrent(NTreal current /* in nA */, NTsize compartmentId);
NTsize _numCompartments() const { return numCompartments; }
/**  */
/**  */
NTreal CompartmentSequenceChannelStateTimeConstant() const;
void ShowHinesMatrix();
NTreal AttachedCurrent(NTsize compIndex, NTsize currIndex){ NT_ASSERT(compIndex > 0); return compartmentVec[compIndex-1]->AttachedCurrent(currIndex);}
vector <NTreal> OpenChannels(NTsize currIndex) const;
vector <NTreal> OpenChannelsRatio(NTsize currIndex) const;
vector <NTreal> NumChannels(NTsize currIndex) const;
vector <NTreal> _vVec() const;
NTreturn WriteMembranePotential( ofstream & file );
NTreturn WriteMembranePotentialASCII( ofstream & file, NTreal timeVar );
NTreturn WriteCurrent( ofstream & file, NTsize currentIndex /* 1..numCurrents in compartment */);
NTreturn WriteCurrentAscii( ofstream & file, NTsize currentIndex /* 1..numCurrents in compartment */);
NTreturn WriteOpenChannelsRatio( ofstream & file, NTsize currentIndex /* 1..numCurrents in compartment */);
NTreturn WriteATP(ofstream & file);
/**  */
bool GillespieStep();
  /**  */
vector <NTreal> GiveCurrent(NTsize index);
/* ***  Data                 ***/  
vector < NTBP_cylindrical_compartment_o* > compartmentVec;

protected:
/* ***  Methods              ***/
/** CRAP not working */
vector <NTreal> ZadorPearlmutterSolveTriDiag(vector <NTreal> lNewVec, vector <NTreal> dNewVec, vector <NTreal>uNewVec, vector <NTreal>rNewVec) const;
/** CRAP not working */
vector <NTreal> MascagniSolveTriDiag(vector <NTreal> lNewVec, vector <NTreal>dNewVec, vector <NTreal>uNewVec, vector <NTreal>rNewVec) const;
/** WORKING ! */
vector <NTreal> NumericalRecipesSolveTriDiag(const vector <NTreal> & l, const vector <NTreal> & d, const vector <NTreal> & u, const vector <NTreal> & r) const;
/* ***  Data                 ***/

private:
/* ***  Methods              ***/  
/* ***  Data                 ***/
vector <NTreal> lVec;
vector <NTreal> dVec;
vector <NTreal> uVec;

//This should be in each compartment
//vector <NTreal> vVec;
vector <NTreal> rVec;
NTsize numCompartments;
bool initialised;
bool swCrankNicholson;
};



#endif /* _ntbp_membrane_compartment_sequence_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
