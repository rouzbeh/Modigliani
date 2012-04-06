/**\file ntbp_object_obj.cpp -  NT bio-physics related main object implementation
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
  

/* $Id: ntbp_object_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
* $Log: ntbp_object_obj.cpp,v $
* Revision 1.1.1.1  2004/12/16 01:38:36  face
* Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
*
* Revision 1.4  2003/08/14 16:43:27  face
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
#include "ntbp_object_obj.h" 

NTreal NTBP_TemperatureRateRelation(NTreal temp, NTreal q10)
{
	return pow(q10,(temp-NTBP_STD_TEMPERATURE)/10.0);
}
		
NTreal NTBP_TemperatureRateRelation(NTreal temp, NTreal baseTemp, NTreal q10)
{
	return pow(q10,(temp-baseTemp)/10.0);
}
		


/** Compute length constant in muMeter */
NTreal NTBP_LengthConstantPassiveCable(NTreal diameter /* muM */,
									   NTreal rAxoplasmic /* Ohm cm */,
									   NTreal gLeak /* mSiemens/cm^2 */)
{
	return ( 1.0e6 /* muMeter/m */ * sqrt( 1.0e-5 /*  m/muMeter/ ((mSiemens/cm^2) Ohm cm) */ * diameter /* muMeter */ / (gLeak * rAxoplasmic * 4.0) ) );
}


NTreal NTBP_object_o::timeStep = NTBP_TIMESTEP; //in msec
NTreal NTBP_object_o::suggestedTimeStep = NTBP_TIMESTEP; //in msec

/* ***      CONSTRUCTORS	***/
/** Create a NTBP_object_o */
NTBP_object_o::NTBP_object_o()
{

 baseTimeStep  = NTBP_TIMESTEP; //in msec
}

/* ***      COPY AND ASSIGNMENT	***/ 
NTBP_object_o::NTBP_object_o(const NTBP_object_o & original):
		NT_o()
{
	baseTimeStep = original.baseTimeStep;
}

const NTBP_object_o&  
NTBP_object_o::operator= (const NTBP_object_o & right)
{
 if (this == &right) return *this; // Gracefully handle self assignment
 // add assignment code here
  return *this;
}

/* ***      DESTRUCTOR		***/
NTBP_object_o::~NTBP_object_o()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown

void
NTBP_object_o::() const
{
}
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */