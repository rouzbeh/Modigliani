/**\file ntbp_tridiag_solver.h - NTBP_tridiag_solver_o class header 
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
  

/* $Id: ntbp_tridiag_solver.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_tridiag_solver.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _mcore_tridiag_solver_h_
#define _mcore_tridiag_solver_h_

/* NT includes */
#include "Object.h"

/* other includes */
#include <vector>

/** @short NTBP_tridiag_solver_o class 
 Solve tridiangonal sequence of multi-compartment cable equations
 using the Thomas algorithm
std::cerr << "OBSOLETE code called";

OBSOLETE CODE

\bug unknown
\warning unknown 
*/
class NTBP_tridiag_solver_o : public mbase::Obj {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_tridiag_solver_o(mbase::Size_t numCompartments);
NTBP_tridiag_solver_o(const NTBP_tridiag_solver_o & original);
const NTBP_tridiag_solver_o & operator= (const NTBP_tridiag_solver_o & right);
virtual ~NTBP_tridiag_solver_o();
/* ***  Methods              ***/  
mbase::Mreturn Setup(std::vector <mbase::Real> newLVec, std::vector <mbase::Real> newUVec);
std::vector <mbase::Real> Solve( std::vector <mbase::Real> dVec,  std::vector <mbase::Real> vVec, std::vector <mbase::Real> rVec);
  /**  */
mbase::Mreturn Resize(mbase::Size_t numCompartment);
// std::vector <mbase::Real> Solve(std::vector <mbase::Real> dVec, std::vector <mbase::Real> vVec, std::vector <mbase::Real> rVec);
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  

private:
/* ***  Methods              ***/  
mbase::Size_t numCompartments;
bool initialised;
/* ***  Data                 ***/ 
};
#endif /* _ntbp_tridiag_solver.h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
