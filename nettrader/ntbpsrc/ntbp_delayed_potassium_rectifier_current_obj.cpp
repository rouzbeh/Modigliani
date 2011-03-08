/**\file ntbp_delayed_potassium_rectifier_current_obj.cpp - squid giant axon sodium conducta nce class implementation 
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
  

/* $Id: ntbp_delayed_potassium_rectifier_current_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_delayed_potassium_rectifier_current_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.1  2001/10/03 14:37:52  face
* *** empty log message ***
*

*/
#include "ntbp_delayed_potassium_rectifier_current_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_delayed_potassium_rectifier_current_o */
NTBP_delayed_potassium_rectifier_current_o::NTBP_delayed_potassium_rectifier_current_o(
			NTreal newArea, 
			enum NTBPdelayedPotassiumRectifierType newType,
			NTreal newDensity
			)
:
NTBP_channels_current_o(-12 /* mV */,0, newArea, 0) // "0" means to be set in constructor (see below)
{								
	NT_ASSERT (newArea >= 0);
	NT_ASSERT (newDensity >= 0);
	type =	newType;
	switch (type)
			{
			case NTBP_SGA_K_CONTI:
				density = 30; // channels per mumeter^2
				conductivity = 0.000000012; // in mSiemens per channel
				break;
			case NTBP_SGA_K_LLANO:
		//	break;
			case NTBP_SNAIL:
		//		break;
			case NTBP_FROG_RANVIER1:
		//		break;
			case NTBP_FROG_RANVIER2:
		//		break;
			case NTBP_FROG_RANVIER3:
		//		break;
			case NTBP_FROG_MUSCLE:
		//		break;
			default:
				cerr <<"NTBP_delayed_potassium_rectifier_current_o::NTBP_delayed_potassium_rectifier_current_o - Warning : current type not implemented yet." << endl;
				break;
			};
		
		if ( 0 != newDensity) density = newDensity;
	
		channelsPtr = new NTBP_potassium_ion_channels_o( density*area );		
}

/* ***      COPY AND ASSIGNMENT	***/ 
NTBP_delayed_potassium_rectifier_current_o::NTBP_delayed_potassium_rectifier_current_o(const NTBP_delayed_potassium_rectifier_current_o & original)
:
NTBP_channels_current_o(original._reversalPotential(), original.density, original.area, original.conductivity)
{
 // add assignment code here
	channelsPtr = new NTBP_potassium_ion_channels_o( density*area );
}

const NTBP_delayed_potassium_rectifier_current_o&  
NTBP_delayed_potassium_rectifier_current_o::operator= (const NTBP_delayed_potassium_rectifier_current_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
	channelsPtr = new NTBP_potassium_ion_channels_o( density*area );
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_delayed_potassium_rectifier_current_o::~NTBP_delayed_potassium_rectifier_current_o()
{
	delete channelsPtr;
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown
 */
inline NTreturn
NTBP_delayed_potassium_rectifier_current_o::StepCurrent()
{
		/*vector < NTreal > vec(2);
		vec[0] = alphaN;
		vec[1] = betaN;
		NT_ASSERT(	(channelsPtr)->UpdateStateProb(vec) == NT_SUCCESS );*/
		return (channelsPtr->Step(voltage));
}

inline void 
NTBP_delayed_potassium_rectifier_current_o::ComputeRateConstants(NTreal voltage)
{
	alphaN = (10.0 - voltage)/(100.0 * (exp(1 - voltage/10.0) - 1) );
	betaN = 0.125 * exp(-voltage / 80.0);
	n += _timeStep() * ((1.0 - n) * alphaN - n * betaN);
	NT_ASSERT(n>=0 &&  n<= 1);
}

/* ***  PROTECTED                         ***   */
NTreturn 
NTBP_delayed_potassium_rectifier_current_o::SetDensity(NTreal channelDensity)// in channels/muMeter^2
{
// 2DO improve : copy the state Besetzungszustaende 
	density = channelDensity;
	delete	channelsPtr;
	channelsPtr = new NTBP_potassium_ion_channels_o( density*area );			
	return NT_SUCCESS;	
} 


/**  */
NTreal NTBP_delayed_potassium_rectifier_current_o::ComputeChannelStateTimeConstant() const
{
	//slow but necessary
	/*vector < NTreal > vec(2);
	vec[0] = alphaN;
	vec[1] = betaN;
	(channelsPtr)->UpdateStateProb(vec);*/
	return channelsPtr->ComputeChannelStateTimeConstant(voltage);
}




/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
/**  */
