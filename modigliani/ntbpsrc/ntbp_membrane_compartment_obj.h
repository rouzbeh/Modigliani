/**\file ntbp_membrane_compartment_obj.h - NTBP_membrane_compartment_o class header 
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
  
/* $Id: ntbp_membrane_compartment_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
* $Log: ntbp_membrane_compartment_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.7  2003/08/14 16:43:27  face
* *** empty log message ***
*
* Revision 1.6  2003/06/20 16:15:05  face
* *** empty log message ***
*
* Revision 1.5  2003/02/17 10:20:22  face
* *** empty log message ***
*
* Revision 1.4  2002/03/13 19:23:27  face
* *** empty log message ***
*
* Revision 1.3  2002/03/08 15:13:26  face
* *** empty log message ***
*
* Revision 1.2  2002/01/21 17:24:28  face
* *** empty log message ***
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#ifndef _ntbp_membrane_compartment_obj_h_ 
#define _ntbp_membrane_compartment_obj_h_ 

/* NT core includes */
#include "ntsrc/nt_main.h" 
#include "ntsrc/nt_types.h" 
#include "ntsrc/nt_obj.h"  
/* NT includes */
#include "ntbp_membrane_current_obj.h"
/* other includes */
#include <vector>

/** @short NTBP_membrane_compartment_o class acts as a "container + glue" for the various membrane_current
classes that can be attached to it.



\bug unknown
\warning 1. rA, rM and cM are set to (arbitrary) standard values, overwrite in derived class
			  2. leak current IS incorported into the COMPARTMENT VECTOR
*/
class NTBP_membrane_compartment_o : public NTBP_object_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_membrane_compartment_o(NTreal area /* in muMeter^2 */, NTreal newTemperature = 6.3);
NTBP_membrane_compartment_o(const NTBP_membrane_compartment_o & original);
const NTBP_membrane_compartment_o & operator= (const NTBP_membrane_compartment_o & right);
virtual ~NTBP_membrane_compartment_o();
/* ***  Methods              ***/
NTreturn AttachCurrent(NTBP_membrane_current_o * currentPtr, NTBPcurrentType type);
NTreturn step(NTreal newVM /* mV */ );
NTreturn InjectCurrent(NTreal current /* in nA */);
NTreal AttachedCurrent(NTsize currentIndex){NT_ASSERT((currentIndex > 0) && (currentIndex-1 < currentVec.size())); return (currentVec[currentIndex-1]->_current());}
NTreal AttachedConductance(NTsize currentIndex){NT_ASSERT((currentIndex > 0) && (currentIndex-1 < currentVec.size())); return (currentVec[currentIndex-1]->_conductance());}
NTreal AttachedReversalPotential(NTsize currentIndex){NT_ASSERT((currentIndex > 0) && (currentIndex-1 < currentVec.size())); return (currentVec[currentIndex-1]->_reversalPotential());}
/**  membrane time constant at instaneous membrane conductivity in ms */
NTreal TimeConstant(){ return ((_cM()/total_conductance())*_area()*1.0e8); }
const NTBP_membrane_current_o * Current(NTsize currentIndex){NT_ASSERT((currentIndex > 0) && (currentIndex-1 < currentVec.size())); return (currentVec[currentIndex-1]);}
/* in muMeter^2 */
NTreal _area()  /* in muMeter^2 */const { return (area); }
/* in mV */
NTreal _vM() /* in mV muMeter */ const { return (vM); }
void Set_vM(NTreal newVoltage) { vM = newVoltage;}
/** Set specific membrane capacitance in muF / cm^2 */
void Set_cM( NTreal newCm /* in muF / cm^2 */ ) { cM = newCm;  compartmentMembraneCapacitance = CompartmentMembraneCapacitance(); }
/* in muF / cm^2 */
NTreal _cM() const { return (cM); }
/** Set specific axoplasmic resistivity in Ohm cm */
void Set_rA( NTreal newRa /* in Ohm cm */ ) { rA = newRa;}
/* in Ohm cm */
NTreal _rA() const { return (rA); }
/* Set temperature [Celsius] in compartment and for all currents within compartment (affects future attached ones also) */
NTreturn Set_temperature(NTreal newTemp /* in Celsius */) { temperature = newTemp; for (NTsize i = 0; i < currentVec.size(); i++) currentVec[i]->Set_temperature(newTemp); return (NT_SUCCESS);}
/* in Celsius */
NTreal _temperature() const { return (temperature);}
/* in muF */
NTreal _compartmentMembraneCapacitance() const { return (compartmentMembraneCapacitance); }
/* in muF */
NTreal CompartmentMembraneCapacitance() const;
/* in nA */
NTreal CompartmentMembraneNetCurrent() const;
/** in 1/mSec or 1 kHz*/
NTreal CompartmentChannelStateTimeConstant() const;
/**  */
bool GillespieStep();
/**  */
void ShowParam() const;
/* ***  Data                 ***/
vector< NTBP_membrane_current_o * > currentVec;

vector< NTBP_membrane_current_o * > ReturnCurrentVec (); //TODO: added

protected:
/* ***  Methods              ***/
NTreal  total_conductance() const;
NTreal  WeightedConductance() const;// OBSOLETE?
/* ***  Data                 ***/
NTreal vM; // membrane voltage in mV
NTreal iInj; // injected current into compartment in nA

private:
/* ***  Methods              ***/
NTreal cM; // membrane capacity in muFarad/cm^2
NTreal rA; // axoplasmatic resistance in Ohm cm
NTreal area; // in muMeter^2
NTreal temperature; // in Celsius
NTreal compartmentMembraneCapacitance; // in muFarad
/* ***  Data                 ***/
};
#endif /* _ntbp_membrane_compartment_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */

