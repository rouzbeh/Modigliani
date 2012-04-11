/**\file ntbp_tridiag_solver_obj.h - NTBP_tridiag_solver_o class header 
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
  

/* $Id: ntbp_tridiag_solver_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_tridiag_solver_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_tridiag_solver_obj_h_ 
#define _ntbp_tridiag_solver_obj_h_ 

/* NT core includes */
#include "../ntsrc/nt_main.h" 
#include "../ntsrc/nt_types.h" 
#include "../ntsrc/nt_obj.h"  
/* NT includes */
#include "../ntsrc/nt_error_obj.h"

/* other includes */
#include <vector>

/** @short NTBP_tridiag_solver_o class 
 Solve tridiangonal sequence of multi-compartment cable equations
 using the Thomas algorithm
cerr << "OBSOLETE code called";

OBSOLETE CODE

\bug unknown
\warning unknown 
*/
class NTBP_tridiag_solver_o : public NT_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_tridiag_solver_o(NTsize numCompartments);
NTBP_tridiag_solver_o(const NTBP_tridiag_solver_o & original);
const NTBP_tridiag_solver_o & operator= (const NTBP_tridiag_solver_o & right);
virtual ~NTBP_tridiag_solver_o();
/* ***  Methods              ***/  
NTreturn Setup(vector <NTreal> newLVec, vector <NTreal> newUVec);
vector <NTreal> Solve( vector <NTreal> dVec,  vector <NTreal> vVec, vector <NTreal> rVec);
  /**  */
NTreturn Resize(NTsize numCompartment);
// vector <NTreal> Solve(vector <NTreal> dVec, vector <NTreal> vVec, vector <NTreal> rVec);
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  

private:
/* ***  Methods              ***/  
NTsize numCompartments;
bool initialised;
/* ***  Data                 ***/ 
};
#endif /* _ntbp_tridiag_solver_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
