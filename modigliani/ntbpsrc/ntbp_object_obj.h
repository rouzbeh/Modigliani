/**\file ntbp_object_obj.h - NT bio-physics related main declaration
 * by Ahmed Aldo Faisal &copy; created 15.3.2001  
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
  

/* $Id: ntbp_object_obj.h,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ /home/face/PRJ/NetTrader/src/ntbpsrc/ntbp_membrane_current_obj.cpp
* $Log: ntbp_object_obj.h,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.4  2003/01/17 16:53:13  face
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
#ifndef _ntbp_object_obj_h_ 
#define _ntbp_object_obj_h_ 

#define NTBP_TIMESTEP 0.001 /* in msec */

/* NT core includes */
#include "nt_main.h" 
#include "nt_types.h" 
#include "nt_obj.h"  
/* NT includes */
#include "nt_error_obj.h"
/* other includes */
#include "nt_physical_constants.h"

enum NTBPKineticFunctionType {NTBP_CONSTANT, NTBP_LINEAR, NTBP, NTBP_FERMI, NTBP_GAUSSIAN};
enum NTBPcurrentType {NTBP_LEAK, NTBP_IONIC, NTBP_EXTERNAL};
enum NTBPstochasticType {NTBP_DETERMINISTIC, NTBP_GILLESPIE, NTBP_LANGEVIN, NTBP_SINGLECHANNEL, NTBP_NOISYMEAN, NTBP_BINOMIALPOPULATION };
enum NTBPdelayedPotassiumRectifierType {NTBP_SGA_K_CONTI, // squid giant axon by Conti et al. (1975)
										NTBP_SGA_SCHNEIDMAN, // values used by Schneidman et al. (1998)
										NTBP_SGA_K_LLANO, // squid giant axon by Llano et al. (1988)
										NTBP_SNAIL, // snail neuron by Reuter & Stevens (1980)
										NTBP_FROG_RANVIER1, // frog ranvier node by Begenisich & Stevens (1975)
										NTBP_FROG_RANVIER2, // frog ranvier node by Neumcke ety al. (1980)
										NTBP_FROG_RANVIER3, // frog ranvier node by Vogel et al. (1991)
										NTBP_FROG_MUSCLE // frog skeletal muscle by Standen et al. (1985)
										};

const NTreal NTBP_STD_TEMPERATURE = 6.3; // Celsius (NOT TO BE CHANGED)

NTreal NTBP_TemperatureRateRelation(NTreal temp /* C */, NTreal q10);
NTreal NTBP_TemperatureRateRelation(NTreal temp /* C */, NTreal baseTemp /* C */, NTreal q10);
/** Compute length constant in muMeter */
NTreal NTBP_LengthConstantPassiveCable(NTreal diameter /* mu */, NTreal rAxoplasmic /* Ohm cm */, NTreal gLeak /* mSiemens/cm^2 */);
																
/** @short NTBP_object_o class
\bug unknown
\warning unknown 
TIMESTEP SHOULD BECOME STATIC! FOR GLOBAL CHANGES!
*/

#define NTBP_DEBUG 1

class NTBP_object_o : public NT_o {
public:
/***   Constructors, Copy/Assignment and Destructor  ***/  
NTBP_object_o();
NTBP_object_o(const NTBP_object_o & original);
const NTBP_object_o & operator= (const NTBP_object_o & right);
virtual ~NTBP_object_o();
/* ***  Methods              ***/  
NTreal _timeStep() const { return timeStep; }
/** @short  update of explicitly time dependent variables using a deltaT = timeStep, NO update
  	of dependent variables     
    @param      none
    @return     none
   \warning    indirectly time dependent variables NOT updated
   \bug        unknown
*/
NTreal UpdateTimeStep(NTreal newSuggestedTimeStep) { /* if (suggestedTimeStep > newSuggestedTimeStep) */ suggestedTimeStep = newSuggestedTimeStep; return newSuggestedTimeStep; }
virtual NTreturn Step() { cerr << "NTBP_object_o:Step() - Warning : Non-overriden base method () called by derived class." << endl; return NT_NOT_IMPLEMENTED; }
NTreturn StepNTBP() { timeStep = suggestedTimeStep; return NT_SUCCESS;}
NTreal _timeStep() { return timeStep;}
NTreal _baseTimeStep(){ return baseTimeStep;}
void setTimeStep(NTreal newTimeStep){
	timeStep = newTimeStep;
}
/* ***  Data                 ***/
protected:
/* ***  Methods              ***/
/* ***  Data                 ***/
private:
/* ***  Methods              ***/
/* ***  Data                 ***/
NTreal baseTimeStep; // time step size in msec;
static NTreal timeStep; // time step size in msec;
static NTreal suggestedTimeStep; // time step size in msec;
};
#endif /* _ntbp_object_obj_h_ */ 

/* File skeleton generated by GenNTObj version 0.7. */
