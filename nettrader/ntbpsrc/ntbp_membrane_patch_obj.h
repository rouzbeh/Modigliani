/**\file ntbp_membrane_patch_obj.h - NTBP_membrane_patch_o class header 
 * by Ahmed Aldo Faisal &copy; created 21.1.2002  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000,2001,2002 Ahmed Aldo Faisal
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
  

/* $Id: ntbp_membrane_patch_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_membrane_patch_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2003/01/30 17:13:43  face
* *** empty log message ***
*

*/
#ifndef _ntbp_membrane_patch_obj_h_ 
#define _ntbp_membrane_patch_obj_h_ 

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* NT includes */
#include "ntbp_membrane_compartment_obj.h"
#include "ntbp_object_obj.h"
/* other includes */

/** @short NTBP_membrane_patch_o class 
\bug unknown
\warning unknown 
*/
class NTBP_membrane_patch_o : public NTBP_membrane_compartment_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_membrane_patch_o(NTreal area /* [muM^2] */, NTreal newCM /* muF/cm^2 */);
NTBP_membrane_patch_o(const NTBP_membrane_patch_o & original);
const NTBP_membrane_patch_o & operator= (const NTBP_membrane_patch_o & right);
virtual ~NTBP_membrane_patch_o();
/* ***  Methods              ***/
NTreal MembranePotential() const {return _vM();}
/** No descriptions */
NTreturn Step();
/** No descriptions */
NTreturn InitialStep();
/** Current [nA]  */
NTreal MembraneCurrent(NTsize currentIndex) const {
						NT_ASSERT((currentIndex > 0) && (currentIndex-1 < currentVec.size()));
						return currentVec[currentIndex-1]->_current();}
/* in muMeter^2 */

/* ***  Data                 ***/
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/ 
};
#endif /* _ntbp_membrane_patch_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
