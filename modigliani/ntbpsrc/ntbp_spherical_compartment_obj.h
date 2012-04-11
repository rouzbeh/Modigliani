/**\file ntbp_spherical_compartment_obj.h - NTBP_spherical_compartment_o class header 
 * by Ahmed Aldo Faisal &copy; created 19.3.2001  
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
  

/* $Id: ntbp_spherical_compartment_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_spherical_compartment_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.3  2003/06/20 16:15:05  face
* *** empty log message ***
*
* Revision 1.2  2003/04/08 15:13:36  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_spherical_compartment_obj_h_ 
#define _ntbp_spherical_compartment_obj_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* Parent includes */
#include "ntbp_membrane_compartment_obj.h" 
/* NT includes */

/* other includes */
#include <cmath>
//#include <math.h>

/** @short NTBP_spherical_compartment_o class 
\bug unknown
\warning unknown 
*/
class NTBP_spherical_compartment_o : public NTBP_membrane_compartment_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_spherical_compartment_o(NTreal radius /* muMeter */, NTreal cM /* muFarad/cm^2 */);
NTBP_spherical_compartment_o(const NTBP_spherical_compartment_o & original);
const NTBP_spherical_compartment_o & operator= (const NTBP_spherical_compartment_o & right);
virtual ~NTBP_spherical_compartment_o();
/* ***  Methods              ***/  
NTreal _radius() const { return radius;}
/* ***  Data                 ***/  
protected:
/* ***  Methods              ***/  
/* ***  Data                 ***/  
private:
/* ***  Methods              ***/  
/* ***  Data                 ***/ 
NTreal radius; // in 
};
#endif /* _ntbp_spherical_compartment_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
