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
#include "Object.h"

using namespace mcore;

mbase::Real NTBP_temperature_rate_relation(mbase::Real temp, mbase::Real q10)
{
	return (pow(q10,(temp-NTBP_STD_TEMPERATURE)/10.0));
}
		
mbase::Real NTBP_temperature_rate_relation(mbase::Real temp, mbase::Real baseTemp, mbase::Real q10)
{
	return (pow(q10,(temp-baseTemp)/10.0));
}
		


/** Compute length constant in muMeter */
mbase::Real NTBP_length_constant_passive_cable(mbase::Real diameter /* muM */,
									   mbase::Real rAxoplasmic /* Ohm cm */,
									   mbase::Real gLeak /* mSiemens/cm^2 */)
{
	return ( 1.0e6 /* muMeter/m */ * sqrt( 1.0e-5 /*  m/muMeter/ ((mSiemens/cm^2) Ohm cm) */ * diameter /* muMeter */ / (gLeak * rAxoplasmic * 4.0) ) );
}


mbase::Real Object::timeStep = NTBP_TIMESTEP; //in msec
mbase::Real Object::suggestedTimeStep = NTBP_TIMESTEP; //in msec

/* ***      CONSTRUCTORS	***/
/** Create a Object */
Object::Object()
{

 baseTimeStep  = NTBP_TIMESTEP; //in msec
}

/* ***      COPY AND ASSIGNMENT	***/ 
Object::Object(const Object & original):
		mbase::Obj()
{
	baseTimeStep = original.baseTimeStep;
}

const Object&
Object::operator= (const Object & right)
{
 if (this == &right) return (*this); // Gracefully handle self assignment
 // add assignment code here
  return (*this);
}

/* ***      DESTRUCTOR		***/
Object::~Object()
{
}

/* ***  PUBLIC                                    ***   */  
/** @short       
    @param      none
    @return     none
   \warning    unknown
   \bug        unknown

void
Object::() const
{
}
 */
/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */


/* File skeleton generated by GenNTObj version 0.7. */
